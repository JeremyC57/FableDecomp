#include <cstdio>
#include <math.h>
struct CStateVariableFilter {
    float         m_f;                   // +0x00 current frequency coefficient
    float         m_damping;             // +0x04 current damping (1/Q)
    float         m_gain;                // +0x08 current gain coefficient
    float         m_targetF;             // +0x0c
    float         m_targetDamping;       // +0x10
    float         m_targetGain;          // +0x14
    float         m_band;                // +0x18 filter state
    float         m_low;                 // +0x1c filter state
    unsigned long m_rampPosition;        // +0x20
    void SetCutoff(unsigned long frequency, float scale);
};

void CStateVariableFilter::SetCutoff(unsigned long frequency, float scale)
{
    float cutoff = (float)frequency * scale;
    if (cutoff < 200.0f)
        cutoff = 200.0f;
    else if (cutoff > 8000.0f)
        cutoff = 8000.0f;
    m_targetF = 2.0f * (float)sin(cutoff * 7.1237933e-05f);    // pi / 44100 Hz (float bits 0x38956593)
    if (m_targetF > 1.0f)
        m_targetF = 1.0f;
    if (m_targetF * m_targetDamping > 1.0f) {
        m_targetDamping = 1.0f / m_targetDamping;
        if (m_targetF * m_targetDamping > 1.0f)
            m_targetDamping = 1.0f / m_targetF;
    }
}static int near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
static int fail() { std::printf("00c6bbd0_TEST FAIL\n"); return 1; }
int main()
{
    CStateVariableFilter f = { 0, 0, 0, 0, 0.5f, 0, 0, 0, 0 };
    f.SetCutoff(1000, 1.0f);
    if (!near(f.m_targetF, 0.142365f) || f.m_targetDamping != 0.5f) return fail();
    f.SetCutoff(50, 2.0f);                       // 100 Hz clamps up to 200 Hz
    if (!near(f.m_targetF, 0.028493f)) return fail();
    f.SetCutoff(4000000000UL, 1.0f);             // unsigned conversion, clamps to 8000 Hz, capped at 1
    if (f.m_targetF != 1.0f || f.m_targetDamping != 0.5f) return fail();
    f.m_targetDamping = 2.0f;                    // unstable: inverted
    f.SetCutoff(9000, 1.0f);
    if (f.m_targetF != 1.0f || f.m_targetDamping != 0.5f) return fail();
    std::printf("00c6bbd0_TEST PASS\n");
    return 0;
}