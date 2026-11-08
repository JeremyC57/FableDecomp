// Behaviour test for 0x00C5C360 CSoftwareStreamVoice::MixRingBlock: the channel count picks the mono / stereo / quad /
// 5.1 kernel, the source is the ring's read pointer, a pending ramp passes (target - level) / 128 steps and adopts the targets.
#include <cstdio>
#include <cstring>

static int g_kind = 0; static const short* g_src = 0; static const unsigned short* g_gains = 0; static short g_steps[6];
static unsigned short g_levelsSeen[6];
class CMixBuffer {
public:
    void note(int k, const short* s, const unsigned short* g, const void* st)
    {
        g_kind = k; g_src = s; g_gains = g; std::memcpy(g_levelsSeen, g, sizeof(g_levelsSeen));
        if (st) std::memcpy(g_steps, st, sizeof(g_steps)); else std::memset(g_steps, 0x55, sizeof(g_steps));
    }
    void MixMono16(const short* s, const unsigned short* g) { note(1, s, g, 0); }
    void MixMono16Ramped(const short* s, const unsigned short* g, const short* st) { note(11, s, g, st); }
    void MixStereo16(const short* s, const unsigned short* g) { note(2, s, g, 0); }
    void MixStereo16Ramped(const short* s, const unsigned short* g, const unsigned short* st) { note(12, s, g, st); }
    void MixQuad16(const short* s, const unsigned short* g) { note(4, s, g, 0); }
    void MixQuad16Ramped(const short* s, const unsigned short* g, const unsigned short* st) { note(14, s, g, st); }
    void MixSurround16(const short* s, const unsigned short* g) { note(6, s, g, 0); }
    void MixSurround16Ramped(const short* s, const unsigned short* g, const unsigned short* st) { note(16, s, g, st); }
};
struct CCodaSystem { CMixBuffer* m_pMixBuffer; };
static CMixBuffer g_mix; static CCodaSystem g_system = { &g_mix }; static CCodaSystem* g_pCodaSystem = &g_system;
class CAudioRingBuffer {
public:
    unsigned char* m_pData; unsigned int m_capacity; unsigned int m_frameBytes; unsigned int m_channels; unsigned int m_readPos;
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; }
};
class CSoftwareStreamVoice {
public:
    unsigned int     m_numChannels;
    CAudioRingBuffer m_ring;
    bool             m_levelRampPending;
    unsigned short   m_channelLevel[6];
    unsigned short   m_channelTarget[6];
    void MixRingBlock();
};

void CSoftwareStreamVoice::MixRingBlock()
{
    const short* src = (const short*)m_ring.ReadPointer();
    CMixBuffer* mix = g_pCodaSystem->m_pMixBuffer;
    short steps[6];
    if (m_numChannels == 1) {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixMono16Ramped(src, m_channelLevel, steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixMono16(src, m_channelLevel);
        }
    } else if (m_numChannels == 2) {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixStereo16Ramped(src, m_channelLevel, (const unsigned short*)steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixStereo16(src, m_channelLevel);
        }
    } else if (m_numChannels == 4) {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixQuad16Ramped(src, m_channelLevel, (const unsigned short*)steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixQuad16(src, m_channelLevel);
        }
    } else {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixSurround16Ramped(src, m_channelLevel, (const unsigned short*)steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixSurround16(src, m_channelLevel);
        }
    }
}

static int fail(int n) { std::printf("00c5c360_TEST FAIL %d\n", n); return 1; }

int main()
{
    static unsigned char ring[0x1000];
    static const unsigned int chans[4] = { 1, 2, 4, 6 };
    static const int kinds[4] = { 1, 2, 4, 6 };
    for (int c = 0; c < 4; ++c)
    {
        CSoftwareStreamVoice v; std::memset(&v, 0, sizeof(v));
        v.m_numChannels = chans[c]; v.m_ring.m_pData = ring; v.m_ring.m_frameBytes = 0x100; v.m_ring.m_readPos = 3;
        for (int i = 0; i < 6; ++i) { v.m_channelLevel[i] = (unsigned short)(0x1000 * i); v.m_channelTarget[i] = (unsigned short)(0x7fff - 0x80 * i); }
        v.MixRingBlock();
        if (g_kind != kinds[c] || g_src != (const short*)(ring + 0x300) || g_gains != v.m_channelLevel || v.m_channelLevel[2] != 0x2000) return fail(1);
        v.m_levelRampPending = true;
        v.m_channelTarget[5] = 0;                      // a falling ramp: (0 - 0x5000) / 128 = -160
        v.MixRingBlock();
        if (g_kind != kinds[c] + 10 || v.m_levelRampPending) return fail(2);
        for (int i = 0; i < 6; ++i)
        {
            int target = (i == 5) ? 0 : 0x7fff - 0x80 * i;
            if (g_steps[i] != (short)((target - 0x1000 * i) / 128) || g_levelsSeen[i] != 0x1000 * i || v.m_channelLevel[i] != target) return fail(3);
        }
        if (g_steps[5] != -160) return fail(4);
    }
    std::printf("00c5c360_TEST PASS\n");
    return 0;
}