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

// Prevent multiple inclusion
#pragma once

// User options for plugin Binding Table Library
#define EMBER_BINDING_TABLE_SIZE 10

/**** Network Section ****/
#define EMBER_SUPPORTED_NETWORKS (1)

#define EMBER_APS_UNICAST_MESSAGE_COUNT 10

/**** Cluster endpoint counts ****/
#define EMBER_AF_ACCESS_CONTROL_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_DIAGNOSTIC_LOGS_CLUSTER_CLIENT_ENDPOINT_COUNT (1)
#define EMBER_AF_DIAGNOSTIC_LOGS_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_GENERAL_COMMISSIONING_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_NETWORK_COMMISSIONING_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_OPERATIONAL_CREDENTIALS_CLUSTER_SERVER_ENDPOINT_COUNT (1)

/**** Cluster Plugins ****/

// Use this macro to check if the server side of the Access Control cluster is included
#define ZCL_USING_ACCESS_CONTROL_CLUSTER_SERVER
#define EMBER_AF_PLUGIN_ACCESS_CONTROL_SERVER
#define EMBER_AF_PLUGIN_ACCESS_CONTROL

// Use this macro to check if the client side of the Diagnostic Logs cluster is included
#define ZCL_USING_DIAGNOSTIC_LOGS_CLUSTER_CLIENT
#define EMBER_AF_PLUGIN_DIAGNOSTIC_LOGS_CLIENT

// Use this macro to check if the server side of the Diagnostic Logs cluster is included
#define ZCL_USING_DIAGNOSTIC_LOGS_CLUSTER_SERVER
#define EMBER_AF_PLUGIN_DIAGNOSTIC_LOGS_SERVER
#define EMBER_AF_PLUGIN_DIAGNOSTIC_LOGS

// Use this macro to check if the server side of the General Commissioning cluster is included
#define ZCL_USING_GENERAL_COMMISSIONING_CLUSTER_SERVER
#define EMBER_AF_PLUGIN_GENERAL_COMMISSIONING_SERVER
#define EMBER_AF_PLUGIN_GENERAL_COMMISSIONING

// Use this macro to check if the server side of the Network Commissioning cluster is included
#define ZCL_USING_NETWORK_COMMISSIONING_CLUSTER_SERVER
#define EMBER_AF_PLUGIN_NETWORK_COMMISSIONING_SERVER
#define EMBER_AF_PLUGIN_NETWORK_COMMISSIONING

// Use this macro to check if the server side of the Operational Credentials cluster is included
#define ZCL_USING_OPERATIONAL_CREDENTIALS_CLUSTER_SERVER
#define EMBER_AF_PLUGIN_OPERATIONAL_CREDENTIALS_SERVER
#define EMBER_AF_PLUGIN_OPERATIONAL_CREDENTIALS
