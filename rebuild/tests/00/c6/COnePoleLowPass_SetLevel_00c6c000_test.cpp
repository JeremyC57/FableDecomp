#include <cstdio>
#include <math.h>
// behaviour test of COnePoleLowPass::SetLevel (0x00C6C000): Q15 feedback from an attenuation in millibels and a reference frequency; zero resets
#ifdef _MSC_VER
// libpk's inline-assembly truncation helper
__forceinline long FloatToLongTrunc(float value)
{
    long result;
    value = value - 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}
__forceinline float Pow10(float x)
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
#include <math.h>
// the truncation helper: round-to-nearest-even of (x - 0.5), +1 when negative == truncation toward zero for non-half values
inline long FloatToLongTrunc(float value)
{
    double v = (double)(value - 0.5f);
    double r = floor(v + 0.5);
    if (r - v == 0.5 && ((long)r & 1)) r -= 1.0;          // ties to even
    long result = (long)r;
    if (result < 0) ++result;
    return result;
}
inline float Pow10(float x) { return (float)pow(10.0, (double)x); }
#endif
static const float kRadiansPerHz = 0.00014247586659621447f;
struct COnePoleLowPass {
    long m_feedback; long m_input; long m_history;
    void SetLevel(long levelMb, float referenceFrequency)
    {
        if (levelMb == 0) { m_feedback = levelMb; m_history = levelMb; return; }
        float r = Pow10((float)(levelMb + levelMb) * 0.0005f);
        float c = (float)cos(referenceFrequency * kRadiansPerHz);
        float a = ((1.0f - r * c) - (float)sqrt(r * ((1.0f - c) * 2.0f - (1.0f - c * c) * r))) * 32768.0f / (1.0f - r);
        long feedback = FloatToLongTrunc(a);
        m_feedback = feedback;
        m_input = 0x7fff - feedback;
        if (m_input == 0) m_history = 0;
    }
};
int main()
{
    COnePoleLowPass f; f.m_feedback = 11; f.m_input = 22; f.m_history = 33;
    f.SetLevel(0, 1000.0f);
    bool ok = f.m_feedback == 0 && f.m_history == 0 && f.m_input == 22;       // zero: only feedback and history reset
    f.m_history = 33;
    f.SetLevel(-3000, 1000.0f);                                                // -30 dB at 1 kHz: a strong low-pass
    ok = ok && f.m_feedback > 20000 && f.m_feedback < 32767 && f.m_input == 0x7fff - f.m_feedback && f.m_history == 33;
    long strong = f.m_feedback;
    f.SetLevel(-300, 1000.0f);                                                 // -3 dB: a gentler one
    ok = ok && f.m_feedback > 0 && f.m_feedback < strong && f.m_input == 0x7fff - f.m_feedback;
    f.SetLevel(-3000, 8000.0f);                                                // a higher reference frequency: less feedback
    ok = ok && f.m_feedback < strong && f.m_feedback > 0;
    std::printf(ok ? "00c6c000_TEST PASS\n" : "00c6c000_TEST FAIL\n");
    return ok ? 0 : 1;
}