#include <cstdio>
#include <math.h>
// behaviour test of Pow10 (0x00c53b10): libpk's x87 2^(x*log2 10) block, i.e. 10^x in single precision
#ifdef _MSC_VER
float __cdecl Pow10(float x)
{
    float scale; float result; float whole; float log2of10 = 3.32192802f; float one = 1.0f;
    __asm {
        fld log2of10
        fmul x
        frndint
        fst whole
        wait
        fld1
        fscale
        fxch st(1)
        fstp scale
        fld log2of10
        fmul x
        fsub whole
        f2xm1
        fadd one
        fmulp st(1), st
        fstp result
        wait
    }
    return result;
}
#else
float Pow10(float x) { return (float)pow(10.0, (double)x); }   // the block computes 2^(whole) * 2^(frac) of x*log2(10)
#endif
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d <= 1e-4f * (b < 0 ? -b : b) + 1e-6f; }
int main()
{
    bool ok = Near(Pow10(0.0f), 1.0f) && Near(Pow10(1.0f), 10.0f) && Near(Pow10(2.0f), 100.0f)
           && Near(Pow10(-1.0f), 0.1f) && Near(Pow10(0.5f), 3.16227766f) && Near(Pow10(-2.5f), 0.00316227766f);
    std::printf(ok ? "00c53b10_TEST PASS\n" : "00c53b10_TEST FAIL\n");
    return ok ? 0 : 1;
}