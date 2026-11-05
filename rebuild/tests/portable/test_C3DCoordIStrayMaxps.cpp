// test_C3DCoordIStrayMaxps.cpp -- test of engine/C3DCoordIStrayMaxps.h: the stray `maxps xmm0,xmm1` changes nothing a C++
// caller sees (both build paths; the portable twin is empty).
#include <stdio.h>
#include "engine/C3DCoordIStrayMaxps.h"
#include "fable_test_mode.h"
static long Body(long x, float v)
{
    long r = (long)((float)x - v);
    GF_STRAY_MAXPS_XMM0_XMM1();
    return r;
}
int main()
{
    int bad = 0;
    for (long x = -5; x <= 5; ++x)
        if (Body(x, 0.5f) != (long)((float)x - 0.5f)) ++bad;
    printf("test_C3DCoordIStrayMaxps %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
