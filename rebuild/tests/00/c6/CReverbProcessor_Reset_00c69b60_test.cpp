// Behaviour test for 0x00C69B60 CReverbProcessor::Reset (standalone copy of the model and logic; part resets stubbed)
#include <cstdio>
#include <cstring>
struct Ring { float* m_data; unsigned int m_length; void Clear() { std::memset(m_data, 0, m_length * sizeof(float)); } };
struct Gain { float m_gain[4]; void Reset() { for (int i = 0; i < 4; ++i) m_gain[i] = 1.0f; } };
struct Part { int resets; void Reset() { ++resets; } };
struct Reverb {
    Ring m_combLine, m_line60, m_delay, m_preDelay, m_combs[4];
    Gain m_inputGain, m_outputGain;
    Part m_taps, m_diffuser, m_stereoDelay;
    void* m_pMemory; float* m_scratch;
    void Reset()
    {
        if (!m_pMemory) return;
        m_inputGain.Reset(); m_outputGain.Reset();
        m_combLine.Clear(); m_line60.Clear();
        std::memset(m_scratch, 0, 128 * sizeof(float));
        m_delay.Clear(); m_preDelay.Clear();
        for (int i = 0; i < 4; ++i) m_combs[i].Clear();
        m_taps.Reset(); m_diffuser.Reset(); m_stereoDelay.Reset();
    }
};
static float buf[9][130];
int main()
{
    for (int b = 0; b < 9; ++b) for (int i = 0; i < 130; ++i) buf[b][i] = 3.0f;
    Reverb r; std::memset(&r, 0, sizeof(r));
    Ring* rings[8] = { &r.m_combLine, &r.m_line60, &r.m_delay, &r.m_preDelay, &r.m_combs[0], &r.m_combs[1], &r.m_combs[2], &r.m_combs[3] };
    for (int k = 0; k < 8; ++k) { rings[k]->m_data = buf[k]; rings[k]->m_length = 10 + k; }
    r.m_scratch = buf[8];
    r.Reset();                                   // no memory: nothing happens
    if (buf[0][0] != 3.0f || r.m_taps.resets != 0) { std::printf("00c69b60_TEST FAIL\n"); return 1; }
    r.m_pMemory = buf;
    r.Reset();
    for (int k = 0; k < 8; ++k)
        if (buf[k][10 + k - 1] != 0.0f || buf[k][10 + k] != 3.0f) { std::printf("00c69b60_TEST FAIL\n"); return 1; }
    if (buf[8][127] != 0.0f || buf[8][128] != 3.0f || r.m_inputGain.m_gain[3] != 1.0f || r.m_outputGain.m_gain[0] != 1.0f
        || r.m_taps.resets != 1 || r.m_diffuser.resets != 1 || r.m_stereoDelay.resets != 1) { std::printf("00c69b60_TEST FAIL\n"); return 1; }
    std::printf("00c69b60_TEST PASS\n");
    return 0;
}