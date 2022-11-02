/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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

// THIS FILE IS GENERATED BY ZAP

#include <app-common/zap-generated/callback.h>
#include <app-common/zap-generated/cluster-id.h>
#include <lib/support/Span.h>
#include <protocols/interaction_model/Constants.h>

using namespace chip;

// Cluster Init Functions
void emberAfClusterInitCallback(EndpointId endpoint, ClusterId clusterId)
{
    switch (clusterId)
    {
    case ZCL_ACCESS_CONTROL_CLUSTER_ID:
        emberAfAccessControlClusterInitCallback(endpoint);
        break;
    case ZCL_ACCOUNT_LOGIN_CLUSTER_ID:
        emberAfAccountLoginClusterInitCallback(endpoint);
        break;
    case ZCL_ADMINISTRATOR_COMMISSIONING_CLUSTER_ID:
        emberAfAdministratorCommissioningClusterInitCallback(endpoint);
        break;
    case ZCL_APPLICATION_BASIC_CLUSTER_ID:
        emberAfApplicationBasicClusterInitCallback(endpoint);
        break;
    case ZCL_APPLICATION_LAUNCHER_CLUSTER_ID:
        emberAfApplicationLauncherClusterInitCallback(endpoint);
        break;
    case ZCL_AUDIO_OUTPUT_CLUSTER_ID:
        emberAfAudioOutputClusterInitCallback(endpoint);
        break;
    case ZCL_BARRIER_CONTROL_CLUSTER_ID:
        emberAfBarrierControlClusterInitCallback(endpoint);
        break;
    case ZCL_BASIC_CLUSTER_ID:
        emberAfBasicClusterInitCallback(endpoint);
        break;
    case ZCL_BINARY_INPUT_BASIC_CLUSTER_ID:
        emberAfBinaryInputBasicClusterInitCallback(endpoint);
        break;
    case ZCL_BINDING_CLUSTER_ID:
        emberAfBindingClusterInitCallback(endpoint);
        break;
    case ZCL_CHANNEL_CLUSTER_ID:
        emberAfChannelClusterInitCallback(endpoint);
        break;
    case ZCL_CONTENT_LAUNCH_CLUSTER_ID:
        emberAfContentLauncherClusterInitCallback(endpoint);
        break;
    case ZCL_DESCRIPTOR_CLUSTER_ID:
        emberAfDescriptorClusterInitCallback(endpoint);
        break;
    case ZCL_DIAGNOSTIC_LOGS_CLUSTER_ID:
        emberAfDiagnosticLogsClusterInitCallback(endpoint);
        break;
    case ZCL_ETHERNET_NETWORK_DIAGNOSTICS_CLUSTER_ID:
        emberAfEthernetNetworkDiagnosticsClusterInitCallback(endpoint);
        break;
    case ZCL_FIXED_LABEL_CLUSTER_ID:
        emberAfFixedLabelClusterInitCallback(endpoint);
        break;
    case ZCL_GENERAL_COMMISSIONING_CLUSTER_ID:
        emberAfGeneralCommissioningClusterInitCallback(endpoint);
        break;
    case ZCL_GENERAL_DIAGNOSTICS_CLUSTER_ID:
        emberAfGeneralDiagnosticsClusterInitCallback(endpoint);
        break;
    case ZCL_GROUP_KEY_MANAGEMENT_CLUSTER_ID:
        emberAfGroupKeyManagementClusterInitCallback(endpoint);
        break;
    case ZCL_GROUPS_CLUSTER_ID:
        emberAfGroupsClusterInitCallback(endpoint);
        break;
    case ZCL_IDENTIFY_CLUSTER_ID:
        emberAfIdentifyClusterInitCallback(endpoint);
        break;
    case ZCL_KEYPAD_INPUT_CLUSTER_ID:
        emberAfKeypadInputClusterInitCallback(endpoint);
        break;
    case ZCL_LEVEL_CONTROL_CLUSTER_ID:
        emberAfLevelControlClusterInitCallback(endpoint);
        break;
    case ZCL_LOCALIZATION_CONFIGURATION_CLUSTER_ID:
        emberAfLocalizationConfigurationClusterInitCallback(endpoint);
        break;
    case ZCL_MEDIA_INPUT_CLUSTER_ID:
        emberAfMediaInputClusterInitCallback(endpoint);
        break;
    case ZCL_MEDIA_PLAYBACK_CLUSTER_ID:
        emberAfMediaPlaybackClusterInitCallback(endpoint);
        break;
    case ZCL_NETWORK_COMMISSIONING_CLUSTER_ID:
        emberAfNetworkCommissioningClusterInitCallback(endpoint);
        break;
    case ZCL_OTA_PROVIDER_CLUSTER_ID:
        emberAfOtaSoftwareUpdateProviderClusterInitCallback(endpoint);
        break;
    case ZCL_ON_OFF_CLUSTER_ID:
        emberAfOnOffClusterInitCallback(endpoint);
        break;
    case ZCL_OPERATIONAL_CREDENTIALS_CLUSTER_ID:
        emberAfOperationalCredentialsClusterInitCallback(endpoint);
        break;
    case ZCL_SCENES_CLUSTER_ID:
        emberAfScenesClusterInitCallback(endpoint);
        break;
    case ZCL_SOFTWARE_DIAGNOSTICS_CLUSTER_ID:
        emberAfSoftwareDiagnosticsClusterInitCallback(endpoint);
        break;
    case ZCL_SWITCH_CLUSTER_ID:
        emberAfSwitchClusterInitCallback(endpoint);
        break;
    case ZCL_TARGET_NAVIGATOR_CLUSTER_ID:
        emberAfTargetNavigatorClusterInitCallback(endpoint);
        break;
    case ZCL_THREAD_NETWORK_DIAGNOSTICS_CLUSTER_ID:
        emberAfThreadNetworkDiagnosticsClusterInitCallback(endpoint);
        break;
    case ZCL_TIME_FORMAT_LOCALIZATION_CLUSTER_ID:
        emberAfTimeFormatLocalizationClusterInitCallback(endpoint);
        break;
    case ZCL_UNIT_LOCALIZATION_CLUSTER_ID:
        emberAfUnitLocalizationClusterInitCallback(endpoint);
        break;
    case ZCL_USER_LABEL_CLUSTER_ID:
        emberAfUserLabelClusterInitCallback(endpoint);
        break;
    case ZCL_WAKE_ON_LAN_CLUSTER_ID:
        emberAfWakeOnLanClusterInitCallback(endpoint);
        break;
    case ZCL_WIFI_NETWORK_DIAGNOSTICS_CLUSTER_ID:
        emberAfWiFiNetworkDiagnosticsClusterInitCallback(endpoint);
        break;
    default:
        // Unrecognized cluster ID
        break;
    }
}

void __attribute__((weak)) emberAfAccessControlClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfAccountLoginClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfAdministratorCommissioningClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfApplicationBasicClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfApplicationLauncherClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfAudioOutputClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfBarrierControlClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfBasicClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfBinaryInputBasicClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfBindingClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfChannelClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfContentLauncherClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfDescriptorClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfDiagnosticLogsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfEthernetNetworkDiagnosticsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfFixedLabelClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfGeneralCommissioningClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfGeneralDiagnosticsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfGroupKeyManagementClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfGroupsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfIdentifyClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfKeypadInputClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfLevelControlClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfLocalizationConfigurationClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfMediaInputClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfMediaPlaybackClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfNetworkCommissioningClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfOtaSoftwareUpdateProviderClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfOnOffClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfOperationalCredentialsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfScenesClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfSoftwareDiagnosticsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfSwitchClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfTargetNavigatorClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfThreadNetworkDiagnosticsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfTimeFormatLocalizationClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfUnitLocalizationClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfUserLabelClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfWakeOnLanClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
void __attribute__((weak)) emberAfWiFiNetworkDiagnosticsClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}
