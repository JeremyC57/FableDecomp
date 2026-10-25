// CReverbDiffuserStage::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index) -- two-pass setup of one diffuser stage: size its ring as (length + 131) & ~3 floats; with memory, record the line index, take and clear the ring from the cursor, advance it and reset the gain; always add the ring's bytes to the running total
// Leaf: ?Allocate@CReverbDiffuserStage@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C89110, length 142 bytes (+ int3 padding), ret 10h. Callee:
//   CDspGain::Reset 0x00C744E0 (landed as "C00c744e0::M"). Caller: CReverbDiffuser's allocation 0x00C75640, once
//   per stage of its two-stage array at +0x40 (`mov ecx,edi`).
// Layout (descriptive; libpk reverb module, no PDB/RTTI; agent444's CReverbDiffuserStage, vtable 0x012BF07C, ctor
//   0x00C75530): CDspObject base, ring {data +4, size +8, pos +0xC}, CDspGain +0x10, line index +0x24.
// Shape: two-pass allocator -- the owner runs every part first with a null cursor to total the bytes, then again over
//   one block. The size is `(length + 131) & ~3` floats; VC7.1 tail-duplicates the closing `*pBytes += size * 4`
//   into both arms. The buffer is cleared through the ring's own inline Clear() (memset of m_data, m_size floats):
//   only that spelling gives retail's `mov ecx,esi ; shl ecx,2` count after the m_size/m_pos stores (a memset of
//   `size * 4` in the body gives `lea ecx,[esi*4]` scheduled above the stores).
//   The index is stored first, so the cursor is re-read (`mov ecx,[ebx]`) for the ring's data store.
#include <string.h>

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
};

struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
    void Clear() { memset(m_data, 0, m_size * sizeof(float)); }
};

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    CRingBuffer m_ring;                    // +0x04
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);
};

void CReverbDiffuserStage::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    unsigned int size = (length + 131) & ~3u;
    if (*pMemory) {
        m_index = index;
        m_ring.m_data = *pMemory;
        m_ring.m_size = size;
        m_ring.m_pos = 0;
        m_ring.Clear();
        *pMemory += size;
        m_gain.Reset();
    }
    *pBytes += size * sizeof(float);
}