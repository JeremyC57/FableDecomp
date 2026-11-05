// test_Pow10.cpp -- dual-path test of libpk/Pow10.h (LIBPK_POW10_BLOCK, the x87 2^(x log2 10) block). Inputs: 2,001 exponents
// x = k / 100 for k = -1000..1000 (the mB range libpk feeds it: 10^(-10) .. 10^10), plus 0, +-0.5 ties of the rounded
// exponent and large / small magnitudes. Every mode checks the result against 10^x to a relative 2^-20 (1 + 4|x|) (log2of10 is the float
// 3.32192802f, so the exponent error grows with |x|); the dual build requires the asm block and fable_portable::Pow10Block to agree bit for bit on every input,
// in the whole / scale stores as well.
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "libpk/Pow10.h"
#include "fable_test_mode.h"
static float Block(float x, float* wholeOut, float* scaleOut)
{
    float scale;
    float result;
    float whole;
    float log2of10 = 3.32192802f;
    float one = 1.0f;
    LIBPK_POW10_BLOCK(log2of10, x, whole, scale, one, result);
    *wholeOut = whole;
    *scaleOut = scale;
    return result;
}
static unsigned int Bits(float f) { unsigned int u; memcpy(&u, &f, 4); return u; }
int main()
{
    int bad = 0, n = 0;
    float extra[] = { 0.0f, 0.150515f, -0.150515f, 0.4515450f, -0.4515450f, 30.0f, -30.0f, 37.5f, -37.5f };
    for (int k = -1000; k <= 1000 + (int)(sizeof extra / sizeof extra[0]); ++k, ++n) {
        float x = k <= 1000 ? (float)k / 100.0f : extra[k - 1001];
        float whole, scale;
        float got = Block(x, &whole, &scale);
        double want = pow(10.0, (double)x);
        if (!(fabs(got - want) <= want * (1.0 + 4.0 * fabs((double)x)) / 1048576.0)) { printf("  x=%.9g: %.9g, expected ~%.9g\n", x, got, want); ++bad; }
#ifdef FABLE_TEST_DUAL
        float tw, ts, tr;
        fable_portable::Pow10Block(3.32192802f, x, tw, ts, 1.0f, tr);
        if (Bits(tr) != Bits(got) || Bits(tw) != Bits(whole) || Bits(ts) != Bits(scale)) {
            printf("  x=%.9g: asm %08X (%g %g), twin %08X (%g %g)\n", x, Bits(got), whole, scale, Bits(tr), tw, ts);
            ++bad;
        }
#else
        (void)Bits;
#endif
    }
    printf("test_Pow10 %s: %s (%d inputs)\n", kFableTestMode, bad ? "FAIL" : "PASS", n);
    return bad ? 1 : 0;
}
