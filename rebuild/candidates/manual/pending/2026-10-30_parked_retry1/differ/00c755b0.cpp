// CReverbDiffuser::Allocate(const CReverbConfig* config, float** pMemory, unsigned int* pBytes, unsigned int* pIndex) -- two-pass setup of the diffuser: with memory, keep the config and reset the three gains; then claim the line indices and size both all-pass stages at one second of samples (the config's sample rate)
// Grade: IMPROVED(178v178) -- retry1: with the landed libpk FloatToLong / FloatToLongTruncate helpers (bare `fistp dword`, retail_asm) the stage length block matches; left is callee-saved colouring: retail keeps this in edi from entry (later the stage pointer, `add edi,40h`) and config in ebp with ebx counting, ours this in ecx (spilled), config in ebx, counters in edi/ebp.
// Leaf: ?Allocate@CReverbDiffuser
// Retail-asm: 0x00C75612 `fistp dword ptr [esp+24h]` (after `fsub [0.5f] ; fstp [esp+18h] ; fld [esp+18h]`) -- libpk's inline-assembly
//   truncation helper (the landed FloatToLong / FloatToLongTruncate of CVoiceEffect::Configure 0x00C6AEF0): VC7.1 C++ `(long)x` calls
//   _ftol2 and /QIfist emits `fistp qword`; no C/C++ form gives the bare dword fistp.
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

// libpk's inline-assembly float-to-long helper (the landed CVoiceEffect::Configure 0x00C6AEF0 spelling). Retail homes the value in a
// dead stack slot and rounds it with a bare `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default); VC7.1 expands
// the __forceinline helper in place.
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
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
    unsigned int length = FloatToLongTruncate((float)config->m_rate);
    for (int j = 0; j < 2; ++j) {
        m_stages[j].Allocate(length, pMemory, pBytes, *pIndex);
        ++*pIndex;
    }
    m_endIndex = *pIndex;
}
