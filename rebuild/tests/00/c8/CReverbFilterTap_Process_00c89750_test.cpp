// Behaviour test for 0x00C89750 CReverbFilterTap::Process (standalone copy of the model and logic; the gain filter and ops stubbed)
#include <cstdio>
#include <cstring>
struct CRingBuffer {
    float* m_data; unsigned int m_size; unsigned int m_pos;
    unsigned int Back(unsigned int pos, unsigned int n) const { return pos >= n ? pos - n : m_size - n + pos; }
};
struct Params { unsigned int m_historyDelay[2]; float m_historyGain[2]; float m_filter[7]; float m_outputGain; };
static float g_apply[256]; static int g_applyCalls;
static void Apply(const float* src, float* block, unsigned int count) { ++g_applyCalls; for (unsigned int i = 0; i < count; ++i) block[i] += src[i]; }
static unsigned int g_ringPos[2]; static int g_ringCalls;
static void MixFromRing(float* dst, const CRingBuffer*, unsigned int pos, float gain, unsigned int count) { g_ringPos[g_ringCalls++] = pos; for (unsigned int i = 0; i < count; ++i) dst[i] += gain; }
static void Mix(float* dst, const float* src, float gain, unsigned int count) { for (unsigned int i = 0; i < count; ++i) dst[i] += src[i] * gain; }
struct Tap {
    CRingBuffer m_ring;
    void Process(const Params* params, const CRingBuffer* history, float* block, const float* input, unsigned int count)
    {
        float* data = m_ring.m_data;
        unsigned int pos = m_ring.m_pos;
        unsigned int start = history->Back(history->m_pos, count);
        std::memset(block, 0, count * sizeof(float));
        unsigned int first = count;
        if (pos + count > m_ring.m_size) first = m_ring.m_size - pos;
        Apply(data + pos, block, first);
        if (first < count) Apply(data, block + first, count - first);
        for (unsigned int i = 0; i < 2; ++i)
            MixFromRing(block, history, history->Back(start, params->m_historyDelay[i]), params->m_historyGain[i], count);
        Mix((float*)input, block, params->m_outputGain, count);
    }
};
int main()
{
    float ring[10]; for (int i = 0; i < 10; ++i) ring[i] = (float)i;
    float hist[16] = {0};
    Tap t = { { ring, 10, 8 } };
    CRingBuffer h = { hist, 16, 2 };
    Params p = { { 1, 5 }, { 0.5f, 0.25f }, {0}, 2.0f };
    float block[4], channel[4] = { 1, 1, 1, 1 };
    t.Process(&p, &h, block, channel, 4);
    // ring wraps: 8,9 then 0,1; history start = Back(2,4) = 14; taps at 13 and 9
    if (g_applyCalls != 2 || block[0] != 8.75f || block[3] != 1.75f || g_ringPos[0] != 13 || g_ringPos[1] != 9
        || channel[0] != 1.0f + 2.0f * 8.75f || channel[2] != 1.0f + 2.0f * 0.75f) { std::printf("00c89750_TEST FAIL\n"); return 1; }
    std::printf("00c89750_TEST PASS\n");
    return 0;
}