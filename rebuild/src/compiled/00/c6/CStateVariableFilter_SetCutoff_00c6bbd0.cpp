// CStateVariableFilter::SetCutoff(unsigned long frequency, float scale) -- target frequency coefficient 2*sin(pi*fc/44100) for fc = frequency*scale clamped to 200..8000 Hz, capped at 1; when f*damping then exceeds 1 the damping is inverted and, if still unstable, set to 1/f
// Leaf: ?SetCutoff@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BBD0, 161 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer DSP (0x00C6Bxxx; no PDB/RTTI, no trace strings). The object is a Chamberlin state-variable
//   filter: the per-sample loop of 0x00C6BAE0 / 0x00C6B8C0 runs band += f * (in - low - q * band), low += f * band with
//   the coefficients at +0x00/+0x04 ramped toward the targets at +0x0c/+0x10 and snapped to them at the end of the ramp
//   (+0x20 the ramp position). The voice set-up 0x00C5CA60 (and 0x00C54C10) calls 0x00C6BC80 / 0x00C6BD00 for the filters at voice
//   +0xdc / +0x100 from a descriptor's two parameter records (flags 0x20000 / 0x40000, 0x80000 / 0x100000).
// Layout: local model (no PDB class), member names descriptive.
// Callers: 0x00C6BC80 and 0x00C6BD00 (same TU; both keep ecx and edx across the call).
// Callees: none (`fsin` is the intrinsic sin, `#pragma intrinsic(sin)`); float pool 200, 8000, pi/44100, 1.0, 2^32.
// Shape: the unsigned frequency converts with the `fild` + 2^32 fix-up; the clamp is `if (c < 200) c = 200; else if
//   (c > 8000) c = 8000;` on a float local (kept in st0: `fcom`, `fstp st0 ; fld`); `2.0f * sin(...)` is `fadd st0,st0`;
//   the 1.0f cap is an immediate store. The inverted-damping step reads +0x10 as if it held Q (retail behaviour).
#include <math.h>
#pragma intrinsic(sin)
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
}