/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
 *    All rights reserved.
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

/* This file contains the declarations for the Matter OTA Requestor implementation and API.
 * Applications implementing the OTA Requestor functionality must include this file.
 */

#pragma once

#include <app/CASESessionManager.h>
#include <app/server/Server.h>
#include <protocols/bdx/BdxMessages.h>

#include "BDXDownloader.h"
#include "OTARequestorDriver.h"
#include "OTARequestorInterface.h"
#include "OTARequestorStorage.h"

namespace chip {

// This class implements all of the core logic of the OTA Requestor
class OTARequestor : public OTARequestorInterface, public BDXDownloader::StateDelegate
{
public:
    OTARequestor() : mOnConnectedCallback(OnConnected, this), mOnConnectionFailureCallback(OnConnectionFailure, this) {}

    //////////// OTARequestorInterface Implementation ///////////////
    EmberAfStatus HandleAnnounceOTAProvider(
        app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
        const app::Clusters::OtaSoftwareUpdateRequestor::Commands::AnnounceOtaProvider::DecodableType & commandData) override;

    // Application API to send the QueryImage command and start the image update process with the next available Provider
    OTATriggerResult TriggerImmediateQuery() override;

    // Internal API meant for use by OTARequestorDriver to send the QueryImage command and start the image update process
    // with the Provider currently set in the OTARequestor
    void TriggerImmediateQueryInternal() override;

    // Initiate download of the new image
    void DownloadUpdate() override;

    // Set the requestor state to kDelayedOnUserConsent
    void DownloadUpdateDelayedOnUserConsent() override;

    // Initiate the session to send ApplyUpdateRequest command
    void ApplyUpdate() override;

    // Initiate the session to send NotifyUpdateApplied command
    void NotifyUpdateApplied() override;

    // Get image update progress in percents unit
    CHIP_ERROR GetUpdateProgress(EndpointId endpointId, app::DataModel::Nullable<uint8_t> & progress) override;

    // Get requestor state
    CHIP_ERROR GetState(EndpointId endpointId, OTAUpdateStateEnum & state) override;

    // Get the current state of the OTA update
    OTAUpdateStateEnum GetCurrentUpdateState() override { return mCurrentUpdateState; }

    // Application directs the Requestor to cancel image update in progress. All the Requestor state is
    // cleared, UpdateState is reset to Idle
    void CancelImageUpdate() override;

    // Clear all entries with the specified fabric index in the default OTA provider list
    CHIP_ERROR ClearDefaultOtaProviderList(FabricIndex fabricIndex) override;

    void SetCurrentProviderLocation(ProviderLocationType providerLocation) override
    {
        mProviderLocation.SetValue(providerLocation);
    }

    void GetProviderLocation(Optional<ProviderLocationType> & providerLocation) override { providerLocation = mProviderLocation; }

    // Add a default OTA provider to the cached list
    CHIP_ERROR AddDefaultOtaProvider(const ProviderLocationType & providerLocation) override;

    // Retrieve an iterator to the cached default OTA provider list
    ProviderLocationList::Iterator GetDefaultOTAProviderListIterator(void) override { return mDefaultOtaProviderList.Begin(); }

    //////////// BDXDownloader::StateDelegate Implementation ///////////////
    void OnDownloadStateChanged(OTADownloader::State state,
                                app::Clusters::OtaSoftwareUpdateRequestor::OTAChangeReasonEnum reason) override;
    void OnUpdateProgressChanged(app::DataModel::Nullable<uint8_t> percent) override;

    //////////// OTARequestor public APIs ///////////////

