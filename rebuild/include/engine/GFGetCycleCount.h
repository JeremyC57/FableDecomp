// GFGetCycleCount() -- read the CPU time-stamp counter (64 bits, edx:eax) with `rdtsc`: the engine's inline-assembly cycle
// counter, expanded in place by its callers (the fast sub-frame timer and the CPU-ahead counter callback).
// Identity: FableWin.pdb ?GFGetCycleCount@@YI_JXZ (__fastcall, signed __int64 result; out of line only in the debug build:
//   28 bytes / 13 instructions = `push ebp ; mov ebp,esp ; sub esp,8 ; xor eax,eax ; xor edx,edx ; rdtsc ;
//   mov [ebp-8],eax ; mov [ebp-4],edx ; mov eax,[ebp-8] ; mov edx,[ebp-4] ; mov esp,ebp ; pop ebp ; ret`, exactly this
//   block in a debug frame).
// Retail-asm-helper: GFGetCycleCount, GF_GET_CYCLE_COUNT
// Retail-asm: 0x009E1C73 `xor eax,eax` / 0x009E1C75 `xor edx,edx` / 0x009E1C77 `rdtsc` + the two dword stores to the 8-byte
//   local (GFGetFastSubFrameTimer 0x009E1C70), the same block at 0x009E24D5 (GFRecalibrateFastSubFrameTimer) and 0x009E2543
//   (CEngineDebug::CPUAheadCounterCallback). VC7.1 (13.10.3077 / 4035) has no rdtsc intrinsic (__rdtsc / <intrin.h> arrived
//   with VC8), so the original read the counter with inline asm.
// Macro form (2026-11-05 asm migration; call it on a line of its own, an `__asm` statement runs to the end of its line): GF_GET_CYCLE_COUNT(counter) stores the counter into the caller's own __int64 local
//   (no return-value copy), as GFGetFastSubFrameTimer 0x009E1C70 does.
// Semantics: the processor's time-stamp counter. The engine only ever uses differences, scaled by g_CyclesPerSecond, which
//   GFRecalibrateFastSubFrameTimer measures against GFGetTime -- any monotonic counter with a steady rate works.
// Portable twin (FABLE_PORTABLE): Strategy C -- no plain C++ reads the TSC: the compiler intrinsic __rdtsc() (GCC / Clang
//   <x86intrin.h>, MSVC >= VC8 <intrin.h>) on x86; std::clock_gettime(CLOCK_MONOTONIC) nanoseconds elsewhere (the
//   engine's own calibration absorbs the rate). VC7.1 has neither an intrinsic nor another way to read the TSC, so its
//   portable build keeps the retail block. Tests: rebuild/tests/portable/test_GFGetCycleCount.cpp (monotonic, both paths
//   read the same counter).
#ifndef FABLE_ENGINE_GFGETCYCLECOUNT_H
#define FABLE_ENGINE_GFGETCYCLECOUNT_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#if defined(_MSC_VER) && _MSC_VER >= 1400
#include <intrin.h>
#elif !defined(_MSC_VER) && (defined(__i386__) || defined(__x86_64__))
#include <x86intrin.h>
#elif !defined(_MSC_VER)
#include <time.h>
#endif
#ifndef _MSC_VER
#include <stdint.h>
#endif
namespace fable_portable
{
#ifdef _MSC_VER
    typedef __int64 CycleCount;
#else
    typedef int64_t CycleCount;                 // <stdint.h>: C99 / every non-MSVC C++03 host
#endif
    inline CycleCount GFGetCycleCount()
    {
#if defined(_MSC_VER) && _MSC_VER >= 1400
        return (CycleCount)__rdtsc();
#elif defined(_MSC_VER)
        CycleCount counter;                     // VC7.1: no intrinsic, no other route to the TSC (see above)
        __asm {
            xor eax, eax
            xor edx, edx
            rdtsc
            mov dword ptr counter, eax
            mov dword ptr counter+4, edx
        }
        return counter;
#elif defined(__i386__) || defined(__x86_64__)
        return (CycleCount)__rdtsc();
#else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (CycleCount)ts.tv_sec * 1000000000 + ts.tv_nsec;
#endif
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::GFGetCycleCount;
#define GF_GET_CYCLE_COUNT(counter) ((counter) = fable_portable::GFGetCycleCount())
#else
#define GF_GET_CYCLE_COUNT(counter) \
    __asm xor eax, eax \
    __asm xor edx, edx \
    __asm rdtsc \
    __asm mov dword ptr counter, eax \
    __asm mov dword ptr counter+4, edx
__forceinline __int64 __fastcall GFGetCycleCount()
{
    __int64 counter;
    __asm {
        xor eax, eax
        xor edx, edx
        rdtsc
        mov dword ptr counter, eax
        mov dword ptr counter+4, edx
    }
    return counter;
}
#endif

#endif  // FABLE_ENGINE_GFGETCYCLECOUNT_H
