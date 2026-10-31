// Behaviour test for CSoftwareVoice::ClearMix (0x00C5AF50): silence `frames` shorts per voice channel at the planar
// mix ring's write cursor (channel stride = capacity shorts), commit the frames, reset the resampler phase.
#include <cstdio>
#include <cstring>
struct CAudioRingBuffer {
    unsigned char* m_pData; unsigned int m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    void CommitWrite(unsigned int frames) { m_queued += frames; m_writePos += frames; if (m_writePos >= m_capacity) m_writePos -= m_capacity; }
};
static CAudioRingBuffer g_mixRing;
struct CSoftwareVoice {
    unsigned int m_numChannels, m_phase;
    void ClearMix(unsigned int frames)
    {
        short* p = (short*)(g_mixRing.m_pData + g_mixRing.m_frameBytes * g_mixRing.m_writePos);
        for (unsigned int c = 0; c < m_numChannels; ++c) { std::memset(p, 0, frames * sizeof(short)); p += g_mixRing.m_capacity; }
        g_mixRing.CommitWrite(frames);
        m_phase = 0;
    }
};
int main()
{
    short data[3 * 16];
    for (int i = 0; i < 48; ++i) data[i] = (short)(i + 1);
    g_mixRing.m_pData = (unsigned char*)data; g_mixRing.m_capacity = 16; g_mixRing.m_frameBytes = 2;
    g_mixRing.m_writePos = 12; g_mixRing.m_queued = 0;
    CSoftwareVoice v; v.m_numChannels = 2; v.m_phase = 0x7ff;
    v.ClearMix(3);
    int ok = v.m_phase == 0 && g_mixRing.m_writePos == 15 && g_mixRing.m_queued == 3;
    for (int c = 0; c < 3; ++c)
        for (int i = 0; i < 16; ++i) {
            bool cleared = c < 2 && i >= 12 && i < 15;
            ok = ok && (data[c * 16 + i] == 0) == cleared;
        }
    if (!ok) { std::printf("00c5af50_TEST FAIL\n"); return 1; }
    std::printf("00c5af50_TEST PASS\n");
    return 0;
}