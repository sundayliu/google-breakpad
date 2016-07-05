////////////////////////////////////////////////////////////////////////////////
//
// file: platform_config.h
// brief: platform config
// date: 2016.07.04
// author:sundayliu
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

// 平台定义

#define TT_PLATFORM_UNKNOWN     0
#define TT_PLATFORM_IOS         1
#define TT_PLATFORM_ANDROID     2
#define TT_PLATFORM_WIN32       3
#define TT_PLATFORM_MAC         4

#define TT_TARGET_PLATFORM      TT_PLATFORM_UNKNOWN

#if defined(__APPLE__)
#include <TargetConditionals.h>
#   if TARGET_OS_IPHONE
#       undef TT_TARGET_PLATFORM
#       define TT_TARGET_PLATFORM   TT_PLATFORM_IOS
#   else
#       undef TT_TARGET_PLATFORM
#       define TT_TARGET_PLATFORM   TT_PLATFORM_MAC
#   endif
#endif

#if defined(ANDROID)||defined(__ANDROID__)
#   undef TT_TARGET_PLATFORM
#   define TT_TARGET_PLATFORM   TT_PLATFORM_ANDROID
#endif

//#if defined(WIN32)||defined(_WINDOWS)
//#   undef TT_TARGET_PLATFORM
//#   define TT_TARGET_PLATFORM   TT_PLATFORM_WIN32
//#endif

#if !TT_TARGET_PLATFORM
#   error "Cannot recognize the target platform;are you targeting an unsupported platform?"
#endif

#if (TT_TARGET_PLATFORM == TT_PLATFORM_WIN32)
#ifndef __MINGW32__
#pragma warning(disable:4127)
#endif
#endif // TT_PLATFORM_WIN32

// ABI 定义
#define TT_ABI_UNKNOWN  0
#define TT_ABI_ARM      1
#define TT_ABI_ARM_64   2
#define TT_ABI_X86      3
#define TT_ABI_X86_64   4

#define TT_TARGET_ABI TT_ABI_UNKNOWN

#if defined(__arm__)
#undef TT_TARGET_ABI
#define TT_TARGET_ABI TT_ABI_ARM
#endif

#if defined(__aarch64__)||defined(__arm64__)
#undef TT_TARGET_ABI
#define TT_TARGET_ABI TT_ABI_ARM_64
#endif

#if defined(__i386__)||defined(i386)||defined(__i386)
#undef TT_TARGET_ABI
#define TT_TARGET_ABI TT_ABI_X86
#endif

#if defined(__x86_64__)||defined(__x86_64)
#undef TT_TARGET_ABI
#define TT_TARGET_ABI TT_ABI_X86_64
#endif

#if !TT_TARGET_ABI
#   error "Cannot recognize the target ABI;are you targeting an unsupported ABI?"
#endif

#endif