    /**
     * Called to perform some initialization including:
     *   - Set server instance used to get access to the system resources necessary to open CASE sessions and drive
     *     BDX transfers
     *   - Set the OTA requestor driver instance used to communicate download progress and errors
     *   - Set the BDX downloader instance used for initiating BDX downloads
     */
    CHIP_ERROR Init(Server & server, OTARequestorStorage & storage, OTARequestorDriver & driver, BDXDownloader & downloader)
    {
        mServer             = &server;
        mCASESessionManager = server.GetCASESessionManager();
        mStorage            = &storage;
        mOtaRequestorDriver = &driver;
        mBdxDownloader      = &downloader;

        ReturnErrorOnFailure(DeviceLayer::ConfigurationMgr().GetSoftwareVersion(mCurrentVersion));

        storage.LoadDefaultProviders(mDefaultOtaProviderList);

        ProviderLocationType providerLocation;
        if (storage.LoadCurrentProviderLocation(providerLocation) == CHIP_NO_ERROR)
        {
            mProviderLocation.SetValue(providerLocation);
        }

        MutableByteSpan updateToken(mUpdateTokenBuffer);
        if (storage.LoadUpdateToken(updateToken) == CHIP_NO_ERROR)
        {
            mUpdateToken = updateToken;
        }


        // Schedule the initializations that needs to be performed in the CHIP context
        DeviceLayer::PlatformMgr().ScheduleWork(InitState, reinterpret_cast<intptr_t>(this));

        return chip::DeviceLayer::PlatformMgrImpl().AddEventHandler(OnCommissioningCompleteRequestor,
                                                                    reinterpret_cast<intptr_t>(this));
    }

private:
    using QueryImageResponseDecodableType  = app::Clusters::OtaSoftwareUpdateProvider::Commands::QueryImageResponse::DecodableType;
    using ApplyUpdateResponseDecodableType = app::Clusters::OtaSoftwareUpdateProvider::Commands::ApplyUpdateResponse::DecodableType;

    using OTAChangeReasonEnum = app::Clusters::OtaSoftwareUpdateRequestor::OTAChangeReasonEnum;

    static constexpr size_t kMaxUpdateTokenLen = 32;

    // TODO: the application should define this, along with initializing the BDXDownloader

    // This class is purely for delivering messages and sending outgoing messages to/from the BDXDownloader.
    class BDXMessenger : public chip::BDXDownloader::MessagingDelegate, public chip::Messaging::ExchangeDelegate
    {
    public:
        CHIP_ERROR SendMessage(const chip::bdx::TransferSession::OutputEvent & event) override
        {
            ChipLogDetail(SoftwareUpdate, "BDX::SendMessage");
            VerifyOrReturnError(mExchangeCtx != nullptr, CHIP_ERROR_INCORRECT_STATE);

            chip::Messaging::SendFlags sendFlags;
            if (!event.msgTypeData.HasMessageType(chip::bdx::MessageType::BlockAckEOF) &&
                !event.msgTypeData.HasMessageType(chip::Protocols::SecureChannel::MsgType::StatusReport))
            {
                sendFlags.Set(chip::Messaging::SendMessageFlags::kExpectResponse);
            }
            ReturnErrorOnFailure(mExchangeCtx->SendMessage(event.msgTypeData.ProtocolId, event.msgTypeData.MessageType,
                                                           event.MsgData.Retain(), sendFlags));
            return CHIP_NO_ERROR;
        }

        CHIP_ERROR OnMessageReceived(chip::Messaging::ExchangeContext * ec, const chip::PayloadHeader & payloadHeader,
                                     chip::System::PacketBufferHandle && payload) override
        {
            if (mDownloader == nullptr)
            {
                ChipLogError(BDX, "BDXDownloader instance is null, can't pass message");
                return CHIP_NO_ERROR;
            }
            else
            {
                mDownloader->OnMessageReceived(payloadHeader, payload.Retain());
            }

            // For a receiver using BDX Protocol, all received messages will require a response except for a StatusReport
            if (!payloadHeader.HasMessageType(chip::Protocols::SecureChannel::MsgType::StatusReport))
            {
                ec->WillSendMessage();
            }
            return CHIP_NO_ERROR;
        }

        void OnResponseTimeout(chip::Messaging::ExchangeContext * ec) override
        {
            ChipLogError(BDX, "exchange timed out");
            if (mDownloader != nullptr)
            {
                mDownloader->OnDownloadTimeout();
            }
        }

        void Init(chip::BDXDownloader * downloader, chip::Messaging::ExchangeContext * ec)
        {
            mExchangeCtx = ec;
            mDownloader  = downloader;
        }

    private:
        chip::Messaging::ExchangeContext * mExchangeCtx;
        chip::BDXDownloader * mDownloader;
    };

    /**
     * Callback to initialize states and server attributes in the CHIP context
     */
    static void InitState(intptr_t context);

    /**
     * Map a CHIP_ERROR to an IdleStateReason enum type
     */
    IdleStateReason MapErrorToIdleStateReason(CHIP_ERROR error);

    /**
     * Record the new update state by updating the corresponding server attribute and logging a StateTransition event
     */
    void RecordNewUpdateState(OTAUpdateStateEnum newState, OTAChangeReasonEnum reason, CHIP_ERROR error = CHIP_NO_ERROR);

