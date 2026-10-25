// CStateVariableFilter::Filter(short* samples, unsigned long count) -- low-pass a block of 16-bit samples in place: while the current coefficients differ from the targets the first samples run through FilterRamped until the ramp reaches the block length (then the targets are committed and the ramp restarts); the rest use the current f / damping
// Grade: RELOCATION_MATCH (3 grades: 12-byte coefficient struct copy, m_current.damping * m_band order, i = ramp before the commit test) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?Filter@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BAE0, 235 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer DSP (local model shared with 0x00C6B8C0 / 0x00C6BBD0). Callers: the voice mixer
//   0x00C5B003, 0x00C5BACD, 0x00C5BAE7. Callee: FilterRamped 0x00C6B8C0 (thiscall). Global: block length 0x0139C9F0.
// Retail-asm: 0x00C6BB87 `fsub [0.5f] ; fstp [esp+14h] ; fld [esp+14h] ; fistp dword [esp+10h] ; test ; jge ; inc` -- libpk's inline-assembly truncation helper. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Shape: `m_current.f != m_target.f` loads the target first (fucompp); the commit copies the three target words
//   through eax = this (`mov eax,esi ; mov [eax],edx ...`), the signature of an inlined struct assignment returning *this.
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
struct SFilterCoefficients {
    float         f;                     // frequency coefficient
    float         damping;               // damping (1/Q)
    float         gain;                  // gain coefficient
};
struct CStateVariableFilter {
    SFilterCoefficients m_current;       // +0x00 (retail copies the target over it as one 12-byte struct)
    SFilterCoefficients m_target;        // +0x0c
    float         m_band;                // +0x18 filter state
    float         m_low;                 // +0x1c filter state
    unsigned long m_rampPosition;        // +0x20 samples into the coefficient ramp
    void FilterRamped(short* samples, unsigned long count);
    void Filter(short* samples, unsigned long count);
};
void CStateVariableFilter::Filter(short* samples, unsigned long count)
{
    unsigned long i = 0;
    if (m_current.f != m_target.f || m_current.damping != m_target.damping) {
        unsigned long ramp = g_mixBlockLength - m_rampPosition;
        if (ramp > count)
            ramp = count;
        FilterRamped(samples, ramp);
        m_rampPosition += ramp;
        i = ramp;
        if (m_rampPosition == g_mixBlockLength) {
            m_current = m_target;
            m_rampPosition = 0;
        }
    }
    for (; i < count; ++i) {
        long in = samples[i];
        m_band = m_current.f * (((float)in - m_low) - m_current.damping * m_band) + m_band;
        m_low = m_current.f * m_band + m_low;
        long out = FloatToLongTruncate(m_low);
        if (out > 0x7fff)
            out = 0x7fff;
        else if (out < -0x8000)
            out = -0x8000;
        samples[i] = (short)out;
    }
}