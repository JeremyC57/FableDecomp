// Behaviour test for 00c89b60: CReverbFilterTap::MixHistory -- mix two history taps out of the shared ring into the block, then the block into the channel buffer
#include <cstdio>

struct CRingBuffer {
    float* m_data;
    unsigned int m_size;
    unsigned int m_pos;
    unsigned int Back(unsigned int pos, unsigned int n) const { return pos >= n ? pos - n : m_size - n + pos; }
};

struct SReverbTapParams {
    unsigned int m_historyDelay[2];
    float m_historyGain[2];
    float m_filter[7];
    float m_outputGain;
};

struct CDspOps {
    void MixFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count)
    {
        for (unsigned int i = 0; i < count; i++) dst[i] += ring->m_data[(pos + i) % ring->m_size] * gain;
    }
    void Mix(float* dst, const float* src, float gain, unsigned int count)
    {
        for (unsigned int i = 0; i < count; i++) dst[i] += src[i] * gain;
    }
};

struct CDspObject { static CDspOps* s_pOps; };
CDspOps* CDspObject::s_pOps = 0;

struct CReverbFilterTap : CDspObject {
    void MixHistory(const SReverbTapParams* params, const CRingBuffer* history, float* block, float* channel, unsigned int count);
};

void CReverbFilterTap::MixHistory(const SReverbTapParams* params, const CRingBuffer* history, float* block, float* channel, unsigned int count)
{
    unsigned int start = history->Back(history->m_pos, count);
    for (unsigned int i = 0; i < 2; ++i)
        s_pOps->MixFromRing(block, history, history->Back(start, params->m_historyDelay[i]), params->m_historyGain[i], count);
    s_pOps->Mix(channel, block, params->m_outputGain, count);
}

static int near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    CDspOps ops;
    CDspObject::s_pOps = &ops;
    float hist[16];
    for (int i = 0; i < 16; i++) hist[i] = (float)i;
    CRingBuffer h; h.m_data = hist; h.m_size = 16; h.m_pos = 2;   // block start = 2 - 4 wraps to 14
    SReverbTapParams p;
    p.m_historyDelay[0] = 1; p.m_historyDelay[1] = 3;            // taps at 13 and 11
    p.m_historyGain[0] = 1.0f; p.m_historyGain[1] = 2.0f;
    p.m_outputGain = 0.5f;
    float block[4] = { 0, 0, 0, 0 }, chan[4] = { 10, 10, 10, 10 };
    CReverbFilterTap t;
    t.MixHistory(&p, &h, block, chan, 4);
    int ok = 1;
    for (int i = 0; i < 4; i++) {
        float want = hist[(13 + i) % 16] * 1.0f + hist[(11 + i) % 16] * 2.0f;
        if (!near(block[i], want) || !near(chan[i], 10.0f + want * 0.5f)) ok = 0;
    }
    if (!ok) { std::printf("00c89b60_TEST FAIL\n"); return 1; }
    std::printf("00c89b60_TEST PASS\n");
    return 0;
}