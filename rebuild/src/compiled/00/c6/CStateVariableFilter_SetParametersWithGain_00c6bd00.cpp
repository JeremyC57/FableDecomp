// CStateVariableFilter::SetParametersWithGain(const SGainFilterParameters* parameters, float scale) -- set the target cutoff (SetCutoff), the gain coefficient ((gain - 1) * 1.1, or -1.001 below a gain of 0.09) and Q; while the filter state is silent the current coefficients snap to the targets
// Leaf: ?SetParametersWithGain@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BD00, 155 bytes, `ret 8`.
// Module: libpk's software mixer DSP (0x00C6Bxxx; no PDB/RTTI, no trace strings). The object is a Chamberlin state-variable
//   filter: the per-sample loop of 0x00C6BAE0 / 0x00C6B8C0 runs band += f * (in - low - q * band), low += f * band with
//   the coefficients at +0x00/+0x04 ramped toward the targets at +0x0c/+0x10 and snapped to them at the end of the ramp
//   (+0x20 the ramp position). The voice set-up 0x00C5CA60 (and 0x00C54C10) calls 0x00C6BC80 / 0x00C6BD00 for the filters at voice
//   +0xdc / +0x100 from a descriptor's two parameter records (flags 0x20000 / 0x40000, 0x80000 / 0x100000).
// Layout: local model (no PDB class), member names descriptive.
// Callees: CStateVariableFilter::SetCutoff 0x00C6BBD0 (same TU).
// Shape: as SetParameters 0x00C6BC80 (same-TU noinline SetCutoff keeps ecx/edx); the gain is read into a named float
//   local so it stays in st0 across the `< 0.09f` test (`fcom` ; `fstp st0` on the constant arm, an immediate
//   -1.001f store); the other arm is `(gain - 1.0f) * 1.1f`.
#include <math.h>
#pragma intrinsic(sin)
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

#pragma auto_inline(off)
__declspec(noinline) void CStateVariableFilter::SetCutoff(unsigned long frequency, float scale)
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
#pragma auto_inline(on)

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
}