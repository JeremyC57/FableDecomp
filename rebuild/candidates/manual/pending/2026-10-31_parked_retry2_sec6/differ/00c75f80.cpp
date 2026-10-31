// CCombFilter::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index) -- two-pass buffer setup of one delay tap: with memory, take `length` floats from the cursor, clear them and record the line index; always add the buffer's bytes to the running total
// Leaf: ?Allocate@CCombFilter
// Grade: DIFFER(111v111) 4 lines (wave-2 sec6): renamed to the bound ?Allocate@CCombFilter@@QAEXIPAPAMPAII@Z
//   (CRingBuffer ring, m_mode +0x10, chunk 598/landed 0x00C698A0); residue unchanged: retail stores m_pos and re-reads
//   m_data before the memset count. Tried: memset by the length parameter (30 lines), data local inside Clear (same),
//   sibling CReverbFilterTap statement order (122 bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75F80, length 111 bytes, ret 10h. No callees (memset intrinsic).
//   The reverb's allocation pass 0x00C6A460 calls it for its four +0x1F8 taps (sizing pass: *pMemory == 0).
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CCombFilter (vtable 0x012B9588, ctor 0x00C69880, 0x14 bytes):
//   CDspObject base, ring {float* data +4, unsigned size +8, unsigned pos +0xC}, line index +0x10.
// Shape: the cursor is advanced before the member stores (`*pMemory = block + length` right after the read), the
//   memset re-reads m_data, and the byte total is the shared tail of both arms.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CRingBuffer {                        // the tap's ring (+0x04 of the tap)
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 floats
    unsigned int m_pos;                    // +0x08
    void Clear() { memset(m_data, 0, m_size * sizeof(float)); }
};
struct CCombFilter : CDspObject {           // vtable 0x012B9588
    CRingBuffer m_ring;                     // +0x04
    unsigned int m_mode;                  // +0x10
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);
};
void CCombFilter::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    if (*pMemory) {
        float* block = *pMemory;
        *pMemory += length;
        m_ring.m_data = block;
        m_ring.m_size = length;
        m_ring.m_pos = 0;
        m_ring.Clear();
        m_mode = index;
    }
    *pBytes += length * sizeof(float);
}
