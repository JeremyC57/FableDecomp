// CReverbDiffuser::ProcessRamped(SDiffuserParams* params, const SDiffuserParams* next, const SDiffuserParams* target, float* output, const float* const* inputs, unsigned int index, unsigned int count, unsigned int rampLength) -- one block of the diffuser while a parameter change is pending: the gains ramp towards the new parameters when this is the diffuser's first index (else they update as usual), the output block is cleared, and each stage whose index matches ramps to its new stage record (adopting it once the block covers the ramp) while the others process normally
// Leaf: ?ProcessRamped@CReverbDiffuser@@QAEXPAUSDiffuserParams@@PBU2@1PAMPBQBMIII@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75AB0, 277 bytes, thiscall `ret 20h` (eight arguments). No code or data reference
//   reaches it in the retail image (dead in TLC); it sits right after CReverbDiffuser::Process 0x00C75A20 in the libpk
//   reverb module and is that function's ramped twin (same this, same callees plus their ramped siblings).
// Names: DESCRIPTIVE. libpk has no PDB or RTTI; the class / struct / member names are the landed CReverbDiffuser models'
//   (Process 0x00C75A20, Reset 0x00C75670); ProcessRamped and its two callees' names are descriptive. The manifest label
//   "Graphics_ProcessTerrainStreamingChunks" is wrong (no graphics or terrain code here).
// Callees: CReverbDiffuser::UpdateGainRamped 0x00C75820 (descriptive; the 6-argument sibling of UpdateGain, called with
//   params/next/target/count/rampLength), CReverbDiffuser::UpdateGain 0x00C756C0, CReverbDiffuserStage::ProcessRamped
//   0x00C89350 (descriptive; 7 arguments), CReverbDiffuserStage::Process 0x00C891A0; memset intrinsic (`rep stosd`);
//   the 0x3C-byte stage-record copy is `rep movsd` with ecx = 15.
// Layout: the landed CReverbDiffuser model (stages +0x40 stride 0x28 with their index at +0x24, config +0x90 with the
//   channel count at +4, first index +0x94); SDiffuserParams carries its two stage records at +0x60.
// Shape: the index test against m_firstIndex picks one of two gain loops; the stage loop computes j % channels before
//   the stage-index test (both arms load inputs[] from it); `count >= rampLength` adopts next's stage record.
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

struct SDiffuserStageParams {
    unsigned char _body[0x3c];
};

struct SDiffuserParams {
    unsigned char _pad00[0x60];
    SDiffuserStageParams m_stages[2];      // +0x60
};

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
    void Process(const SDiffuserStageParams* params, const SDiffuserParams* target, float* output, const float* input,
        unsigned int count);               // 0x00C891A0
    void ProcessRamped(const SDiffuserStageParams* params, const SDiffuserStageParams* next, const SDiffuserParams* target,
        float* output, const float* input, unsigned int count, unsigned int rampLength);   // 0x00C89350 (descriptive)
};

struct CReverbConfig {
    unsigned int m_00;
    unsigned int m_channels;               // +0x04
    unsigned int m_rate;                   // +0x08
};

struct CReverbDiffuser : CDspObject {      // vtable 0x012BF080
    CDspGain m_gains[3];                   // +0x04
    CReverbDiffuserStage m_stages[2];      // +0x40
    CReverbConfig* m_pConfig;              // +0x90
    unsigned int m_firstIndex;             // +0x94
    unsigned int m_endIndex;               // +0x98
    void UpdateGain(unsigned int index, const SDiffuserParams* params, const SDiffuserParams* target, unsigned int count);   // 0x00C756C0
    void UpdateGainRamped(unsigned int index, const SDiffuserParams* params, const SDiffuserParams* next,
        const SDiffuserParams* target, unsigned int count, unsigned int rampLength);   // 0x00C75820 (descriptive)
    void ProcessRamped(SDiffuserParams* params, const SDiffuserParams* next, const SDiffuserParams* target, float* output,
        const float* const* inputs, unsigned int index, unsigned int count, unsigned int rampLength);
};


void CReverbDiffuser::ProcessRamped(SDiffuserParams* params, const SDiffuserParams* next, const SDiffuserParams* target,
                                    float* output, const float* const* inputs, unsigned int index, unsigned int count,
                                    unsigned int rampLength)
{
    if (index == m_firstIndex)
    {
        for (unsigned int i = 0; i < 3; ++i)
            UpdateGainRamped(i, params, next, target, count, rampLength);
    }
    else
    {
        for (unsigned int i = 0; i < 3; ++i)
            UpdateGain(i, params, target, count);
    }
    memset(output, 0, count * sizeof(float));
    for (unsigned int j = 0; j < 2; ++j)
    {
        unsigned int channel = j % m_pConfig->m_channels;
        if (index == m_stages[j].m_index)
        {
            m_stages[j].ProcessRamped(&params->m_stages[j], &next->m_stages[j], target, output, inputs[channel], count,
                                      rampLength);
            if (count >= rampLength)
                params->m_stages[j] = next->m_stages[j];
        }
        else
            m_stages[j].Process(&params->m_stages[j], target, output, inputs[channel], count);
    }
}