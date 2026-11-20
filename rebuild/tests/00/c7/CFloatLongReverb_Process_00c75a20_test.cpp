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
static int fail() { std::printf("00c75a20_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
CDspObject::CDspObject() { }
CDspObject::~CDspObject() { }
static unsigned int g_gainCalls[8]; static int g_gains = 0;
static const float* g_stageInput[4]; static const SDiffuserStageParams* g_stageParams[4]; static int g_stages = 0;
static bool g_outputClearAtStage = true;
void CReverbDiffuser::UpdateGain(unsigned int index, const SDiffuserParams*, const SDiffuserParams*, unsigned int)
{ g_gainCalls[g_gains++] = index; }
void CReverbDiffuserStage::Process(const SDiffuserStageParams* params, const SDiffuserParams*, float* output, const float* input, unsigned int count)
{
    if (g_stages == 0) for (unsigned int k = 0; k < count; ++k) if (output[k] != 0.0f) g_outputClearAtStage = false;
    g_stageParams[g_stages] = params; g_stageInput[g_stages++] = input;
    for (unsigned int k = 0; k < count; ++k) output[k] += input[k];
}
int main()
{
    static SDiffuserParams params, target;
    float in0[4] = { 1, 2, 3, 4 }, in1[4] = { 10, 20, 30, 40 };
    const float* inputs[2] = { in0, in1 };
    float out[6] = { 9, 9, 9, 9, 9, 9 };
    CReverbConfig config; config.m_00 = 0; config.m_channels = 2; config.m_rate = 48000;
    CReverbDiffuser diffuser;
    diffuser.m_pConfig = &config;
    diffuser.Process(&params, &target, out, inputs, 4);
    if (g_gains != 3 || g_gainCalls[0] != 0 || g_gainCalls[1] != 1 || g_gainCalls[2] != 2) return fail();
    if (g_stages != 2 || g_stageInput[0] != in0 || g_stageInput[1] != in1 || !g_outputClearAtStage) return fail();
    if (g_stageParams[0] != &params.m_stages[0] || g_stageParams[1] != &params.m_stages[1]) return fail();
    for (int k = 0; k < 4; ++k) if (out[k] != in0[k] + in1[k]) return fail();
    if (out[4] != 9 || out[5] != 9) return fail();
    config.m_channels = 1; g_stages = 0; g_gains = 0;            // mono: both stages read input 0
    diffuser.Process(&params, &target, out, inputs, 4);
    if (g_stageInput[0] != in0 || g_stageInput[1] != in0) return fail();
    for (int k = 0; k < 4; ++k) if (out[k] != 2 * in0[k]) return fail();
    std::printf("00c75a20_TEST PASS\n");
    return 0;
}