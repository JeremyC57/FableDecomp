// CStateVariableFilter::FilterWithGain(short* samples, unsigned long count) -- the gain (peaking) form of the state-variable filter over a block of 16-bit samples in place: while the current coefficients differ from the targets the first samples run through FilterRampedWithGain until the ramp reaches the block length (then the targets are committed); the rest output in + gain * damping * high, clamped to 16 bits
// Grade: RELOCATION_MATCH -- retail_asm through the shared helper libpk/FtoL.h (no inline asm in this row). w24A: the damping product's x87 operand order (retail `fld [esi+4] ; fmul [esi+18h]`) follows the spelling of the OTHER statements of the loop, not of the product: the state updates as `+=` with the high-pass value first and the output as `in + gain * damping * high` (the landed FilterRampedWithGain 0x00C6B7C0 spelling) give it.
// Leaf: ?FilterWithGain@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B9D0, 269 bytes, `ret 8` (thiscall).
// Module: libpk's software mixer DSP; the twin of the landed CStateVariableFilter::Filter 0x00C6BAE0 (same local model,
//   same object: the voice keeps two 0x24-byte filters at +0xdc / +0x100). Callers: 0x00C5B020 (voice +0xdc, flag 0x60000
//   without 0xa0000) and 0x00C5BB01 (voice +0x100, flag 0x140000) -- the filters SetParametersWithGain 0x00C6BD00 sets up.
// Callee: FilterRampedWithGain 0x00C6B7C0 (thiscall; the ramped twin of FilterRamped 0x00C6B8C0, interpolating f,
//   damping and gain). Global: the mix block length 0x0139C9F0.
// Name: descriptive (libpk has no PDB or RTTI); "WithGain" after the landed SetParametersWithGain / FilterRampedWithGain.
// Model: the local CStateVariableFilter of the landed Filter 0x00C6BAE0 (libpk has no shared class header).
// Retail-asm: 0x00C6BAA7 `fistp dword ptr [esp+10h]` (after `fsub [0.5f] ; fstp [esp+14h] ; fld [esp+14h]`, then `test ; jge ; inc`) -- libpk's inline-assembly truncation helper FtoL (shared libpk/FtoL.h), as in the landed Filter 0x00C6BAE0. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's inline assembly.
// Shape: three coefficient tests (f, gain, damping: target loaded first, fucompp); the commit is the 12-byte struct copy
//   through eax = this; the high-pass value stays on the x87 stack for both state updates and the output.
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