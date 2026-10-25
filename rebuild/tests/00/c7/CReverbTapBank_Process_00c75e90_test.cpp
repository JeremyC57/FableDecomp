#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
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
static int fail() { std::printf("00c75e90_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
CDspObject::CDspObject() { }
CDspObject::~CDspObject() { }
const float g_reverbMixMatrix[4][4] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } };
static int g_kind[4]; static const float* g_in[4]; static float* g_block[4]; static int g_calls = 0;
static const SReverbTapParams* g_cur[4]; static const SReverbTapParams* g_tgt[4]; static unsigned int g_ramp[4];
static int g_feedbacks = 0; static const float* g_row[4]; static float (*g_blocks)[128]; static int g_blocksSame = 1;
void CReverbFilterTap::ProcessRamp(const SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer*,
    float* block, const float* input, unsigned int count, unsigned int rampPosition)
{ g_kind[g_calls] = 2; g_cur[g_calls] = current; g_tgt[g_calls] = target; g_ramp[g_calls] = rampPosition; g_in[g_calls] = input; g_block[g_calls++] = block; block[0] = input[0] * 2; (void)count; }
void CReverbFilterTap::Process(const SReverbTapParams* current, const CRingBuffer*, float* block, const float* input, unsigned int count)
{ g_kind[g_calls] = 1; g_cur[g_calls] = current; g_in[g_calls] = input; g_block[g_calls++] = block; block[0] = input[0]; (void)count; }
void CReverbFilterTap::Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count)
{
    if (g_feedbacks && blocks != g_blocks) g_blocksSame = 0;
    g_blocks = blocks; g_row[g_feedbacks++] = mixRow;
    feedback[0] += blocks[0][0] * mixRow[0] + blocks[1][0] * mixRow[1] + blocks[2][0] * mixRow[2] + blocks[3][0] * mixRow[3];
    (void)count;
}
int main()
{
    static SReverbTapParams current[4], target[4];
    CRingBuffer history; history.m_data = 0; history.m_size = 0; history.m_pos = 0;
    float in0[1] = { 1.0f }, in1[1] = { 3.0f };
    const float* inputs[2] = { in0, in1 };
    float feedback[1] = { 0.0f };
    CReverbTapBank bank;
    bank.m_channels = 2;
    for (unsigned int i = 0; i < 4; ++i) bank.m_taps[i].m_index = 10 + i;
    bank.Process(current, target, &history, feedback, inputs, 12, 64, 7);    // tap 2 is the one changing
    if (g_calls != 4 || g_feedbacks != 4 || !g_blocksSame) return fail();
    for (int i = 0; i < 4; ++i) {
        if (g_kind[i] != (i == 2 ? 2 : 1) || g_in[i] != inputs[i % 2] || g_cur[i] != &current[i]) return fail();
        if (g_block[i] != g_blocks[i] || g_row[i] != g_reverbMixMatrix[i]) return fail();
    }
    if (g_tgt[2] != &target[2] || g_ramp[2] != 7) return fail();
    // blocks: 1, 3, 2*1, 3 -> sum over rows of row . blocks
    float expect = 0.0f; const float b[4] = { 1, 3, 2, 3 };
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) expect += g_reverbMixMatrix[r][c] * b[c];
    if (!nearly(feedback[0], expect)) return fail();
    std::printf("00c75e90_TEST PASS\n");
    return 0;
}