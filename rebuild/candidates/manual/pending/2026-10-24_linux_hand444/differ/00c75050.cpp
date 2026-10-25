// CStereoDelayLine::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes) -- two-pass setup of the stereo pair of delay lines: when sizing, make each line the first prime at or above its scaled length (1051 and 337 samples at 48 kHz, made odd); with memory, hand each line its block from the cursor and clear it; always add both lines' bytes to the running total
// Grade: DIFFER(237v232) -- LTCG register argument: retail passes the IsPrime candidate in ebx (no push, no stack cleanup) and keeps it there across the next-prime loops; ours pushes it
// Leaf: ?Allocate@CStereoDelayLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75050, length 232 bytes, ret 0Ch. Callee: IsPrime 0x00C74EE0, an LTCG
//   static helper that takes its argument in ebx (the candidate stays in ebx across the next-prime loops: `or ebx,1 ;
//   call ; test al,al ; jne ; add ebx,2 ; call ...`), which a VC7.1 non-LTCG build cannot reproduce. The divisions by
//   48000 are the magic multiply 0x057619F1 / shr 10.
// Layout: as CStereoDelayLine::Clear 0x00C75010 (two 0x10-byte CDelayLine at +4: vptr, data, size, position).
//   Names descriptive (libpk, no PDB/RTTI).
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

bool IsPrime(unsigned int n);              // 0x00C74EE0 (n in ebx in retail)

inline unsigned int NextPrime(unsigned int n)
{
    unsigned int candidate = n | 1;
    while (!IsPrime(candidate))
        candidate += 2;
    return candidate;
}

struct CDelayLine {                        // vtable 0x012BF074, 0x10 bytes
    virtual ~CDelayLine();
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08 (floats)
    unsigned int m_pos;                    // +0x0C
    void Clear()
    {
        if (m_data)
            memset(m_data, 0, m_size * sizeof(float));
        m_pos = 0;
    }
};

struct CStereoDelayLine {                  // vtable 0x012BF078
    virtual ~CStereoDelayLine();           // 0x00C75440
    CDelayLine m_lines[2];                 // +0x04
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes);
};

void CStereoDelayLine::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes)
{
    if (!*pMemory) {
        m_lines[0].m_size = NextPrime(rate * 1051 / 48000);
        m_lines[1].m_size = NextPrime(rate * 337 / 48000);
    } else {
        for (int i = 0; i < 2; ++i) {
            m_lines[i].m_data = *pMemory;
            m_lines[i].Clear();
            *pMemory += m_lines[i].m_size;
        }
    }
    *pBytes += (m_lines[0].m_size + m_lines[1].m_size) * sizeof(float);
}
