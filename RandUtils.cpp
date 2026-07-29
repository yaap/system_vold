/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "RandUtils.h"

#include <openssl/rand.h>

namespace android {
namespace vold {

status_t ReadRandomBytes(size_t bytes, std::string& out) {
    out.resize(bytes);
    return ReadRandomBytes(bytes, &out[0]);
}

status_t ReadRandomBytes(size_t bytes, char* buf) {
    if (RAND_bytes(reinterpret_cast<uint8_t*>(buf), bytes) != 1) {
        return -EIO;
    }
    return OK;
}

status_t GenerateRandomUuid(std::string& out) {
    status_t res = ReadRandomBytes(16, out);
    if (res == OK) {
        out[6] &= 0x0f; /* clear version        */
        out[6] |= 0x40; /* set to version 4     */
        out[8] &= 0x3f; /* clear variant        */
        out[8] |= 0x80; /* set to IETF variant  */
    }
    return res;
}

}  // namespace vold
}  // namespace android
