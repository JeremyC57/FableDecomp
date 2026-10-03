/* POSIX build: the C runtime is the system one; only Microsoft-specific CRT names here. */
#ifndef _INC_CORECRT
#define _INC_CORECRT
#include <_mingw.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <wchar.h>
#define _CRT_PACKING 8
#define _CRTNOALIAS
#define _CRTRESTRICT
#define _SIZE_T_DEFINED
#define _SSIZE_T_DEFINED
#define _RSIZE_T_DEFINED
#define _INTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
#define _PTRDIFF_T_DEFINED
#define _WCHAR_T_DEFINED
#define _WCTYPE_T_DEFINED
#define _ERRCODE_DEFINED
#define _TIME32_T_DEFINED
#define _TIME64_T_DEFINED
#define _TIME_T_DEFINED
typedef size_t rsize_t;
typedef int errno_t;
typedef int errcode;
typedef int32_t __time32_t;
typedef int64_t __time64_t;
#endif
#include "w32crt.h"
