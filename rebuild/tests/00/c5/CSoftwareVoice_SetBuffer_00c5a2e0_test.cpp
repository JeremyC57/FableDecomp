// Behaviour test for 0x00C5A2E0 CSoftwareVoice::SetBuffer
#include <cstdio>
struct Desc { unsigned int m_state, m_numChannels, m_sampleRate, m_dataBytes, m_playOffset, m_prefetchBytes; unsigned char* m_pSource; };
struct Ring { unsigned char* m_pData; unsigned int m_capacity, m_frameBytes, m_channels, m_readPos, m_queued; };
struct Buffer { Desc m_desc; };
static int g_baseCalls = 0;
struct Voice {
    Buffer* m_pBuffer; Desc m_desc; Ring m_ring;
    unsigned int GetSampleBytes() { return 4; } unsigned int GetLoopStart() { return 40; } unsigned int GetLoopEnd() { return 200; }
    void SetBuffer(Buffer* buffer, bool resume)
    {
        m_desc = buffer->m_desc;
        unsigned int sampleBytes;
        switch (m_desc.m_state) { case 1: sampleBytes = 2; break; case 2: sampleBytes = 1; break; default: sampleBytes = GetSampleBytes(); break; }
        unsigned int start;
        if (buffer == m_pBuffer && resume) {
            start = m_desc.m_playOffset;
            if (start == 0) start = (m_desc.m_state > 0 && m_desc.m_state <= 2) ? 0 : GetLoopStart();
        } else start = 0;
        unsigned int end;
        if (resume && m_desc.m_prefetchBytes != 0) end = m_desc.m_prefetchBytes;
        else if (m_desc.m_state > 0 && m_desc.m_state <= 2) end = m_desc.m_dataBytes;
        else end = GetLoopEnd();
        unsigned int channels = m_desc.m_numChannels;
        unsigned int frameBytes = channels * sampleBytes;
        unsigned int frames = (end - start) / frameBytes;
        m_ring.m_pData = m_desc.m_pSource + start; m_ring.m_capacity = frames; m_ring.m_frameBytes = frameBytes;
        m_ring.m_channels = channels; m_ring.m_readPos = 0; m_ring.m_queued = frames;
        ++g_baseCalls;
    }
};
static int Fail() { std::printf("00c5a2e0_TEST FAIL\n"); return 1; }
int main()
{
    static unsigned char pcm[1000];
    Buffer b; b.m_desc.m_state = 1; b.m_desc.m_numChannels = 2; b.m_desc.m_sampleRate = 22050; b.m_desc.m_dataBytes = 800;
    b.m_desc.m_playOffset = 100; b.m_desc.m_prefetchBytes = 0; b.m_desc.m_pSource = pcm;
    Voice v; v.m_pBuffer = 0;
    v.SetBuffer(&b, true);                                     // different buffer: from 0 to the data size
    if (v.m_ring.m_pData != pcm || v.m_ring.m_capacity != 200 || v.m_ring.m_frameBytes != 4 || v.m_desc.m_sampleRate != 22050) return Fail();
    v.m_pBuffer = &b; v.SetBuffer(&b, true);                   // same buffer resumed: from the play offset
    if (v.m_ring.m_pData != pcm + 100 || v.m_ring.m_capacity != 175 || v.m_ring.m_queued != 175) return Fail();
    b.m_desc.m_state = 2; b.m_desc.m_playOffset = 0; b.m_desc.m_prefetchBytes = 300; v.SetBuffer(&b, true);
    if (v.m_ring.m_pData != pcm || v.m_ring.m_capacity != 150 || v.m_ring.m_frameBytes != 2) return Fail();
    b.m_desc.m_state = 5; v.SetBuffer(&b, true);               // other formats: slot sample size and loop start
    if (v.m_ring.m_pData != pcm + 40 || v.m_ring.m_capacity != 32) return Fail();
    v.SetBuffer(&b, false);                                    // not resuming: loop end
    if (v.m_ring.m_pData != pcm || v.m_ring.m_capacity != 25 || g_baseCalls != 5) return Fail();
    std::printf("00c5a2e0_TEST PASS\n");
    return 0;
}