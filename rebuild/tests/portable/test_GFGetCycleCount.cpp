// test_GFGetCycleCount.cpp -- dual-path test of engine/GFGetCycleCount.h (the time-stamp counter read).
// A counter has no golden value: the test checks that the build's path is monotonic over many reads and, in the dual-path
// build, that the asm path and fable_portable::GFGetCycleCount read the same counter (asm <= twin <= asm, interleaved).
// Build modes as the other tests (tools/decomp_pipeline/portable_twins_check.py).
#include <stdio.h>
#include "engine/GFGetCycleCount.h"

#ifdef _MSC_VER
typedef __int64 fable_cycle_t;
#else
typedef fable_portable::CycleCount fable_cycle_t;
#endif

static fable_cycle_t ByMacro()                          // GF_GET_CYCLE_COUNT on the caller's own local (2026-11-05)
{
    fable_cycle_t counter;
    GF_GET_CYCLE_COUNT(counter);
    return (fable_cycle_t)counter;
}

int main()
{
    int bad = 0;
    for (int i = 0; i < 1000; ++i) {
        fable_cycle_t a = (fable_cycle_t)GFGetCycleCount();
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
        fable_cycle_t b = (fable_cycle_t)fable_portable::GFGetCycleCount();
#else
        fable_cycle_t b = a;
#endif
        fable_cycle_t m = ByMacro();
        fable_cycle_t c = (fable_cycle_t)GFGetCycleCount();
        if (!(a <= b && b <= m && m <= c))
            ++bad;
    }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_GFGetCycleCount %s: %s (1000 interleaved reads, %d out of order)\n", mode, bad ? "FAIL" : "PASS", bad);
    return bad ? 1 : 0;
}
