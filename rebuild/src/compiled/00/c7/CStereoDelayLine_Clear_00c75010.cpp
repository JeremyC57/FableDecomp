// CStereoDelayLine::Clear() -- zero both delay lines' sample buffers (when allocated) and rewind their write positions
// Leaf: ?Clear@CStereoDelayLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75010, length 61 bytes. No callees (memset intrinsic).
//   The reverb's reset 0x00C69C4E tail-jumps here for its +0x25C member.
// Layout (descriptive; libpk reverb module): vptr +0, two 0x10-byte CDelayLine at +4 {vptr, float* data,
//   unsigned size, unsigned pos}.
// Shape: a counted loop over the two elements calling an inline CDelayLine::Clear (`if (m_data) memset(m_data,
//   0, m_size * sizeof(float)); m_pos = 0;`): VC7.1 walks one pointer at &m_lines[0].m_data with a dec counter.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CDelayLine {
    virtual ~CDelayLine();                 // 0x00C75400
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDelayLine();
    void Clear()
    {
        if (m_data)
            memset(m_data, 0, m_size * sizeof(float));
        m_pos = 0;
    }
};

struct CStereoDelayLine {
    virtual ~CStereoDelayLine();
    CDelayLine m_lines[2];                 // +0x04
    void Clear();
};

void CStereoDelayLine::Clear()
{
    for (int i = 0; i < 2; ++i)
        m_lines[i].Clear();
}