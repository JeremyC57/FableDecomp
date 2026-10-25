// CReverbDiffuser::Process(const SDiffuserParams* params, const SDiffuserParams* target, float* output, const float* const* inputs, unsigned int count) -- one block of the diffuser: update its three gain ramps, clear the output block, then run both stages, each on its channel's input
// Leaf: ?Process@CReverbDiffuser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75A20, length 132 bytes, ret 14h. Callees: CReverbDiffuser::UpdateGain
//   0x00C756C0 (gain index, params, target, count), CReverbDiffuserStage::Process 0x00C891A0 (stage params, target,
//   output, input, count); memset intrinsic. Called by the reverb's diffuser step 0x00C6A275.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): see CReverbDiffuser::Reset 0x00C75670 (stages +0x40,
//   stride 0x28; config +0x90 with the channel count at +4). The parameter block carries two 0x3C-byte stage
//   records at +0x60.
// Shape: `memset(output, 0, count * sizeof(float))` on a plain variable is `rep stosd` with ecx = count; the stage
//   loop walks the stage, the stage record and the index in ebx/edi/esi.
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
    void Process(const SDiffuserParams* params, const SDiffuserParams* target, float* output, const float* const* inputs, unsigned int count);
};

void CReverbDiffuser::Process(const SDiffuserParams* params, const SDiffuserParams* target, float* output, const float* const* inputs, unsigned int count)
{
    for (unsigned int i = 0; i < 3; ++i)
        UpdateGain(i, params, target, count);
    memset(output, 0, count * sizeof(float));
    for (unsigned int j = 0; j < 2; ++j)
        m_stages[j].Process(&params->m_stages[j], target, output, inputs[j % m_pConfig->m_channels], count);
}