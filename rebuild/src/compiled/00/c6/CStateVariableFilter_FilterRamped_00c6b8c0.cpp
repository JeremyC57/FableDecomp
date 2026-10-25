// CStateVariableFilter::FilterRamped(short* samples, unsigned long count) -- low-pass `count` 16-bit samples in place while the coefficients ramp: per sample t = (ramp position + i) / block length interpolates f and damping between the current and target values, then band += f * (in - low - damping * band), low += f * band, and the output is trunc(low) clamped to 16 bits
// Grade: RELOCATION_MATCH (3 grades: x87 operand order needs (1.0f - t) written inline in both lerps) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?FilterRamped@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B8C0, 219 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer DSP, the Chamberlin state-variable filter of the matched 0x00C6BBD0 / 0x00C6BC80 /
//   0x00C6BD00 rows (same local model). Caller: Filter 0x00C6BAE0 (the ramp part of a block). Global: the mix block length
//   0x0139C9F0 (unsigned, 128).
// Retail-asm: 0x00C6B951 `fsub [0.5f] ; fstp [esp+14h] ; fld [esp+14h] ; fistp dword [esp+8] ; test edx,edx ; jge ; inc edx` -- libpk's inline-assembly truncation helper (the per-sample f local's slot is reused for the homed value). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Shape: both unsigned-to-float conversions get the `jge ; fadd [2^32]` fix-up; the interpolated f is a
//   float local (`fstp [esp+14h]`, reloaded by both state updates); the clamp is signed (`jle` / `jge`).
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
};
void CStateVariableFilter::FilterRamped(short* samples, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) {
        float t = (float)(m_rampPosition + i) / (float)g_mixBlockLength;
        long in = samples[i];
        float f = (1.0f - t) * m_f + t * m_targetF;
        float damping = (1.0f - t) * m_damping + t * m_targetDamping;
        m_band = f * (((float)in - m_low) - damping * m_band) + m_band;
        m_low = f * m_band + m_low;
        long out = FloatToLongTruncate(m_low);
        if (out > 0x7fff)
            out = 0x7fff;
        else if (out < -0x8000)
            out = -0x8000;
        samples[i] = (short)out;
    }
}