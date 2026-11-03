// prelude_win32_types.h -- the Win32 types a vendor helper header expects from <windows.h> (strmbase/*.h), for the
// stand-alone header compile of tools/decomp_pipeline/portable_twins_check.py.
#ifndef FABLE_PRELUDE_WIN32_TYPES_H
#define FABLE_PRELUDE_WIN32_TYPES_H
#ifdef _MSC_VER
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef unsigned __int64 ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    struct { DWORD LowPart; DWORD HighPart; } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;
#define WINAPI __stdcall
#else
#include <stdint.h>
typedef uint32_t ULONG;
typedef uint64_t ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { ULONG LowPart; ULONG HighPart; } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;
#define WINAPI
#endif
#endif
