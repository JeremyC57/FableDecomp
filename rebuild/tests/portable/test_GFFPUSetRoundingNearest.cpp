// test_GFFPUSetRoundingNearest.cpp -- dual-path test of engine/GFFPUSetRoundingNearest.h (the x87 rounding-control switch).
// From each starting rounding mode (toward zero, down, up, nearest) the build's path must leave the FPU rounding to nearest.
// MSVC builds check it on the x87 itself: GFFloatToLongNear (engine/GFFloatToLongNear.h, a bare `fistp` under the current RC)
// must give 3 / -3 / 2 / -2 for 2.7 / -2.7 / 2.5 / -2.5 (only round-to-nearest-even gives all four); the C runtime's
// _controlfp(0, 0) read-back is not used because it merges the SSE MXCSR, which this x87-only helper leaves alone. The g++
// builds (portable twin) check fegetround(). In the dual-path build the asm path and fable_portable::GFFPUSetRoundingNearest
// are both checked from every starting mode. Build modes as the other tests (tools/decomp_pipeline/portable_twins_check.py).
#include <stdio.h>
#include "engine/GFFPUSetRoundingNearest.h"
#if defined(_MSC_VER)
#include <float.h>
#include "engine/GFFloatToLongNear.h"
static const unsigned int kModes[4] = { _RC_CHOP, _RC_DOWN, _RC_UP, _RC_NEAR };
static void SetMode(unsigned int mode) { _controlfp(mode, _MCW_RC); }
static bool IsNearest()
{
    return GFFloatToLongNear(2.7f) == 3 && GFFloatToLongNear(-2.7f) == -3 && GFFloatToLongNear(2.5f) == 2
        && GFFloatToLongNear(-2.5f) == -2;
}
#else
#include <fenv.h>
static const int kModes[4] = { FE_TOWARDZERO, FE_DOWNWARD, FE_UPWARD, FE_TONEAREST };
static void SetMode(int mode) { fesetround(mode); }
static bool IsNearest() { return fegetround() == FE_TONEAREST; }
#endif

int main()
{
    int bad = 0;
    for (int i = 0; i < 4; ++i) {
        SetMode(kModes[i]);
        GFFPUSetRoundingNearest();
        if (!IsNearest())
            ++bad;
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
        SetMode(kModes[i]);
        fable_portable::GFFPUSetRoundingNearest();
        if (!IsNearest())
            ++bad;
#endif
    }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_GFFPUSetRoundingNearest %s: %s (4 starting modes, %d failures)\n", mode, bad ? "FAIL" : "PASS", bad);
    return bad ? 1 : 0;
}
