// Behaviour test for 0x00C6B9D0 CStateVariableFilter::FilterWithGain (the row body below is the landed text; FtoL comes
// from the shared libpk/FtoL.h: inline asm under VC7.1, its plain C++ twin elsewhere). Self-contained doubles: the mix
// block length and FilterRampedWithGain (records its call and marks the samples it was given).
#if !defined(_MSC_VER) && !defined(FABLE_PORTABLE)
#define FABLE_PORTABLE
#endif
#include <stdio.h>
#include <string.h>
#include <math.h>
// ---- row body ----
#include "fable_compiler.h"
#include "libpk/FtoL.h"
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
FABLE_FORCEINLINE long FloatToLongTruncate(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
extern unsigned long g_mixBlockLength;    // 0x0139C9F0 (128 samples)
struct SFilterCoefficients {
    float         f;                     // frequency coefficient
    float         damping;               // damping (1/Q)
    float         gain;                  // gain coefficient
};
struct CStateVariableFilter {
    SFilterCoefficients m_current;       // +0x00
    SFilterCoefficients m_target;        // +0x0c
    float         m_band;                // +0x18 filter state
    float         m_low;                 // +0x1c filter state
    unsigned long m_rampPosition;        // +0x20 samples into the coefficient ramp
    void FilterRampedWithGain(short* samples, unsigned long count);   // 0x00C6B7C0
    void FilterWithGain(short* samples, unsigned long count);
};
void CStateVariableFilter::FilterWithGain(short* samples, unsigned long count)
{
    unsigned long i = 0;
    if (m_current.f != m_target.f || m_current.gain != m_target.gain || m_current.damping != m_target.damping) {
        unsigned long ramp = g_mixBlockLength - m_rampPosition;
        if (ramp > count)
            ramp = count;
        FilterRampedWithGain(samples, ramp);
        m_rampPosition += ramp;
        i = ramp;
        if (m_rampPosition == g_mixBlockLength) {
            m_current = m_target;
            m_rampPosition = 0;
        }
    }
    for (; i < count; ++i) {
        float in = (float)samples[i];
        float high = in - m_low - m_current.damping * m_band;
        m_band += high * m_current.f;
        m_low += m_band * m_current.f;
        long out = FloatToLongTruncate(in + m_current.gain * m_current.damping * high);
        if (out > 0x7fff)
            out = 0x7fff;
        else if (out < -0x8000)
            out = -0x8000;
        samples[i] = (short)out;
    }
}


// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default a bare fistp uses), then the truncation step
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static void ref_svf(float f, float damping, float gain, float* band, float* low, const short* in, short* out, int n)
{
    for (int i = 0; i < n; ++i) {
        float x = (float)in[i];
        float high = x - *low - damping * *band;
        *band = *band + high * f;
        *low = *low + *band * f;
        long o = ref_trunc(x + gain * damping * high);
        out[i] = (short)(o > 0x7fff ? 0x7fff : (o < -0x8000 ? -0x8000 : o));
    }
}
unsigned long g_mixBlockLength = 128;
static int g_ramped = 0;
void CStateVariableFilter::FilterRampedWithGain(short* samples, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        samples[i] = 7;
    g_ramped += (int)count;
}
int main()
{
    CStateVariableFilter s; memset(&s, 0, sizeof(s));
    short in[10] = { 100, 100, -50, 0, 300, 7, 9, 11, 1000, -1000 }, a[10], b[10];
    // steady coefficients: no ramp call, every sample filtered with the current f / damping / gain
    memcpy(a, in, sizeof(a));
    s.m_current.f = s.m_target.f = 0.5f; s.m_current.damping = s.m_target.damping = 0.25f; s.m_current.gain = s.m_target.gain = 2.0f;
    s.FilterWithGain(a, 6);
    float band = 0, low = 0; ref_svf(0.5f, 0.25f, 2.0f, &band, &low, in, b, 6);
    check(memcmp(a, b, 6 * sizeof(short)) == 0 && g_ramped == 0, "steady coefficients");
    check(s.m_band == band && s.m_low == low && a[6] == 9, "state carried, tail untouched");
    // hand value: f 0.5, damping 1, gain 2, in 1000 -> high 1000, band 500, low 250, out 1000 + 2*1*1000 = 3000
    CStateVariableFilter h; memset(&h, 0, sizeof(h));
    h.m_current.f = h.m_target.f = 0.5f; h.m_current.damping = h.m_target.damping = 1.0f; h.m_current.gain = h.m_target.gain = 2.0f;
    short one[1] = { 1000 };
    h.FilterWithGain(one, 1);
    check(one[0] == 3000 && h.m_band == 500.0f && h.m_low == 250.0f, "hand value");
    // clamping both ways
    short big[2] = { 32767, -32768 };
    h.m_band = 0; h.m_low = 0; h.FilterWithGain(big, 1);
    check(big[0] == 32767, "clamp high");
    h.m_band = 0; h.m_low = 0; h.FilterWithGain(big + 1, 1);
    check(big[1] == -32768, "clamp low");
    // a gain-only change starts the ramp: 8 samples to the block end go to FilterRampedWithGain, then the target is committed
    memcpy(a, in, sizeof(a));
    s.m_band = 0; s.m_low = 0; s.m_target.gain = 0.0f; s.m_rampPosition = 120;
    s.FilterWithGain(a, 10);
    check(g_ramped == 8 && a[0] == 7 && a[7] == 7, "ramp to the block end");
    check(s.m_rampPosition == 0 && s.m_current.gain == 0.0f, "target committed");
    band = 0; low = 0; ref_svf(0.5f, 0.25f, 0.0f, &band, &low, in + 8, b, 2);
    // zero gain passes the input; the helper is round(x - 0.5) stepped up when negative, so the exact -1000 (tie to even) comes out -999
    check(a[8] == b[0] && a[9] == b[1] && a[8] == 1000 && a[9] == -999, "tail with the new gain");
    // a ramp that does not reach the block end leaves the targets pending
    g_ramped = 0; s.m_target.damping = 0.5f; s.m_rampPosition = 10;
    memcpy(a, in, sizeof(a));
    s.FilterWithGain(a, 4);
    check(g_ramped == 4 && s.m_rampPosition == 14 && s.m_current.damping == 0.25f && a[3] == 7 && a[4] == 300, "partial ramp");
    if (!g_fail) printf("00c6b9d0_TEST PASS\n");
    return g_fail;
}