/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "VendorVoldNativeService.h"

#include <android-base/logging.h>
#include <binder/IServiceManager.h>
#include <private/android_filesystem_config.h>

#include "Checkpoint.h"
#include "VoldNativeServiceValidation.h"

#define ENFORCE_SYSTEM_OR_ROOT                              \
    {                                                       \
        binder::Status status = CheckUidOrRoot(AID_SYSTEM); \
        if (!status.isOk()) {                               \
            return status;                                  \
        }                                                   \
    }

namespace android::vold {

status_t VendorVoldNativeService::try_start() {
    auto service_name = String16("android.system.vold.IVold/default");
    if (!defaultServiceManager()->isDeclared(service_name)) {
        LOG(DEBUG) << "Service for VendorVoldNativeService (" << service_name << ") not declared.";
        return OK;
    }
    return defaultServiceManager()->addService(std::move(service_name),
                                               new VendorVoldNativeService());
}

binder::Status VendorVoldNativeService::registerCheckpointListener(
        const sp<android::system::vold::IVoldCheckpointListener>& listener,
        android::system::vold::CheckpointingState* _aidl_return) {
    ENFORCE_SYSTEM_OR_ROOT;

    bool possible_checkpointing = cp_registerCheckpointListener(listener);
    *_aidl_return = possible_checkpointing
                            ? android::system::vold::CheckpointingState::POSSIBLE_CHECKPOINTING
                            : android::system::vold::CheckpointingState::CHECKPOINTING_COMPLETE;
    return binder::Status::ok();
}

}  // namespace android::vold
