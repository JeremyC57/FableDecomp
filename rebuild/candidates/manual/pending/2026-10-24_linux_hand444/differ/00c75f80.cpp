// CReverbTap::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index) -- two-pass buffer setup of one delay tap: with memory, take `length` floats from the cursor, clear them and record the line index; always add the buffer's bytes to the running total
// Grade: DIFFER(111v111) -- register allocation: retail keeps the block in eax (cursor `lea edi,[eax+esi*4]`, the memset reloads m_data, size `mov ecx,esi ; shl ecx,2`); ours CSEs length*4 between the cursor and the memset size and keeps the block in edi
// Leaf: ?Allocate@CReverbTap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75F80, length 111 bytes, ret 10h. No callees (memset intrinsic).
//   The reverb's allocation pass 0x00C6A460 calls it for its four +0x1F8 taps (sizing pass: *pMemory == 0).
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CReverbTap (vtable 0x012B9588, ctor 0x00C69880, 0x14 bytes):
//   CDspObject base, ring {float* data +4, unsigned size +8, unsigned pos +0xC}, line index +0x10.
// Shape: the cursor is advanced before the member stores (`*pMemory = block + length` right after the read), the
//   memset re-reads m_data, and the byte total is the shared tail of both arms.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CReverbTap : CDspObject {           // vtable 0x012B9588
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    unsigned int m_index;                  // +0x10
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);
};

void CReverbTap::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    if (*pMemory) {
        float* block = *pMemory;
        *pMemory = block + length;
        m_data = block;
        m_size = length;
        m_pos = 0;
        memset(m_data, 0, length * sizeof(float));
        m_index = index;
    }
    *pBytes += length * sizeof(float);
}
