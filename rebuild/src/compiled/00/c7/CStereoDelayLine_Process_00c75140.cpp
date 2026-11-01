// CStereoDelayLine::Process(float* buffer, unsigned long count) -- run a float block in place through the reverb's two chained first-order allpass delay lines (coefficient 0.7): each line reads its delayed sample, writes `delayed * 0.7 + input` back at the read position, advances its write position with wrap, and passes `delayed - value * 0.7` on
// Leaf: ?Process@CStereoDelayLine@@QAEXPAMK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75140, 700 bytes, thiscall `ret 8`. No callees. Constant 0.7f @ 0x01255318 (the
//   landed 0x00713EE0 names it). Sole caller: the landed reverb post stage 0x00C6A080 on the CReverbProcessor member at +0x25C,
//   which is the CStereoDelayLine the landed constructor 0x00C75410 builds there (two 0x10-byte CDelayLine at +4 / +0x14:
//   vptr, m_data, m_size, m_pos). The caller's binding `?Process@PostStage@@QAEXPAMK@Z` names a class that does not own
//   this object (NAME_FIXES.tsv); the catalog's "FFT_RadixStride_Vectorized" is a placeholder. Names are the landed
//   descriptive libpk ones (no PDB/RTTI): Allpass is this file's name for the inline per-line step.
// Shape: VC7.1 /O2 unrolls the post-incremented pointer loop by four (count >= 4 block of (count-4)/4+1 passes, then the
//   remainder loop). The inline per-line helper takes its input by value: the first line's delayed sample is copied through
//   an integer register into a frame slot and its new value kept with `fst`, because both feed the second line's inlined
//   body (`delayed - value * 0.7f` is evaluated there, after the second line's own delayed sample is loaded).
struct CDelayLine {
    virtual ~CDelayLine();                 // 0x00C75400
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    float Allpass(float input)
    {
        float delayed = m_data[m_pos];
        float value = delayed * 0.7f + input;
        m_data[m_pos] = value;
        if (++m_pos == m_size)
            m_pos = 0;
        return delayed - value * 0.7f;
    }
};

struct CStereoDelayLine {
    virtual ~CStereoDelayLine();           // 0x00C75440
    CDelayLine m_lines[2];                 // +0x04
    void Process(float* buffer, unsigned long count);
};

void CStereoDelayLine::Process(float* buffer, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) {
        *buffer = m_lines[1].Allpass(m_lines[0].Allpass(*buffer));
        buffer++;
    }
}