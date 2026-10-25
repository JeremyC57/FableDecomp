// CReverbDiffuser::Allocate(const CReverbConfig* config, float** pMemory, unsigned int* pBytes, unsigned int* pIndex) -- two-pass setup of the diffuser: with memory, keep the config and reset the three gains; then claim the line indices and size both all-pass stages at one second of samples (the config's sample rate)
// Grade: DIFFER(166v178) -- retail_asm candidate: the stage length is a bare `fistp dword` float-to-int (inline asm in retail); ours calls _ftol
// Leaf: ?Allocate@CReverbDiffuser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C755B0, length 178 bytes, ret 10h. Callees: CDspGain::Reset 0x00C744E0
//   (x3), CReverbDiffuserStage::Allocate 0x00C89110 (x2: length, pMemory, pBytes, index). The stage length is the
//   libpk float-to-int truncation of the unsigned rate (fild + 2^32 fix-up, `fsub 0.5f ; fstp ; fld ; fistp dword`,
//   then `if (i < 0) ++i`): a bare fistp that VC7.1 cannot emit from C++ (a C cast calls _ftol).
// Layout: as CReverbDiffuser::Reset 0x00C75670 / Process 0x00C75A20 (gains +4, stages +0x40, config +0x90 with the
//   sample rate at +8, first / end line index +0x94 / +0x98). Names descriptive (libpk, no PDB/RTTI).
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
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);   // 0x00C89110
};

struct CReverbConfig {
    unsigned int _pad00;
    unsigned int m_channels;               // +0x04
    unsigned int m_rate;                   // +0x08 samples per second
};

inline long TruncToLong(float value)       // retail: `fsub 0.5f` + inline-asm `fistp dword`, then `if (i < 0) ++i`
{
    return (long)value;
}

struct CReverbDiffuser : CDspObject {      // vtable 0x012BF080
    CDspGain m_gains[3];                   // +0x04
    CReverbDiffuserStage m_stages[2];      // +0x40
    const CReverbConfig* m_pConfig;        // +0x90
    unsigned int m_firstIndex;             // +0x94
    unsigned int m_endIndex;               // +0x98
    void Allocate(const CReverbConfig* config, float** pMemory, unsigned int* pBytes, unsigned int* pIndex);
};

void CReverbDiffuser::Allocate(const CReverbConfig* config, float** pMemory, unsigned int* pBytes, unsigned int* pIndex)
{
    if (*pMemory) {
        m_pConfig = config;
        for (int i = 0; i < 3; ++i)
            m_gains[i].Reset();
    }
    m_firstIndex = *pIndex;
    ++*pIndex;
    unsigned int length = TruncToLong((float)config->m_rate);
    for (int j = 0; j < 2; ++j) {
        m_stages[j].Allocate(length, pMemory, pBytes, *pIndex);
        ++*pIndex;
    }
    m_endIndex = *pIndex;
}
