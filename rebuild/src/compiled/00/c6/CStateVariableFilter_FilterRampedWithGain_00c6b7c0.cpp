// CStateVariableFilter::FilterRampedWithGain(short* samples, unsigned long count) -- the gain (peaking) form of the ramped state-variable filter: per sample t = (ramp position + i) / block length interpolates f, damping and gain between the current and target values, then high = in - low - damping * band, band += f * high, low += f * band, and the output is trunc(in + gain * high * damping) clamped to 16 bits
// Grade: RELOCATION_MATCH retry1 -- the (1 - t) lerp weight as one named local `s` shared by the three lerps (fixes the gain-lerp fxch order)
// Leaf: ?FilterRampedWithGain@CStateVariableFilter
// Retail-asm: 0x00C6B86D `fsub [0.5f] ; fstp [esp+14h] ; fstp st(0) ; fld [esp+14h] ; fistp dword [esp+8] ; test edx,edx ; jge ; inc edx` -- libpk's inline-assembly truncation helper (the high-pass local's slot is reused for the homed value), the helper of the landed retail_asm FilterRamped 0x00C6B8C0. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B7C0, 249 bytes, thiscall `ret 8`.
// Identity: the landed CStateVariableFilter (0x00C6B8C0 FilterRamped, 0x00C6BAE0 Filter, 0x00C6BD00 SetParametersWithGain: the gain
//   coefficients +0x08 / +0x14): the same loop as FilterRamped plus the third (gain) lerp and the in + gain * high * damping output.
//   Its only caller 0x00C6BA21 is the gain twin of Filter (0x00C6B9D0) running the ramp part of the block. The method name is
//   descriptive (libpk: no PDB/RTTI); the catalog's "Audio_FilterSamples_2ndOrderDirect" is a placeholder.
// Global: the mix block length 0x0139C9F0 (unsigned, 128). Constants 1.0f 0x0122DED8, 0.5f 0x0122F59C, 2^32 0x0122DCB4.
// Model: the landed FilterRamped 0x00C6B8C0 (local model, member names descriptive) with its FloatToLong helpers verbatim.
// Shape: the f lerp is written `t * target + (1 - t) * current` (VC7.1 then multiplies +0x0C before +0x00, as retail); the
//   input sample is a float local
//   converted right after t; damping and high are float locals homed in the dead argument slots.
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default); VC7.1 expands the __forceinline helper in place.
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
extern unsigned long g_mixBlockLength;    // 0x0139C9F0 (128 samples)
struct CStateVariableFilter {
    float         m_f;                   // +0x00 current frequency coefficient
    float         m_damping;             // +0x04 current damping (1/Q)
    float         m_gain;                // +0x08 current gain coefficient
    float         m_targetF;             // +0x0c
    float         m_targetDamping;       // +0x10
    float         m_targetGain;          // +0x14
    float         m_band;                // +0x18 filter state
    float         m_low;                 // +0x1c filter state
    unsigned long m_rampPosition;        // +0x20 samples into the coefficient ramp
    void FilterRamped(short* samples, unsigned long count);
    void Filter(short* samples, unsigned long count);
    void FilterRampedWithGain(short* samples, unsigned long count);
};
void CStateVariableFilter::FilterRampedWithGain(short* samples, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) {
        float t = (float)(m_rampPosition + i) / (float)g_mixBlockLength;
        float in = (float)samples[i];
        float s = 1.0f - t;
        float f = t * m_targetF + s * m_f;
        float damping = s * m_damping + t * m_targetDamping;
        float high = (in - m_low) - damping * m_band;
        m_band = high * f + m_band;
        m_low = m_band * f + m_low;
        float gain = s * m_gain + t * m_targetGain;
        long out = FloatToLongTruncate(gain * high * damping + in);
        if (out > 0x7fff)
            out = 0x7fff;
        else if (out < -0x8000)
            out = -0x8000;
        samples[i] = (short)out;
    }
}