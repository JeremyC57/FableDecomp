// CStateVariableFilter::SetParameters(const SFilterParameters* parameters, float scale) -- set the target cutoff (SetCutoff) and Q (damping 1/Q, limited to 1/f for stability); while the filter state is silent (band == low == 0) the current coefficients snap to the targets
// Leaf: ?SetParameters@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BC80, 113 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer DSP (0x00C6Bxxx; no PDB/RTTI, no trace strings). The object is a Chamberlin state-variable
//   filter: the per-sample loop of 0x00C6BAE0 / 0x00C6B8C0 runs band += f * (in - low - q * band), low += f * band with
//   the coefficients at +0x00/+0x04 ramped toward the targets at +0x0c/+0x10 and snapped to them at the end of the ramp
//   (+0x20 the ramp position). The voice set-up 0x00C5CA60 (and 0x00C54C10) calls 0x00C6BC80 / 0x00C6BD00 for the filters at voice
//   +0xdc / +0x100 from a descriptor's two parameter records (flags 0x20000 / 0x40000, 0x80000 / 0x100000).
// Layout: local model (no PDB class), member names descriptive.
// Callees: CStateVariableFilter::SetCutoff 0x00C6BBD0 (same TU).
// Shape: retail keeps `this` (ecx) and `parameters` (edx) live across the SetCutoff call, so SetCutoff's body is a
//   same-TU `__declspec(noinline)` stand-in under `#pragma auto_inline(off)`; SetQ is inline (`fld 1.0 ; fdiv ; fst`);
//   the silent test is written `0.0f == m_band` (retail loads the member first: `fld [ecx+18h] ; fld 0.0 ; fucompp`).
#include <math.h>
#pragma intrinsic(sin)
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

void CStateVariableFilter::SetParameters(const SFilterParameters* parameters, float scale)
{
    SetCutoff(parameters->Frequency, scale);
    SetQ(parameters->Q);
    if (0.0f == m_band && 0.0f == m_low) {
        m_f = m_targetF;
        m_damping = m_targetDamping;
        m_gain = m_targetGain;
    }
}