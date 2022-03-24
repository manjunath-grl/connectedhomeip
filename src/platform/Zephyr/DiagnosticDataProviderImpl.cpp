/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          Provides an implementation of the DiagnosticDataProvider object
 *          for Zephy platform.
 */

#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <lib/support/logging/CHIPLogging.h>
#include <platform/DiagnosticDataProvider.h>
#include <platform/Zephyr/DiagnosticDataProviderImpl.h>

#include <drivers/hwinfo.h>

#ifdef CONFIG_MCUBOOT_IMG_MANAGER
#include <dfu/mcuboot.h>
#endif

#include <malloc.h>

namespace chip {
namespace DeviceLayer {

namespace {

uint8_t DetermineBootReason()
{
#ifdef CONFIG_HWINFO
    uint32_t reason;

    if (hwinfo_get_reset_cause(&reason) != 0)
    {
        return BootReasonType::Unspecified;
    }

    // Bits returned by hwinfo_get_reset_cause() are accumulated between subsequent resets, so
    // the reset cause must be cleared after reading in order to make sure it always contains
    // information about the most recent boot only.
    (void) hwinfo_clear_reset_cause();

    // If no reset cause is provided, it indicates a power-on-reset.
    if (reason == 0 || reason & (RESET_POR | RESET_PIN))
    {
        return BootReasonType::PowerOnReboot;
    }

    if (reason & RESET_WATCHDOG)
    {
        return BootReasonType::HardwareWatchdogReset;
    }

    if (reason & RESET_BROWNOUT)
    {
        return BootReasonType::BrownOutReset;
    }

    if (reason & RESET_SOFTWARE)
    {
#ifdef CONFIG_MCUBOOT_IMG_MANAGER
        if (mcuboot_swap_type() == BOOT_SWAP_TYPE_REVERT)
        {
            return BootReasonType::SoftwareUpdateCompleted;
        }
#endif
        return BootReasonType::SoftwareReset;
    }
#endif

    return BootReasonType::Unspecified;
}

} // namespace

DiagnosticDataProviderImpl & DiagnosticDataProviderImpl::GetDefaultInstance()
{
    static DiagnosticDataProviderImpl sInstance;
    return sInstance;
}

inline DiagnosticDataProviderImpl::DiagnosticDataProviderImpl() : mBootReason(DetermineBootReason())
{
    ChipLogDetail(DeviceLayer, "Boot reason: %" PRIu16, static_cast<uint16_t>(mBootReason));
}

CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapFree(uint64_t & currentHeapFree)
{
#ifdef CONFIG_NEWLIB_LIBC
    // This will return the amount of memory which has been allocated from the system, but is not
    // used right now. Ideally, this value should be increased by the amount of memory which can
    // be allocated from the system, but Zephyr does not expose that number.
    currentHeapFree = mallinfo().fordblks;
    return CHIP_NO_ERROR;
#else
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
#endif
}

CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapUsed(uint64_t & currentHeapUsed)
{
#ifdef CONFIG_NEWLIB_LIBC
    currentHeapUsed = mallinfo().uordblks;
    return CHIP_NO_ERROR;
#else
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
#endif
}

CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapHighWatermark(uint64_t & currentHeapHighWatermark)
{
#ifdef CONFIG_NEWLIB_LIBC
    // ARM newlib does not provide a way to obtain the peak heap usage, so for now just return
    // the amount of memory allocated from the system which should be an upper bound of the peak
    // usage provided that the heap is not very fragmented.
    currentHeapHighWatermark = mallinfo().arena;
    return CHIP_NO_ERROR;
#else
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
#endif
}

CHIP_ERROR DiagnosticDataProviderImpl::GetRebootCount(uint16_t & rebootCount)
{
    uint32_t count = 0;
    CHIP_ERROR err = ConfigurationMgr().GetRebootCount(count);

    if (err == CHIP_NO_ERROR)
    {
        // If the value overflows, return UINT16 max value to provide best-effort number.
        rebootCount = static_cast<uint16_t>(count <= UINT16_MAX ? count : UINT16_MAX);
    }

    return err;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetUpTime(uint64_t & upTime)
{
    System::Clock::Timestamp currentTime = System::SystemClock().GetMonotonicTimestamp();
    System::Clock::Timestamp startTime   = PlatformMgrImpl().GetStartTime();

    if (currentTime >= startTime)
    {
        upTime = std::chrono::duration_cast<System::Clock::Seconds64>(currentTime - startTime).count();
        return CHIP_NO_ERROR;
    }

    return CHIP_ERROR_INVALID_TIME;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetTotalOperationalHours(uint32_t & totalOperationalHours)
{
    uint64_t upTimeS;

    ReturnErrorOnFailure(GetUpTime(upTimeS));

    uint64_t totalHours      = 0;
    const uint32_t upTimeH   = upTimeS / 3600 < UINT32_MAX ? static_cast<uint32_t>(upTimeS / 3600) : UINT32_MAX;
    const uint64_t deltaTime = upTimeH - PlatformMgrImpl().GetSavedOperationalHoursSinceBoot();

    ReturnErrorOnFailure(ConfigurationMgr().GetTotalOperationalHours(reinterpret_cast<uint32_t &>(totalHours)));

    totalOperationalHours = totalHours + deltaTime < UINT32_MAX ? totalHours + deltaTime : UINT32_MAX;

    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetBootReason(uint8_t & bootReason)
{
#if CONFIG_HWINFO
    bootReason = mBootReason;
    return CHIP_NO_ERROR;
#else
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
#endif
}

CHIP_ERROR DiagnosticDataProviderImpl::GetNetworkInterfaces(NetworkInterface ** netifpp)
{
    NetworkInterface * head = NULL;

    for (Inet::InterfaceIterator interfaceIterator; interfaceIterator.HasCurrent(); interfaceIterator.Next())
    {
        NetworkInterface * ifp = new NetworkInterface();

        interfaceIterator.GetInterfaceName(ifp->Name, Inet::InterfaceId::kMaxIfNameLength);
        ifp->name          = CharSpan::fromCharString(ifp->Name);
        ifp->isOperational = true;
        Inet::InterfaceType interfaceType;
        if (interfaceIterator.GetInterfaceType(interfaceType) == CHIP_NO_ERROR)
        {
            switch (interfaceType)
            {
            case Inet::InterfaceType::Unknown:
                ifp->type = EMBER_ZCL_INTERFACE_TYPE_UNSPECIFIED;
                break;
            case Inet::InterfaceType::WiFi:
                ifp->type = EMBER_ZCL_INTERFACE_TYPE_WI_FI;
                break;
            case Inet::InterfaceType::Ethernet:
                ifp->type = EMBER_ZCL_INTERFACE_TYPE_ETHERNET;
                break;
            case Inet::InterfaceType::Thread:
                ifp->type = EMBER_ZCL_INTERFACE_TYPE_THREAD;
                break;
            case Inet::InterfaceType::Cellular:
                ifp->type = EMBER_ZCL_INTERFACE_TYPE_CELLULAR;
                break;
            }
        }
        else
        {
            ChipLogError(DeviceLayer, "Failed to get interface type");
        }

        ifp->offPremiseServicesReachableIPv4.SetNull();
        ifp->offPremiseServicesReachableIPv6.SetNull();

        uint8_t addressSize;
        if (interfaceIterator.GetHardwareAddress(ifp->MacAddress, addressSize, sizeof(ifp->MacAddress)) != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "Failed to get network hardware address");
        }
        else
        {
            ifp->hardwareAddress = ByteSpan(ifp->MacAddress, addressSize);
        }

        // Assuming IPv6-only support
        Inet::InterfaceAddressIterator interfaceAddressIterator;
        uint8_t ipv6AddressesCount = 0;
        while (interfaceAddressIterator.HasCurrent() && ipv6AddressesCount < kMaxIPv6AddrCount)
        {
            if (interfaceAddressIterator.GetInterfaceId() == interfaceIterator.GetInterfaceId())
            {
                chip::Inet::IPAddress ipv6Address;
                if (interfaceAddressIterator.GetAddress(ipv6Address) == CHIP_NO_ERROR)
                {
                    memcpy(ifp->Ipv6AddressesBuffer[ipv6AddressesCount], ipv6Address.Addr, kMaxIPv6AddrSize);
                    ifp->Ipv6AddressSpans[ipv6AddressesCount] = ByteSpan(ifp->Ipv6AddressesBuffer[ipv6AddressesCount]);
                    ipv6AddressesCount++;
                }
            }
            interfaceAddressIterator.Next();
        }

        ifp->IPv6Addresses = chip::app::DataModel::List<chip::ByteSpan>(ifp->Ipv6AddressSpans, ipv6AddressesCount);
        head               = ifp;
    }

    *netifpp = head;
    return CHIP_NO_ERROR;
}

void DiagnosticDataProviderImpl::ReleaseNetworkInterfaces(NetworkInterface * netifp)
{
    while (netifp)
    {
        NetworkInterface * del = netifp;
        netifp                 = netifp->Next;
        delete del;
    }
}

} // namespace DeviceLayer
} // namespace chip