    /**
     * Record the error update state by informing the driver of the error and calling `RecordNewUpdateState`
     */
    void RecordErrorUpdateState(UpdateFailureState failureState, CHIP_ERROR error,
                                OTAChangeReasonEnum reason = OTAChangeReasonEnum::kFailure);

    /**
     * Generate an update token using the operational node ID in case of token lost, received in QueryImageResponse
     */
    CHIP_ERROR GenerateUpdateToken();

    /**
     * Send QueryImage request using values matching Basic cluster
     */
    CHIP_ERROR SendQueryImageRequest(OperationalDeviceProxy & deviceProxy);

    /**
     * Validate and extract mandatory information from QueryImageResponse
     */
    CHIP_ERROR ExtractUpdateDescription(const QueryImageResponseDecodableType & response, UpdateDescription & update) const;

    // Various actions to take when OnConnected callback is called
    enum OnConnectedAction
    {
        kQueryImage = 0,
        kDownload,
        kApplyUpdate,
        kNotifyUpdateApplied,
    };

    /**
     * Called to establish a session to provider indicated by mProviderLocation
     *
     * @param onConnectedAction  The action to take once session to provider has been established
     */
    void ConnectToProvider(OnConnectedAction onConnectedAction);

    /**
     * Called to tear down a session to provider indicated by mProviderLocation
     */
    void DisconnectFromProvider();

    /**
     * Start download of the software image returned in QueryImageResponse
     */
    CHIP_ERROR StartDownload(OperationalDeviceProxy & deviceProxy);

    /**
     * Send ApplyUpdate request using values obtained from QueryImageResponse
     */
    CHIP_ERROR SendApplyUpdateRequest(OperationalDeviceProxy & deviceProxy);

    /**
     * Send NotifyUpdateApplied request
     */
    CHIP_ERROR SendNotifyUpdateAppliedRequest(OperationalDeviceProxy & deviceProxy);

    /**
     * Store current update information to KVS
     */
    void StoreCurrentUpdateInfo();

    /**
     * Session connection callbacks
     */
    static void OnConnected(void * context, OperationalDeviceProxy * deviceProxy);
    static void OnConnectionFailure(void * context, PeerId peerId, CHIP_ERROR error);
    Callback::Callback<OnDeviceConnected> mOnConnectedCallback;
    Callback::Callback<OnDeviceConnectionFailure> mOnConnectionFailureCallback;

    /**
     * QueryImage callbacks
     */
    static void OnQueryImageResponse(void * context, const QueryImageResponseDecodableType & response);
    static void OnQueryImageFailure(void * context, CHIP_ERROR error);

    /**
     * ApplyUpdate callbacks
     */
    static void OnApplyUpdateResponse(void * context, const ApplyUpdateResponseDecodableType & response);
    static void OnApplyUpdateFailure(void * context, CHIP_ERROR error);

    /**
     * NotifyUpdateApplied callbacks
     */
    static void OnNotifyUpdateAppliedResponse(void * context, const app::DataModel::NullObjectType & response);
    static void OnNotifyUpdateAppliedFailure(void * context, CHIP_ERROR error);

    /**
     * Commissioning callback
     */
    static void OnCommissioningCompleteRequestor(const DeviceLayer::ChipDeviceEvent * event, intptr_t arg);

    OTARequestorStorage * mStorage            = nullptr;
    OTARequestorDriver * mOtaRequestorDriver  = nullptr;
    CASESessionManager * mCASESessionManager  = nullptr;
    OnConnectedAction mOnConnectedAction      = kQueryImage;
    Messaging::ExchangeContext * mExchangeCtx = nullptr;
    BDXDownloader * mBdxDownloader            = nullptr; // TODO: this should be OTADownloader
    BDXMessenger mBdxMessenger;                          // TODO: ideally this is held by the application
    uint8_t mUpdateTokenBuffer[kMaxUpdateTokenLen];
    ByteSpan mUpdateToken;
    uint32_t mCurrentVersion = 0;
    uint32_t mTargetVersion  = 0;
    char mFileDesignatorBuffer[bdx::kMaxFileDesignatorLen];
    CharSpan mFileDesignator;
    OTAUpdateStateEnum mCurrentUpdateState = OTAUpdateStateEnum::kUnknown;
    Server * mServer                       = nullptr;
    ProviderLocationList mDefaultOtaProviderList;
    Optional<ProviderLocationType> mProviderLocation; // Provider location used for the current/last update in progress
};

} // namespace chip
