// Copyright (c) 2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef __SERVICE_LOGGING_H__
#define __SERVICE_LOGGING_H__

#include <PmLogLib.h>


extern PmLogContext getServiceContext();

#define GPIO_LOG_CRITICAL(msgid, kvcount, ...) \
        PmLogCritical(getServiceContext(), msgid, kvcount, ##__VA_ARGS__)

#define GPIO_LOG_ERROR(msgid, kvcount, ...) \
        PmLogError(getServiceContext(), msgid, kvcount, ##__VA_ARGS__)

#define GPIO_LOG_WARNING(msgid, kvcount, ...) \
        PmLogWarning(getServiceContext(), msgid, kvcount, ##__VA_ARGS__)

#define GPIO_LOG_INFO(msgid, kvcount, ...) \
        PmLogInfo(getServiceContext(), msgid, kvcount, ##__VA_ARGS__)

#define GPIO_LOG_DEBUG(...) \
        PmLogDebug(getServiceContext(), ##__VA_ARGS__)

void logKmsg(const char *fmt, ...);
#define GPIO_KMSG_DEBUG_MSG(b, fmt, arg...)   if( (b) ) logKmsg(fmt, ##arg)


#define MSGID_LUNASERVICE   "GPIO_LUNASERVICE"

#endif
