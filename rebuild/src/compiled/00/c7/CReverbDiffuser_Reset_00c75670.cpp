// CReverbDiffuser::Reset() -- reset the diffuser's three gain ramps, then clear both stages' delay buffers and reset their gains
// Leaf: ?Reset@CReverbDiffuser@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75670, length 79 bytes. Callee: CDspGain::Reset 0x00C744E0
//   (all four gain floats to 1.0f). Called by the reverb's reset 0x00C69C3F for its +0x144 member.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CReverbDiffuser (vtable 0x012BF080, ctor 0x00C75980):
//   CDspObject base, three 0x14-byte CDspGain at +4, two 0x28-byte CReverbDiffuserStage at +0x40 (vtable 0x012BF07C,
//   ctor 0x00C75530: CDspObject base, ring {data +4, size +8, pos +0xC}, CDspGain +0x10, line index +0x24),
//   config +0x90, first/end line index +0x94/+0x98.
// Shape: two counted loops; the stage reset is an inline member (memset of the whole buffer with no null test,
//   then the gain's out-of-line Reset), so VC7.1 walks &m_stages[0].m_data in esi and reuses ebx as the counter.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
};

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
    void Reset()
    {
        memset(m_data, 0, m_size * sizeof(float));
        m_gain.Reset();
    }
};

struct CReverbConfig;

struct CReverbDiffuser : CDspObject {      // vtable 0x012BF080
    CDspGain m_gains[3];                   // +0x04
    CReverbDiffuserStage m_stages[2];      // +0x40
    CReverbConfig* m_pConfig;              // +0x90
    unsigned int m_firstIndex;             // +0x94
    unsigned int m_endIndex;               // +0x98
    void Reset();
};

void CReverbDiffuser::Reset()
{
    for (int i = 0; i < 3; ++i)
        m_gains[i].Reset();
    for (int j = 0; j < 2; ++j)
        m_stages[j].Reset();
}