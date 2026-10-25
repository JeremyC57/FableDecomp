// CStateVariableFilter::Filter(short* samples, unsigned long count) -- low-pass a block of 16-bit samples in place: while the current coefficients differ from the targets the first samples run through FilterRamped until the ramp reaches the block length (then the targets are committed and the ramp restarts); the rest use the current f / damping
// Grade: DIFFER(252v235) -- retail_asm candidate: bare `fistp dword` truncation helper (0x00C6BB95); ours rounds in C++ + __ftol2 | 1 grade | next: land as retail_asm
// Leaf: ?Filter@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BAE0, 235 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer DSP (local model shared with 0x00C6B8C0 / 0x00C6BBD0). Callers: the voice mixer
//   0x00C5B003, 0x00C5BACD, 0x00C5BAE7. Callee: FilterRamped 0x00C6B8C0 (thiscall). Global: block length 0x0139C9F0.
// Retail-asm: 0x00C6BB87 `fsub [0.5f] ; fstp [esp+14h] ; fld [esp+14h] ; fistp dword [esp+10h] ; test ; jge ; inc` -- libpk's
//   inline-assembly truncation helper.
// Shape notes for landing: `m_f != m_targetF` loads the target first (fucompp); the commit copies the three target words
//   through eax = this (`mov eax,esi ; mov [eax],edx ...`), the signature of an inlined struct assignment returning *this.
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default). VC7.1 C++ cannot emit that (`(long)x` calls
// __ftol2 and /QIfist emits `fistp qword`), so the body below is a C++ stand-in; at landing it becomes
// the minimal inline-assembly `fld value ; fistp result` body (grade retail_asm, like the landed ROUND of 0x00BE6790).
__forceinline long FloatToLong(float value)
{
    return (long)(value < 0.0f ? value - 0.5f : value + 0.5f);     // stand-in: round to nearest (fistp: ties to even)
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
void CStateVariableFilter::Filter(short* samples, unsigned long count)
{
    unsigned long i = 0;
    if (m_f != m_targetF || m_damping != m_targetDamping) {
        unsigned long ramp = g_mixBlockLength - m_rampPosition;
        if (ramp > count)
            ramp = count;
        FilterRamped(samples, ramp);
        m_rampPosition += ramp;
        if (m_rampPosition == g_mixBlockLength) {
            m_f = m_targetF;
            m_damping = m_targetDamping;
            m_gain = m_targetGain;
            m_rampPosition = 0;
        }
        i = ramp;
    }
    for (; i < count; ++i) {
        long in = samples[i];
        m_band = m_f * (((float)in - m_low) - m_band * m_damping) + m_band;
        m_low = m_f * m_band + m_low;
        long out = FloatToLongTruncate(m_low);
        if (out > 0x7fff)
            out = 0x7fff;
        else if (out < -0x8000)
            out = -0x8000;
        samples[i] = (short)out;
    }
}
