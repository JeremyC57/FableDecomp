/* POSIX build: the MinGW-w64 Windows headers (found after the system headers, via
   -idirafter) in LP64 mode, with 16-bit wchar_t (-fshort-wchar). _WIN32/_WIN64 are set
   only while they are parsed, so system and third-party headers stay in POSIX mode. */
#ifndef FABLE_POSIX_WINDOWS_H
#define FABLE_POSIX_WINDOWS_H
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#if !defined(_WIN32)
#define FABLE_W32_TMP 1
#define _WIN32 1
#define _WIN64 1
#endif
#include_next <windows.h>
#ifdef FABLE_W32_TMP
#undef _WIN32
#undef _WIN64
#undef FABLE_W32_TMP
#endif
#endif
