// test_GFFPUSetDoublePrecision.cpp -- dual-path test of engine/GFFPUSetDoublePrecision.h (the x87 precision-control switch).
// From each starting precision (24, 53, 64 bits), with a non-default rounding mode set, the build's path must leave the x87 at
// 53-bit precision and must not touch the rounding control. MSVC builds read the x87 word back with _controlfp(0, 0) (its PC and
// RC fields are the x87's; the SSE MXCSR has no precision field). 32-bit glibc builds read <fpu_control.h>. Other g++ targets
// (x86-64: SSE arithmetic) have no x87 precision to set, so the portable twin is a no-op there and only the call is checked.
// In the dual-path build the asm path and fable_portable::GFFPUSetDoublePrecision are both checked from every starting state.
// Build modes as the other tests (tools/decomp_pipeline/portable_twins_check.py).
#include <stdio.h>
#include "engine/GFFPUSetDoublePrecision.h"
#if defined(_MSC_VER)
#include <float.h>
static const unsigned int kPrec[3] = { _PC_24, _PC_53, _PC_64 };
static void SetState(unsigned int prec) { _controlfp(prec, _MCW_PC); _controlfp(_RC_UP, _MCW_RC); }
static bool IsDoubleKeepingRounding()
{
    unsigned int cw = _controlfp(0, 0);
    return (cw & _MCW_PC) == _PC_53 && (cw & _MCW_RC) == _RC_UP;
}
static void Restore() { _controlfp(_RC_NEAR, _MCW_RC); }
#elif defined(__GLIBC__) && defined(__i386__)
#include <fpu_control.h>
static const fpu_control_t kPrec[3] = { _FPU_SINGLE, _FPU_DOUBLE, _FPU_EXTENDED };
static void SetState(fpu_control_t prec)
{
    fpu_control_t cw; _FPU_GETCW(cw);
    cw = (fpu_control_t)((cw & ~0x0F00) | prec | _FPU_RC_UP); _FPU_SETCW(cw);
}
static bool IsDoubleKeepingRounding()
{
    fpu_control_t cw; _FPU_GETCW(cw);
    return (cw & 0x0300) == _FPU_DOUBLE && (cw & 0x0C00) == _FPU_RC_UP;
}
static void Restore() { fpu_control_t cw; _FPU_GETCW(cw); cw = (fpu_control_t)(cw & ~0x0C00); _FPU_SETCW(cw); }
#else
static const int kPrec[3] = { 0, 0, 0 };
static void SetState(int) {}
static bool IsDoubleKeepingRounding() { return true; }
static void Restore() {}
#endif

int main()
{
    int bad = 0;
    for (int i = 0; i < 3; ++i) {
        SetState(kPrec[i]);
        GFFPUSetDoublePrecision();
        if (!IsDoubleKeepingRounding())
            ++bad;
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
        SetState(kPrec[i]);
        fable_portable::GFFPUSetDoublePrecision();
        if (!IsDoubleKeepingRounding())
            ++bad;
#endif
        Restore();
    }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_GFFPUSetDoublePrecision %s: %s (3 starting precisions, %d failures)\n", mode, bad ? "FAIL" : "PASS", bad);
    return bad ? 1 : 0;
}
