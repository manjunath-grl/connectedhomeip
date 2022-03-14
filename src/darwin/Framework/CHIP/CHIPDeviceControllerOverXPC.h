/**
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

#import <CHIP/CHIPDeviceController.h>

NS_ASSUME_NONNULL_BEGIN

@class CHIPDeviceControllerXPCConnection;

@interface CHIPDeviceControllerOverXPC : CHIPDeviceController

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/**
 * Returns a shared remote device controller associated with an implementation specific id and implementation specific way to
 * connect to an XPC listener.
 */
+ (CHIPDeviceControllerOverXPC *)sharedControllerWithId:(id<NSCopying> _Nullable)controllerId
                                        xpcConnectBlock:(NSXPCConnection * (^)(void) )connectBlock;

@end

NS_ASSUME_NONNULL_END
