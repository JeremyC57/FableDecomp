// CReverbTapBank::Allocate(unsigned int channels, const unsigned int* lengths, float** pMemory, unsigned int* pBytes, unsigned int* pIndex) -- two-pass setup of the bank's four filtered taps: keep the channel count (memory pass), record the first line index, allocate each tap with the next index, record the end index
// Leaf: ?Allocate@CReverbTapBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75C30, length 106 bytes, ret 14h. Callee: CReverbFilterTap::Allocate
//   0x00C896C0 (length, pMemory, pBytes, index). Called by the reverb's allocation pass 0x00C6A541 for +0x94 with
//   the config's channel count (+4) and its tap-length table at +0x1C.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): see CReverbTapBank::Reset 0x00C75CA0.
// Shape: `this` spilled into the push-ecx slot; the index is re-read from *pIndex for each call and after each
//   increment, the incremented value copied to eax for the end-index store after the loop.
struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    float* m_data;                         // +0x18
    unsigned int m_size;                   // +0x1C
    unsigned int m_pos;                    // +0x20
    unsigned int m_index;                  // +0x24
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);   // 0x00C896C0
};

struct CReverbTapBank : CDspObject {       // vtable 0x012B9590
    CReverbFilterTap m_taps[4];            // +0x04
    unsigned int m_channels;               // +0xA4 output channels (Process mixes tap i into channel i % m_channels)
    unsigned int m_firstIndex;             // +0xA8
    unsigned int m_endIndex;               // +0xAC
    void Allocate(unsigned int channels, const unsigned int* lengths, float** pMemory, unsigned int* pBytes, unsigned int* pIndex);
};

void CReverbTapBank::Allocate(unsigned int channels, const unsigned int* lengths, float** pMemory, unsigned int* pBytes, unsigned int* pIndex)
{
    if (*pMemory)
        m_channels = channels;
    m_firstIndex = *pIndex;
    for (unsigned int i = 0; i < 4; ++i) {
        m_taps[i].Allocate(lengths[i], pMemory, pBytes, *pIndex);
        ++*pIndex;
    }
    m_endIndex = *pIndex;
}