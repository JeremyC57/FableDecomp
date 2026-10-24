// Behaviour test for 0x00C69FB0 CReverbProcessor_00c6a460::ProcessCombs
#include <cstdio>
#include <cstring>
struct SCombSettings { float gain; };
struct SReverbSettings { SCombSettings Combs[4]; };
struct CDelayLine { int uses; };
static int g_blends = 0, g_processes = 0;
struct CCombFilter {
    long m_mode;
    void Blend(const SCombSettings* current, const SCombSettings* target, CDelayLine* line, float* output,
               const float* input, unsigned long count, unsigned long blendPosition)
    {
        ++g_blends; ++line->uses;
        for (unsigned long k = 0; k < count; ++k)
            output[k] += input[k] * (blendPosition ? target->gain : current->gain);
    }
    void Process(const SCombSettings* current, CDelayLine* line, float* output, const float* input, unsigned long count)
    {
        ++g_processes; ++line->uses;
        for (unsigned long k = 0; k < count; ++k)
            output[k] += input[k] * current->gain;
    }
};
struct CReverbProcessor {
    SReverbSettings* m_pSettings;
    SReverbSettings* m_pTargetSettings;
    bool m_blending;
    long m_blendMode;
    unsigned long m_blendPosition;
    unsigned long m_inputCount;
    CDelayLine m_line;
    CCombFilter m_combs[4];
    void ProcessCombs(const float* const* inputs, unsigned long count, float* output)
    {
        SReverbSettings* settings = m_pSettings;
        SReverbSettings* target = m_pTargetSettings;
        std::memset(output, 0, count * sizeof(float));
        unsigned long input = 0;
        for (int i = 0; i < 4; ++i) {
            if (m_blending && m_blendMode == m_combs[i].m_mode)
                m_combs[i].Blend(&settings->Combs[i], &target->Combs[i], &m_line, output, inputs[input], count, m_blendPosition);
            else
                m_combs[i].Process(&settings->Combs[i], &m_line, output, inputs[input], count);
            if (++input >= m_inputCount)
                input = 0;
        }
    }
};
int main()
{
    SReverbSettings cur = { { { 1.0f }, { 2.0f }, { 4.0f }, { 8.0f } } };
    SReverbSettings tgt = { { { 16.0f }, { 32.0f }, { 64.0f }, { 128.0f } } };
    float in0[2] = { 1.0f, 0.0f };
    float in1[2] = { 0.0f, 1.0f };
    const float* inputs[2] = { in0, in1 };
    float out[2] = { 99.0f, 99.0f };
    CReverbProcessor r;
    r.m_pSettings = &cur; r.m_pTargetSettings = &tgt;
    r.m_blending = false; r.m_blendMode = 1; r.m_blendPosition = 1; r.m_inputCount = 2; r.m_line.uses = 0;
    r.m_combs[0].m_mode = 1; r.m_combs[1].m_mode = 0; r.m_combs[2].m_mode = 1; r.m_combs[3].m_mode = 0;
    r.ProcessCombs(inputs, 2, out);
    // combs 0,2 read in0 (gains 1,4), combs 1,3 read in1 (gains 2,8)
    int ok = out[0] == 5.0f && out[1] == 10.0f && g_processes == 4 && g_blends == 0 && r.m_line.uses == 4;
    r.m_blending = true;
    r.ProcessCombs(inputs, 2, out);
    // combs 0 and 2 blend to the target gains (16, 64)
    ok = ok && out[0] == 80.0f && out[1] == 10.0f && g_blends == 2 && g_processes == 6;
    r.m_inputCount = 1;
    r.m_blending = false;
    r.ProcessCombs(inputs, 2, out);
    ok = ok && out[0] == 15.0f && out[1] == 0.0f;
    if (!ok) { std::printf("00c69fb0_TEST FAIL\n"); return 1; }
    std::printf("00c69fb0_TEST PASS\n");
    return 0;
}