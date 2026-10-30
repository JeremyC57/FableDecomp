// CReverbDiffuser::UpdateGainRamped(unsigned int index, SDiffuserParams* params, const SDiffuserParams* next, CRingBuffer* history, unsigned int count, unsigned int rampLength) -- libpk reverb diffuser, while a parameter change is pending: cross-fade gain `index` from the old tap delay to the new one over 512 samples (read the old tap scaled down from rampLength/512 and mix the new tap in from 1 - rampLength/512 into an aligned scratch block), run the gain's cross-fade over the block, write it back at the gain's line offset ((index + 1) * spacing / 4 + base), and adopt the new tap delay once the fade ends in this block
// Leaf: ?UpdateGainRamped@CReverbDiffuser@@QAEXIPAUSDiffuserParams@@PBU2@PAUCRingBuffer@@II@Z
// Grades: 1 (RELOCATION_MATCH first grade; compile-only sweeps chose the float conversion as its own local)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75820, 343 bytes, thiscall `ret 18h`, 16-byte aligned frame (`and esp,-16`).
//   Caller: the landed CReverbDiffuser::ProcessRamped 0x00C75AE7 (the first-index arm, once per gain).
// Names: DESCRIPTIVE (libpk: no PDB, no RTTI); the class / method name is the landed ProcessRamped's binding. Its
//   spelling there, `?UpdateGainRamped@CReverbDiffuser@@QAEXIPBUSDiffuserParams@@00II@Z`, is wrong on two counts: params is
//   written (the closing `params->m_taps[index] = next->m_taps[index]`, and ProcessRamped's own params is non-const),
//   and the fourth parameter is the shared delay line {data, size, position} handed to CDspOps::WriteRing, a
//   CRingBuffer (NAME_FIXES.tsv).
// Callees: CDspOps::ReadRingScaledRamp 0x00C76560, CDspOps::MixRampFromRing 0x00C767E0, CDspOps::WriteRing 0x00C76460
//   (non-virtual members on the shared table CDspObject::s_pOps 0x0143CCDC), CDspGain::Crossfade 0x00C74D30 (the landed
//   definition's spelling). Floats: 1/512 0x012A3AF4, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (unsigned-to-float); the
//   -1/512 and +1/512 steps are pushed as immediates.
// Shape: the unsigned-to-float conversion is a named local (the inline `(float)rampLength * k` schedules the fmul
//   straight after the 2^32 correction; retail places it after the first tap compare).
struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
};

struct CDspOps {                           // x87 vtable 0x012BF02C (SIMD build: ctor 0x00C88A30)
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
    void WriteRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count);     // 0x00C76460
    void ReadRingScaledRamp(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step,
        unsigned int count);                                                                        // 0x00C76560
    void MixRampFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step,
        unsigned int count);                                                                        // 0x00C767E0
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
};

struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
    void Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count);   // 0x00C74CE0
    void Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode,
        unsigned int count, unsigned int remaining);                                                         // 0x00C74D30
};

struct SDiffuserStageParams {
    unsigned char _body[0x3c];
};

struct SDiffuserParams {
    unsigned int m_taps[3];                // +0x00 per-gain read delay (samples)
    SGainParams m_gains[3];                // +0x0C
    SDiffuserStageParams m_stages[2];      // +0x60
};

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
};

struct CReverbConfig {
    unsigned int m_00;
    unsigned int m_channels;               // +0x04
    unsigned int m_rate;                   // +0x08
    unsigned int m_0C;
    unsigned int m_tapBase;                // +0x10
    unsigned int m_tapSpacing;             // +0x14
};

struct CReverbDiffuser : CDspObject {      // vtable 0x012BF080
    CDspGain m_gains[3];                   // +0x04
    CReverbDiffuserStage m_stages[2];      // +0x40
    CReverbConfig* m_pConfig;              // +0x90
    unsigned int m_firstIndex;             // +0x94
    unsigned int m_endIndex;               // +0x98
    void UpdateGain(unsigned int index, const SDiffuserParams* params, const CRingBuffer* history, unsigned int count);
    void UpdateGainRamped(unsigned int index, SDiffuserParams* params, const SDiffuserParams* next, CRingBuffer* history,
        unsigned int count, unsigned int rampLength);
};
void CReverbDiffuser::UpdateGainRamped(unsigned int index, SDiffuserParams* params, const SDiffuserParams* next,
                                       CRingBuffer* history, unsigned int count, unsigned int rampLength)
{
    __declspec(align(16)) float block[128];
    unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
    float ramp = (float)rampLength;
    float fade = ramp * (1.0f / 512.0f);
    float rise = 1.0f - fade;
    unsigned int tap = params->m_taps[index];
    s_pOps->ReadRingScaledRamp(block, history, start >= tap ? start - tap : history->m_size - tap + start, fade,
                               -1.0f / 512.0f, count);
    tap = next->m_taps[index];
    s_pOps->MixRampFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, rise,
                            1.0f / 512.0f, count);
    m_gains[index].Crossfade(&params->m_gains[index], &next->m_gains[index], block, block, 1, count, rampLength);
    unsigned int offset = (index + 1) * m_pConfig->m_tapSpacing / 4 + m_pConfig->m_tapBase;
    s_pOps->WriteRing(history, start >= offset ? start - offset : history->m_size - offset + start, block, count);
    if (count >= rampLength)
        params->m_taps[index] = next->m_taps[index];
}