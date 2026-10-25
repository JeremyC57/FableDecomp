// CReverbPreDelay::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index) -- two-pass setup of the reverb's pre-delay line: half a second of samples rounded up to a multiple of four; with memory, take that many floats from the cursor, clear them, reset the read offset and record the line index; always add the line's bytes to the running total
// Grade: DIFFER(157v186) -- retail_asm candidate: the line length is a bare `fistp dword` float-to-int (inline asm in retail); ours calls _ftol
// Leaf: ?Allocate@CReverbPreDelay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76DD0, length 186 bytes, ret 10h. No callees (memset intrinsic). The
//   length is the libpk float-to-int truncation of `rate * 0.5f` (0.5f at 0x0122F59C: fild + 2^32 fix-up, fmul,
//   `fsub 0.5f ; fstp ; fld ; fistp dword`, then `if (i < 0) ++i`), a bare fistp that VC7.1 cannot emit from C++.
// Layout: the reverb's +0x1E0 member (vtable 0x012B9580): ring data / size / position +4 / +8 / +0xc, a read offset
//   +0x10 (cleared here), line index +0x14. Names descriptive (libpk, no PDB/RTTI).
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

inline long TruncToLong(float value)       // retail: `fsub 0.5f` + inline-asm `fistp dword`, then `if (i < 0) ++i`
{
    return (long)value;
}

struct CReverbPreDelay : CDspObject {      // vtable 0x012B9580
    CRingBuffer m_ring;                    // +0x04
    unsigned int m_readOffset;             // +0x10
    unsigned int m_index;                  // +0x14
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index);
};

void CReverbPreDelay::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    unsigned int length = (TruncToLong(rate * 0.5f) + 3) & ~3u;
    if (*pMemory) {
        m_ring.m_data = *pMemory;
        m_ring.m_size = length;
        m_ring.m_pos = 0;
        memset(m_ring.m_data, 0, length * sizeof(float));
        m_index = index;
        m_readOffset = 0;
        *pMemory += length;
    }
    *pBytes += length * sizeof(float);
}
