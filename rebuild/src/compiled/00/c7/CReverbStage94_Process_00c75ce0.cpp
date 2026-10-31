// CReverbStage94::Process(const SStageSettings94* current, CDelayLine* line, float* output, const float* const* inputs, unsigned long count) -- one block of the reverb's four filtered taps with the current settings: on the SIMD ops table each tap reads its own ring into a scratch block, one four-wide gain filter runs over all four blocks and each tap then mixes its history taps and its block into its channel; on the x87 table each tap runs its whole Process; finally every tap feeds back the 4x4 mix of the four blocks
// Leaf: ?Process@CReverbStage94@@QAEXPBUSStageSettings94@@PAUCDelayLine@@PAMPBQBMK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75CE0, 432 bytes, thiscall, `ret 14h`, 16-byte aligned ebp frame (0x834 bytes).
// Identity: the landed caller CReverbProcessor::ProcessStage94 0x00C6A180 binds this address as
//   `?Process@CReverbStage94@@QAEXPBUSStageSettings94@@PAUCDelayLine@@PAMPBQBMK@Z` (its non-blending arm); the stage is the
//   four-tap part at reverb +0x94 that the landed definers Allocate 0x00C75C30 / Reset 0x00C75CA0 / Process 0x00C75E90
//   call CReverbTapBank (taps +4, stride 0x28, channel count +0xA4; same class, two descriptive names). libpk: no PDB/RTTI.
// Callees: CDspObject::s_pOps 0x0143CCDC slot 0 GetKind (1 = the SIMD table), CReverbFilterTap::ReadRing 0x00C89B40 (new
//   descriptive name: forwards to CDspOps::ReadRing(block, &m_ring, m_ring.m_pos, count); landed under a propagated
//   CTCActivationReceptorBase::AddTrigger label), CDspOps::FilterGain4 0x00C88E60 (new descriptive name: the SIMD table's
//   four-wide CDspGain filter, thiscall on s_pOps), CReverbFilterTap::MixHistory 0x00C89B60, CReverbFilterTap::Process
//   0x00C89750, CReverbFilterTap::Feedback 0x00C89AA0 (landed), the mix matrix g_reverbMixMatrix 0x012BF088.
// Shape: the landed CReverbTapBank::Process 0x00C75E90 TU (four 128-float blocks in one 16-byte aligned local, `j % m_channels`
//   named above each call); the SIMD path also fills two four-pointer arrays (filter parameters, tap gain states).
struct CDspOps;
struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
};
struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
};
struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04
    unsigned int m_pos;                    // +0x08
};
struct CDelayLine {                        // the caller's name for the history ring (same three words)
    float*        m_pBuffer;
    unsigned long m_length;
    unsigned long m_position;
};
struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};
struct SReverbTapParams {                  // 0x30 bytes
    unsigned int m_historyDelay[2];        // +0x00
    float m_historyGain[2];                // +0x08
    SGainParams m_filter;                  // +0x10
    float m_outputGain;                    // +0x2C
};
struct SStageSettings94 {                  // 0xC0 bytes: one tap record per tap
    SReverbTapParams m_taps[4];
};
struct CDspOps {                           // x87 vtable 0x012BF02C (SIMD build: ctor 0x00C88A30)
    virtual long GetKind();                // slot 0
    void FilterGain4(float (*dst)[128], float (*src)[128], float* const* states, const SGainParams* const* params,
                     unsigned int count);  // 0x00C88E60
};
struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C, 0x28 bytes
    CDspGain m_gain;                       // +0x04
    CRingBuffer m_ring;                    // +0x18
    unsigned int m_index;                  // +0x24
    void ReadRing(float* block, unsigned int count);                                              // 0x00C89B40
    void MixHistory(const SReverbTapParams* params, const CRingBuffer* history, float* block, float* channel,
                    unsigned int count);                                                          // 0x00C89B60
    void Process(const SReverbTapParams* params, const CRingBuffer* history, float* block, const float* input,
                 unsigned int count);                                                             // 0x00C89750
    void Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count);  // 0x00C89AA0
};
extern const float g_reverbMixMatrix[4][4];   // 0x012BF088

struct CReverbStage94 : CDspObject {       // vtable 0x012B9590 (CReverbTapBank in its definers)
    CReverbFilterTap m_taps[4];            // +0x04
    unsigned int m_channels;               // +0xA4
    long m_firstElement;                   // +0xA8
    long m_endElement;                     // +0xAC
    void Process(const SStageSettings94* current, CDelayLine* line, float* output, const float* const* inputs,
                 unsigned long count);
};

void CReverbStage94::Process(const SStageSettings94* current, CDelayLine* line, float* output, const float* const* inputs,
                             unsigned long count)
{
    __declspec(align(16)) float blocks[4][128];
    const CRingBuffer* history = reinterpret_cast<const CRingBuffer*>(line);
    if (s_pOps->GetKind() == 1) {
        const SGainParams* params[4];
        float* states[4];
        for (unsigned int i = 0; i < 4; ++i) {
            states[i] = m_taps[i].m_gain.m_gain;
            params[i] = &current->m_taps[i].m_filter;
            m_taps[i].ReadRing(blocks[i], count);
        }
        s_pOps->FilterGain4(blocks, blocks, states, params, count);
        for (unsigned int j = 0; j < 4; ++j) {
            unsigned int channel = j % m_channels;
            m_taps[j].MixHistory(&current->m_taps[j], history, blocks[j], const_cast<float*>(inputs[channel]), count);
        }
    } else {
        for (unsigned int j = 0; j < 4; ++j) {
            unsigned int channel = j % m_channels;
            m_taps[j].Process(&current->m_taps[j], history, blocks[j], inputs[channel], count);
        }
    }
    for (unsigned int k = 0; k < 4; ++k)
        m_taps[k].Feedback(blocks, output, g_reverbMixMatrix[k], count);
}