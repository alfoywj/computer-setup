//
// Created by redbean on 22. 7. 30.
//

#ifndef VULKAN_TOOLS_PCH_H
#define VULKAN_TOOLS_PCH_H

/*
 * Copyright (c) 2015-2019 The Khronos Group Inc.
 * Copyright (c) 2015-2019 Valve Corporation
 * Copyright (c) 2015-2019 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Chia-I Wu <olv@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 * Author: Ian Elliott <ian@LunarG.com>
 * Author: Ian Elliott <ianelliott@google.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Gwan-gyeong Mun <elongbug@gmail.com>
 * Author: Tony Barbour <tony@LunarG.com>
 * Author: Bill Hollings <bill.hollings@brenwill.com>
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <signal.h>
#if defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR)
#include <X11/Xutil.h>
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
#include <linux/input.h>
#include "xdg-shell-client-header.h"
#include "xdg-decoration-client-header.h"
#endif

#ifdef _WIN32
#ifdef _MSC_VER
#pragma comment(linker, "/subsystem:windows")
#endif  // MSVC
#define APP_NAME_STR_LEN 80
#endif  // _WIN32

#ifdef ANDROID
#include "vulkan_wrapper.h"
#else
#include <vulkan/vulkan.h>
#endif

#include <vulkan/vk_sdk_platform.h>
#include "linmath.h"
#include "object_type_string_helper.h"

#include "gettime.h"
#include "inttypes.h"
#define MILLION 1000000L
#define BILLION 1000000000L

#define DEMO_TEXTURE_COUNT 1
#define APP_SHORT_NAME "vkcube"
#define APP_LONG_NAME "Vulkan Cube"

// Allow a maximum of two outstanding presentation operations.
#define FRAME_LAG 2

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#ifdef _WIN32
bool in_callback = false;
#define ERR_EXIT(err_msg, err_class)                                             \
    do {                                                                         \
        if (!demo->suppress_popups) MessageBox(NULL, err_msg, err_class, MB_OK); \
        exit(1);                                                                 \
    } while (0)
void DbgMsg(char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vprintf(fmt, va);
    va_end(va);
    fflush(stdout);
}

#elif defined __ANDROID__
#include <android/log.h>
#define ERR_EXIT(err_msg, err_class)                                           \
    do {                                                                       \
        ((void)__android_log_print(ANDROID_LOG_INFO, "Vulkan Cube", err_msg)); \
        exit(1);                                                               \
    } while (0)
#ifdef VARARGS_WORKS_ON_ANDROID
void DbgMsg(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    __android_log_print(ANDROID_LOG_INFO, "Vulkan Cube", fmt, va);
    va_end(va);
}
#else  // VARARGS_WORKS_ON_ANDROID
#define DbgMsg(fmt, ...)                                                                  \
    do {                                                                                  \
        ((void)__android_log_print(ANDROID_LOG_INFO, "Vulkan Cube", fmt, ##__VA_ARGS__)); \
    } while (0)
#endif  // VARARGS_WORKS_ON_ANDROID
#else
#define ERR_EXIT(err_msg, err_class) \
    do {                             \
        printf("%s\n", err_msg);     \
        fflush(stdout);              \
        exit(1);                     \
    } while (0)
void DbgMsg(char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vprintf(fmt, va);
    va_end(va);
    fflush(stdout);
}
#endif

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                                                              \
    {                                                                                                         \
        demo->fp##entrypoint = (PFN_vk##entrypoint)vkGetInstanceProcAddr(inst, "vk" #entrypoint);             \
        if (demo->fp##entrypoint == NULL) {                                                                   \
            ERR_EXIT("vkGetInstanceProcAddr failed to find vk" #entrypoint, "vkGetInstanceProcAddr Failure"); \
        }                                                                                                     \
    }

static PFN_vkGetDeviceProcAddr g_gdpa = NULL;

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                                                                    \
    {                                                                                                            \
        if (!g_gdpa) g_gdpa = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(demo->inst, "vkGetDeviceProcAddr"); \
        demo->fp##entrypoint = (PFN_vk##entrypoint)g_gdpa(dev, "vk" #entrypoint);                                \
        if (demo->fp##entrypoint == NULL) {                                                                      \
            ERR_EXIT("vkGetDeviceProcAddr failed to find vk" #entrypoint, "vkGetDeviceProcAddr Failure");        \
        }                                                                                                        \
    }

#endif //VULKAN_TOOLS_PCH_H

#include <array>
#include <string>
#include <vector>
#include "ArcCamera.h"