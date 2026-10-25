// CReverbTapBank::Process(const SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history, float* feedback, const float* const* inputs, unsigned int changingIndex, unsigned int count, unsigned int rampPosition) -- one block of the bank's four filtered taps: each tap reads its channel's input into its own scratch block (ramping toward the target parameters when it is the line being changed), then each tap feeds back the 4x4 Householder mix of the four blocks
// Leaf: ?Process@CReverbTapBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75E90, length 221 bytes, ret 20h. Callees: CReverbFilterTap
//   ProcessRamp 0x00C89850 (7 args), Process 0x00C89750 (5 args), Feedback 0x00C89AA0 (blocks, feedback, matrix row,
//   count). Matrix: the 4x4 table 0x012BF088 (0.5 on the diagonal, -0.5 elsewhere). Called by the reverb's tap-bank
//   step 0x00C6A1CE when the changing line index lies in [m_firstIndex, m_endIndex).
// Layout (descriptive; libpk reverb module, no PDB/RTTI): see CReverbTapBank::Reset 0x00C75CA0 (taps +4, stride
//   0x28, line index +0x24 of each tap, channel count +0xA4). The parameter records are 0x30 bytes per tap.
// Shape: four 128-float scratch blocks in one 16-byte-aligned local; `i % m_channels` is a named local computed above the
//   per-tap branch (the input load stays in each arm); the target record walks as `current + delta`.
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
    unsigned int m_size;                   // +0x04
    unsigned int m_pos;                    // +0x08
};

struct SReverbTapParams {
    unsigned char _body[0x30];
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    float* m_data;                         // +0x18
    unsigned int m_size;                   // +0x1C
    unsigned int m_pos;                    // +0x20
    unsigned int m_index;                  // +0x24
    void ProcessRamp(const SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history,
        float* block, const float* input, unsigned int count, unsigned int rampPosition);                    // 0x00C89850
    void Process(const SReverbTapParams* current, const CRingBuffer* history, float* block, const float* input,
        unsigned int count);                                                                                 // 0x00C89750
    void Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count);          // 0x00C89AA0
};

extern const float g_reverbMixMatrix[4][4];   // 0x012BF088

struct CReverbTapBank : CDspObject {       // vtable 0x012B9590
    CReverbFilterTap m_taps[4];            // +0x04
    unsigned int m_channels;               // +0xA4
    unsigned int m_firstIndex;             // +0xA8
    unsigned int m_endIndex;               // +0xAC
    void Process(const SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history,
        float* feedback, const float* const* inputs, unsigned int changingIndex, unsigned int count, unsigned int rampPosition);
};

void CReverbTapBank::Process(const SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history,
    float* feedback, const float* const* inputs, unsigned int changingIndex, unsigned int count, unsigned int rampPosition)
{
    __declspec(align(16)) float blocks[4][128];
    for (unsigned int i = 0; i < 4; ++i) {
        unsigned int channel = i % m_channels;
        if (changingIndex == m_taps[i].m_index)
            m_taps[i].ProcessRamp(&current[i], &target[i], history, blocks[i], inputs[channel], count, rampPosition);
        else
            m_taps[i].Process(&current[i], history, blocks[i], inputs[channel], count);
    }
    for (unsigned int j = 0; j < 4; ++j)
        m_taps[j].Feedback(blocks, feedback, g_reverbMixMatrix[j], count);
}