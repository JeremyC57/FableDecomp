// Test for 0x00C412D0 CLibpkObject1D8::GetEffectiveGain: factors times amplitude, times the attenuation amplitude when set.
#include <cstdio>
static float Attenuate(int mb) { return mb == -2000 ? 0.1f : 1.0f; }   // stand-in for MillibelsToAmplitude
struct CObj { float m_amplitude, m_gain130, m_gain134; int m_attenuation;
    float GetEffectiveGain() { float gain = m_gain134 * m_gain130 * m_amplitude; if (m_attenuation) return Attenuate(m_attenuation) * gain; return gain; } };
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
int main()
{
    CObj o; o.m_amplitude = 0.5f; o.m_gain130 = 2.0f; o.m_gain134 = 3.0f; o.m_attenuation = 0;
    bool ok = Near(o.GetEffectiveGain(), 3.0f);
    o.m_attenuation = -2000; ok = ok && Near(o.GetEffectiveGain(), 0.3f);
    if (!ok) { std::printf("00c412d0_TEST FAIL\n"); return 1; }
    std::printf("00c412d0_TEST PASS\n");
    return 0;
}