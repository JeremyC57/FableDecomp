/* POSIX build: the MinGW-w64 Windows headers (found after the system headers, via
   -idirafter) in LP64 mode, with 16-bit wchar_t (-fshort-wchar). _WIN32/_WIN64 are set
   only while they are parsed, so system and third-party headers stay in POSIX mode. */
#ifndef FABLE_POSIX_WINDOWS_H
#define FABLE_POSIX_WINDOWS_H
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#if !defined(_WIN32)
/* Let the C++ library configure itself for POSIX before the Windows macros appear. */
#ifdef __cplusplus
#include <cstddef>
#endif
#include <stddef.h>
#define FABLE_W32_TMP 1
#define _WIN32 1
#define _WIN64 1
#endif
#if defined(__aarch64__)
/* On ARM64 MinGW reads the TEB from x18, which on Linux and Android is no TEB: park
   its versions under other names and use the emulated TEB, as on x86_64. */
#define NtCurrentTeb w32_mingw_NtCurrentTeb
#define GetCurrentFiber w32_mingw_GetCurrentFiber
#define GetFiberData w32_mingw_GetFiberData
#endif
#include_next <windows.h>
#if defined(__aarch64__)
#undef NtCurrentTeb
#undef GetCurrentFiber
#undef GetFiberData
#ifdef __cplusplus
extern "C" {
#endif
unsigned long long w32_readgsqword(unsigned long off); /* fake TEB (posix/w32/sync.cpp) */
static inline struct _TEB* NtCurrentTeb(void) { return (struct _TEB*)w32_readgsqword(0x30); }
static inline PVOID GetCurrentFiber(void) { return (PVOID)w32_readgsqword(0x20); }
static inline PVOID GetFiberData(void) { return *(PVOID*)GetCurrentFiber(); }
#ifdef __cplusplus
}
#endif
#endif
#ifdef FABLE_W32_TMP
#undef _WIN32
#undef _WIN64
/* libc++ takes __MINGW32__ to mean the MinGW C runtime (and its locale functions). */
#undef __MINGW32__
#undef __MINGW64__
#undef FABLE_W32_TMP
#endif
#endif
