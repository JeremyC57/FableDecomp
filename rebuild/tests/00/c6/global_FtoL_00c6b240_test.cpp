// Behaviour test for 0x00C6B240 FtoL(float): fld + fistp dword, current FPU rounding mode (default round-to-nearest-even).
#if !defined(_MSC_VER) && !defined(FABLE_PORTABLE)
#define FABLE_PORTABLE   // a non-MSVC build takes the helpers' plain C++ twins
#endif
#define FABLE_LIBPK_FTOL_MACRO_ONLY
#include "libpk/FtoL.h"
#include <cstdio>
#ifdef _MSC_VER
static long __cdecl FtoL(float f) 
{
    long result;
    LIBPK_FTOL(result, f);
    return result;
}
#else
static long FtoL(float f) { return fable_portable::X87FistpNear((double)f); }
#endif
int main()
{
    if (FtoL(1.4f) != 1 || FtoL(1.6f) != 2 || FtoL(2.5f) != 2 || FtoL(3.5f) != 4 || FtoL(-1.6f) != -2
        || FtoL(-2.5f) != -2 || FtoL(123456.0f) != 123456 || FtoL(0.0f) != 0) { std::printf("00c6b240_TEST FAIL\n"); return 1; }
    std::printf("00c6b240_TEST PASS\n");
    return 0;
}
