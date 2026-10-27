#include <cstdio>
#include <math.h>
// behaviour test of MillibelsToAmplitude (0x00c77700): silence at or below -10000 mB, else 10^(mB/2000) through libpk's x87 2^x block
#ifdef _MSC_VER
float __cdecl MillibelsToAmplitude(long millibels)
{
    if (millibels <= -10000)
        return 0.0f;
    float scale; float result; float whole; float log2of10 = 3.32192802f; float exponent = millibels * 0.0005f; float one = 1.0f;
    __asm {
        fld log2of10
        fmul exponent
        frndint
        fst whole
        wait
        fld1
        fscale
        fxch st(1)
        fstp scale
        fld log2of10
        fmul exponent
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
float MillibelsToAmplitude(long millibels)
{
    if (millibels <= -10000)
        return 0.0f;
    return (float)pow(10.0, (double)(millibels * 0.0005f));
}
#endif
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d <= 1e-4f * (b < 0 ? -b : b) + 1e-6f; }
int main()
{
    bool ok = MillibelsToAmplitude(-10000) == 0.0f && MillibelsToAmplitude(-20000) == 0.0f && Near(MillibelsToAmplitude(-9999), 0.0000100115f)
           && Near(MillibelsToAmplitude(0), 1.0f) && Near(MillibelsToAmplitude(-2000), 0.1f) && Near(MillibelsToAmplitude(2000), 10.0f) && Near(MillibelsToAmplitude(-600), 0.501187234f);
    std::printf(ok ? "00c77700_TEST PASS\n" : "00c77700_TEST FAIL\n");
    return ok ? 0 : 1;
}