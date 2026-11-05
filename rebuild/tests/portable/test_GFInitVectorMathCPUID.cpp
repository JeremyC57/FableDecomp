// test_GFInitVectorMathCPUID.cpp -- dual-path test of engine/GFInitVectorMathCPUID.h: the asm probe and the twin report
// the same CPUID.1:EDX.SSE bit, a set flag stays set, and the result is stable across calls.
#include <stdio.h>
#include "engine/GFInitVectorMathCPUID.h"
#include "fable_test_mode.h"
static bool g_flag;                                     // a global byte, as g_SSEAvailable
static bool Probe() { g_flag = false;
    GF_CPUID_DETECT_SSE(g_flag);
return g_flag; }
static bool ProbeSet() { g_flag = true;
    GF_CPUID_DETECT_SSE(g_flag);
return g_flag; }
int main()
{
    int bad = 0;
    bool first = Probe();
    if (Probe() != first) ++bad;
    if (!ProbeSet()) ++bad;                             // never clears the flag
#ifdef FABLE_TEST_DUAL
    bool twin = false;
    fable_portable::CpuidDetectSSE(twin);
    if (twin != first) { printf("  asm %d, twin %d\n", (int)first, (int)twin); ++bad; }
#endif
    printf("test_GFInitVectorMathCPUID %s: %s (SSE %s)\n", kFableTestMode, bad ? "FAIL" : "PASS", first ? "yes" : "no");
    return bad ? 1 : 0;
}
