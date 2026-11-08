// ReadTimeStampCounter() -- libpk (the GameCODA audio middleware): read the CPU time-stamp counter (64 bits, edx:eax) with a
// bare `rdtsc` into an 8-byte local, expanded in place by libpk's CPU-speed code (no PDB: a descriptive name; distinct from the
// engine's GFGetCycleCount, which clears eax/edx first).
// Retail-asm-helper: ReadTimeStampCounter, LIBPK_READ_TIME_STAMP_COUNTER
// Retail-asm: 0x00C52A83 `rdtsc` + 0x00C52A85 `mov [esp],eax` / 0x00C52A88 `mov [esp+4],edx` (the out-of-line counter read
//   0x00C52A80), 0x00C52D17 / 0x00C52D30 `rdtsc` with the same two stores (MeasureCpuFrequency 0x00C52CE0, twice). VC7.1
//   (13.10.3077) has no rdtsc intrinsic (__rdtsc / <intrin.h> arrived with VC8; no header of $VC71/include declares one), so
//   the original read the counter with inline asm.
// Forms: the __forceinline function (MeasureCpuFrequency expands it, the result copied to the caller's local) and the macro
//   LIBPK_READ_TIME_STAMP_COUNTER(counter) on the caller's own unsigned __int64 (the out-of-line 0x00C52A80 body).
// Semantics: the processor's time-stamp counter; libpk only uses differences against QueryPerformanceCounter.
// Portable twin (FABLE_PORTABLE): Strategy C, as engine/GFGetCycleCount.h: __rdtsc() (GCC / Clang <x86intrin.h>, MSVC >= VC8
//   <intrin.h>) on x86, clock_gettime(CLOCK_MONOTONIC) nanoseconds elsewhere; VC7.1 has no other route to the TSC, so its
//   portable build reuses engine/GFGetCycleCount.h's. Test: rebuild/tests/portable/test_ReadTimeStampCounter.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_LIBPK_READTIMESTAMPCOUNTER_H
#define FABLE_LIBPK_READTIMESTAMPCOUNTER_H
#include "fable_compiler.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#include "engine/GFGetCycleCount.h"
namespace fable_portable
{
    inline CycleCount ReadTimeStampCounter()
    {
        return GFGetCycleCount();
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::ReadTimeStampCounter;
#define LIBPK_READ_TIME_STAMP_COUNTER(counter) ((counter) = fable_portable::ReadTimeStampCounter())
#else
#define LIBPK_READ_TIME_STAMP_COUNTER(counter) \
    __asm rdtsc \
    __asm mov dword ptr counter, eax \
    __asm mov dword ptr counter+4, edx
FABLE_FORCEINLINE unsigned __int64 ReadTimeStampCounter()
{
    unsigned __int64 counter;
    LIBPK_READ_TIME_STAMP_COUNTER(counter);
    return counter;
}
#endif

#endif  // FABLE_LIBPK_READTIMESTAMPCOUNTER_H
