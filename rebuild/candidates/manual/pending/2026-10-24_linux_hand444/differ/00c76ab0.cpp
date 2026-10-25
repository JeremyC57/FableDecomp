// CReverbDelay::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index) -- two-pass setup of the reverb's feedback delay line: a quarter second of samples rounded up to a multiple of four; with memory, take that many floats from the cursor, clear them and record the line index; always add the line's bytes to the running total
// Grade: DIFFER(150v175) -- retail_asm candidate: the line length is a bare `fistp dword` float-to-int (inline asm in retail); ours calls _ftol
// Leaf: ?Allocate@CReverbDelay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76AB0, length 175 bytes, ret 10h. No callees (memset intrinsic). The
//   length is the libpk float-to-int truncation of `rate * 0.25f` (0.25f at 0x0123078C: fild + 2^32 fix-up, fmul,
//   `fsub 0.5f ; fstp ; fld ; fistp dword`, then `if (i < 0) ++i`), a bare fistp that VC7.1 cannot emit from C++.
// Layout: as CReverbDelay::Process 0x00C76B60 (ring data / size / position +4 / +8 / +0xc, line index +0x10). Names
//   descriptive (libpk, no PDB/RTTI).
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

struct CReverbDelay : CDspObject {         // vtable 0x012B957C
    CRingBuffer m_ring;                    // +0x04
    unsigned int m_index;                  // +0x10
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index);
};

void CReverbDelay::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    unsigned int length = (TruncToLong(rate * 0.25f) + 3) & ~3u;
    if (*pMemory) {
        m_ring.m_data = *pMemory;
        m_ring.m_size = length;
        m_ring.m_pos = 0;
        memset(m_ring.m_data, 0, length * sizeof(float));
        m_index = index;
        *pMemory += length;
    }
    *pBytes += length * sizeof(float);
}
