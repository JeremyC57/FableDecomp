#include <cstdio>
#include <math.h>
struct SFilterParameters {                // descriptor record (voice set-up passes descriptor +0x24 / +0x110)
    unsigned long Frequency;             // +0x00 Hz
    float         Q;                     // +0x04
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
    void SetParameters(const SFilterParameters* parameters, float scale);
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

void CStateVariableFilter::SetParameters(const SFilterParameters* parameters, float scale)
{
    SetCutoff(parameters->Frequency, scale);
    SetQ(parameters->Q);
    if (0.0f == m_band && 0.0f == m_low) {
        m_f = m_targetF;
        m_damping = m_targetDamping;
        m_gain = m_targetGain;
    }
}static int near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
static int fail() { std::printf("00c6bc80_TEST FAIL\n"); return 1; }
int main()
{
    CStateVariableFilter f = { 0, 0, 0, 0, 0.5f, 0.25f, 0, 0, 0 };
    SFilterParameters p = { 1000, 2.0f };
    f.SetParameters(&p, 1.0f);
    if (!near(f.m_targetF, 0.142365f) || f.m_targetDamping != 0.5f) return fail();
    if (f.m_f != f.m_targetF || f.m_damping != 0.5f || f.m_gain != 0.25f) return fail();   // silent: snapped
    f.m_band = 0.1f;                             // running: targets only
    p.Frequency = 9000; p.Q = 0.5f;              // damping 2 is unstable at f = 1: limited to 1/f
    f.SetParameters(&p, 1.0f);
    if (f.m_targetF != 1.0f || f.m_targetDamping != 1.0f) return fail();
    if (f.m_damping != 0.5f || !near(f.m_f, 0.142365f)) return fail();
    std::printf("00c6bc80_TEST PASS\n");
    return 0;
}