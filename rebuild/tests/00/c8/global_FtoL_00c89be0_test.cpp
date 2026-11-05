// Behaviour test for 0x00C89BE0 FtoL(double) __stdcall: fld qword + fistp dword, current FPU rounding mode.
#if !defined(_MSC_VER) && !defined(FABLE_PORTABLE)
#define FABLE_PORTABLE   // a non-MSVC build takes the helpers' plain C++ twins
#endif
#include "libvorbis/vorbis_ftoi.h"
#include <cstdio>
#ifdef _MSC_VER
static long __stdcall FtoL(double d) 
{
    long result;
    VORBIS_FTOI(result, d);
    return result;
}
#else
static long FtoL(double d) { return fable_portable::vorbis_ftoi(d); }
#endif
int main()
{
    if (FtoL(1.4) != 1 || FtoL(1.6) != 2 || FtoL(2.5) != 2 || FtoL(3.5) != 4 || FtoL(-1.6) != -2
        || FtoL(-2.5) != -2 || FtoL(2000000000.4) != 2000000000L || FtoL(0.0) != 0) { std::printf("00c89be0_TEST FAIL\n"); return 1; }
    std::printf("00c89be0_TEST PASS\n");
    return 0;
}
