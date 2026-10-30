// behaviour test for the parked 0x00C6B9D0 CStateVariableFilter::FilterWithGain (copy of the logic; the retail
// round-to-nearest helper is modelled in plain C++ here)
#include <cstdio>
#include <cmath>
static long FloatToLong(float value)                // FPU default rounding: to nearest, ties to even
{
    double r = std::floor((double)value), d = (double)value - r;
    if (d > 0.5 || (d == 0.5 && std::fmod(r, 2.0) != 0.0)) r += 1.0;
    return (long)r;
}
static long FloatToLongTruncate(float value) { long r = FloatToLong(value - 0.5f); if (r < 0) ++r; return r; }
static unsigned long g_mixBlockLength = 4;
struct SFilterCoefficients { float f, damping, gain; };
static int g_rampCalls = 0; static unsigned long g_rampCount = 0;
struct CStateVariableFilter {
    SFilterCoefficients m_current, m_target; float m_band, m_low; unsigned long m_rampPosition;
    void FilterWithGainRamped(short*, unsigned long count) { ++g_rampCalls; g_rampCount = count; }
    void FilterWithGain(short* samples, unsigned long count);
};
void CStateVariableFilter::FilterWithGain(short* samples, unsigned long count)
{
    unsigned long i = 0;
    if (m_current.f != m_target.f || m_current.gain != m_target.gain || m_current.damping != m_target.damping) {
        unsigned long ramp = g_mixBlockLength - m_rampPosition;
        if (ramp > count) ramp = count;
        FilterWithGainRamped(samples, ramp);
        m_rampPosition += ramp;
        i = ramp;
        if (m_rampPosition == g_mixBlockLength) { m_current = m_target; m_rampPosition = 0; }
    }
    for (; i < count; ++i) {
        long in = samples[i];
        float high = ((float)in - m_low) - m_current.damping * m_band;
        m_band = m_current.f * high + m_band;
        m_low = m_current.f * m_band + m_low;
        long out = FloatToLongTruncate(m_current.damping * m_current.gain * high + (float)in);
        if (out > 0x7fff) out = 0x7fff; else if (out < -0x8000) out = -0x8000;
        samples[i] = (short)out;
    }
}
int main()
{
    CStateVariableFilter flt = { { 0.5f, 1.0f, 0.0f }, { 0.5f, 1.0f, 0.0f }, 0.0f, 0.0f, 0 };
    short s[3] = { 100, -100, 32767 };
    flt.FilterWithGain(s, 3);                       // gain 0: the output is trunc(in - 0.5 rounded) (-100 -> -99, 32767 -> 32766: retail ties to even)
    if (s[0] != 100 || s[1] != -99 || s[2] != 32766 || g_rampCalls != 0) { std::printf("00c6b9d0_TEST FAIL\n"); return 1; }
    flt.m_band = flt.m_low = 0.0f; flt.m_current.gain = 1.0f; flt.m_target.gain = 1.0f;
    short t[1] = { 30000 };
    flt.FilterWithGain(t, 1);                       // high = 30000, out = 60000 -> clamped
    if (t[0] != 32767) { std::printf("00c6b9d0_TEST FAIL\n"); return 1; }
    flt.m_target.f = 0.25f;                         // coefficient change: ramp 4 of 6, commit, 2 filtered
    short u[6] = { 0, 0, 0, 0, 0, 0 };
    flt.FilterWithGain(u, 6);
    if (g_rampCalls != 1 || g_rampCount != 4 || flt.m_rampPosition != 0 || flt.m_current.f != 0.25f) { std::printf("00c6b9d0_TEST FAIL\n"); return 1; }
    std::printf("00c6b9d0_TEST PASS\n");
    return 0;
}
