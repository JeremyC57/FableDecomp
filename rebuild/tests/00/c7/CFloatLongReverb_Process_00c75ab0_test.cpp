// Behaviour test for 0x00C75AB0 CReverbDiffuser::ProcessRamped: gain-ramp vs gain-update choice by the first index, the
// output clear, per-stage ramp vs plain processing by stage index, the channel pick j % channels, and the stage-record
// adoption once count >= rampLength.
#include <cstdio>
#include <cstring>

struct SDiffuserStageParams { unsigned char _body[0x3c]; };
struct SDiffuserParams { unsigned char _pad00[0x60]; SDiffuserStageParams m_stages[2]; };

static int g_rampGain = 0, g_gain = 0, g_rampStage[2] = { 0, 0 }, g_stage[2] = { 0, 0 };
static const float* g_stageInput[2];

struct CReverbDiffuserStage {
    int slot; unsigned int m_index;
    void Process(const SDiffuserStageParams*, const SDiffuserParams*, float* output, const float* input, unsigned int count)
    { ++g_stage[slot]; g_stageInput[slot] = input; output[0] += 1.0f; (void)count; }
    void ProcessRamped(const SDiffuserStageParams*, const SDiffuserStageParams*, const SDiffuserParams*, float* output,
                       const float* input, unsigned int, unsigned int)
    { ++g_rampStage[slot]; g_stageInput[slot] = input; output[0] += 10.0f; }
};
struct CReverbConfig { unsigned int m_00, m_channels, m_rate; };
struct CReverbDiffuser {
    CReverbDiffuserStage m_stages[2];
    CReverbConfig* m_pConfig;
    unsigned int m_firstIndex;
    void UpdateGainRamped(unsigned int, const SDiffuserParams*, const SDiffuserParams*, const SDiffuserParams*, unsigned int, unsigned int) { ++g_rampGain; }
    void UpdateGain(unsigned int, const SDiffuserParams*, const SDiffuserParams*, unsigned int) { ++g_gain; }
    void ProcessRamped(SDiffuserParams* params, const SDiffuserParams* next, const SDiffuserParams* target, float* output,
                       const float* const* inputs, unsigned int index, unsigned int count, unsigned int rampLength)
    {
        if (index == m_firstIndex) {
            for (unsigned int i = 0; i < 3; ++i) UpdateGainRamped(i, params, next, target, count, rampLength);
        } else {
            for (unsigned int i = 0; i < 3; ++i) UpdateGain(i, params, target, count);
        }
        std::memset(output, 0, count * sizeof(float));
        for (unsigned int j = 0; j < 2; ++j) {
            unsigned int channel = j % m_pConfig->m_channels;
            if (index == m_stages[j].m_index) {
                m_stages[j].ProcessRamped(&params->m_stages[j], &next->m_stages[j], target, output, inputs[channel], count, rampLength);
                if (count >= rampLength) params->m_stages[j] = next->m_stages[j];
            } else
                m_stages[j].Process(&params->m_stages[j], target, output, inputs[channel], count);
        }
    }
};

int main()
{
    CReverbConfig cfg = { 0, 2, 44100 };
    CReverbDiffuser d; d.m_stages[0].slot = 0; d.m_stages[0].m_index = 5; d.m_stages[1].slot = 1; d.m_stages[1].m_index = 6;
    d.m_pConfig = &cfg; d.m_firstIndex = 4;
    SDiffuserParams p, n, t;
    std::memset(&p, 1, sizeof(p)); std::memset(&n, 2, sizeof(n)); std::memset(&t, 3, sizeof(t));
    float in0[4] = { 0 }, in1[4] = { 0 };
    const float* inputs[2] = { in0, in1 };
    float out[4] = { 7, 7, 7, 7 };
    d.ProcessRamped(&p, &n, &t, out, inputs, 5, 4, 8);           // gains update; stage 0 ramps (count < ramp: no adopt)
    int bad = g_gain != 3 || g_rampGain != 0 || g_rampStage[0] != 1 || g_stage[1] != 1 || out[0] != 11.0f || out[3] != 0.0f
              || g_stageInput[0] != in0 || g_stageInput[1] != in1 || p.m_stages[0]._body[0] != 1;
    d.ProcessRamped(&p, &n, &t, out, inputs, 4, 4, 4);           // first index: gains ramp; no stage matches
    bad |= g_rampGain != 3 || g_stage[0] != 1 || g_stage[1] != 2 || out[0] != 2.0f;
    cfg.m_channels = 1;
    d.ProcessRamped(&p, &n, &t, out, inputs, 6, 4, 4);           // stage 1 ramps and adopts next's record; channel 1 % 1 = 0
    bad |= g_rampStage[1] != 1 || g_stageInput[1] != in0 || p.m_stages[1]._body[0] != 2 || p.m_stages[1]._body[0x3b] != 2
           || p.m_stages[0]._body[0] != 1 || p._pad00[0] != 1;
    if (bad) { std::printf("00c75ab0_TEST FAIL\n"); return 1; }
    std::printf("00c75ab0_TEST PASS\n");
    return 0;
}