#include <cstdio>
#include <math.h>
struct SGainFilterParameters {            // descriptor record (voice set-up passes descriptor +0x28 / +0x114)
    unsigned long Frequency;             // +0x00 Hz
    float         Gain;                  // +0x04 linear
    float         Q;                     // +0x08
};
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
    void SetQ(float q)
    {
        m_targetDamping = 1.0f / q;
        if (m_targetF * m_targetDamping > 1.0f)
            m_targetDamping = 1.0f / m_targetF;
    }
    void SetParametersWithGain(const SGainFilterParameters* parameters, float scale);
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
}

void CStateVariableFilter::SetParametersWithGain(const SGainFilterParameters* parameters, float scale)
{
    SetCutoff(parameters->Frequency, scale);
    float gain = parameters->Gain;
    if (gain < 0.09f)
        m_targetGain = -1.001f;
    else
        m_targetGain = (gain - 1.0f) * 1.1f;
    SetQ(parameters->Q);
    if (0.0f == m_band && 0.0f == m_low) {
        m_f = m_targetF;
        m_damping = m_targetDamping;
        m_gain = m_targetGain;
    }
}static int near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
static int fail() { std::printf("00c6bd00_TEST FAIL\n"); return 1; }
int main()
{
    CStateVariableFilter f = { 0, 0, 0, 0, 0.5f, 0, 0, 0, 0 };
    SGainFilterParameters p = { 1000, 2.0f, 4.0f };
    f.SetParametersWithGain(&p, 1.0f);
    if (!near(f.m_targetGain, 1.1f) || f.m_targetDamping != 0.25f || !near(f.m_targetF, 0.142365f)) return fail();
    if (f.m_gain != f.m_targetGain || f.m_damping != 0.25f) return fail();
    f.m_low = -0.2f;
    p.Gain = 0.05f;                              // below 0.09: the floor coefficient
    f.SetParametersWithGain(&p, 1.0f);
    if (!near(f.m_targetGain, -1.001f) || !near(f.m_gain, 1.1f)) return fail();
    p.Gain = 1.0f;
    f.SetParametersWithGain(&p, 1.0f);
    if (!near(f.m_targetGain, 0.0f)) return fail();
    std::printf("00c6bd00_TEST PASS\n");
    return 0;
}