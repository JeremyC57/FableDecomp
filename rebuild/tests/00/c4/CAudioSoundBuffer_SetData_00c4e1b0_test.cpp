#include <cstdio>
static int g_getErrors = 0, g_uploads = 0, g_creates = 0, g_shadows = 0, g_lastFormat = -1, g_lastSize = 0, g_lastFreq = 0, g_checkResult = 0, g_shadowResult = 0;
static unsigned int g_lastBuffer = 0; static const void* g_lastData = 0;
struct CAudioContext {
    int GetError() { ++g_getErrors; return 0; }
    void BufferData(unsigned int buffer, int format, const void* data, int size, int frequency)
    { ++g_uploads; g_lastBuffer = buffer; g_lastFormat = format; g_lastData = data; g_lastSize = size; g_lastFreq = frequency; }
    int CheckError() { return g_checkResult; }
};
static CAudioContext g_audioContext;
struct SWaveData { int m_sampleFormat; unsigned int m_channels; int m_frequency; int m_size; const void* m_pData; };
static inline int ToALFormat(int sampleFormat, unsigned int channels)
{
    switch (sampleFormat) {
    case 1: return channels == 1 ? 0x1101 : 0x1103;
    case 2: return channels == 1 ? 0x1100 : 0x1102;
    }
    return 0;
}
struct CAudioSoundBuffer {
    unsigned int m_buffer; unsigned int m_flags;
    int Create(const SWaveData* wave) { (void)wave; ++g_creates; return 55; }
    int KeepShadowCopy() { ++g_shadows; return g_shadowResult; }
    int SetData(const SWaveData* wave)
    {
        if (wave->m_channels > 2)
            return -14;
        if (!(m_flags & 1))
            return Create(wave);
        g_audioContext.GetError();
        int format = ToALFormat(wave->m_sampleFormat, wave->m_channels);
        g_audioContext.BufferData(m_buffer, format, wave->m_pData, wave->m_size, wave->m_frequency);
        int result = g_audioContext.CheckError();
        if (result >= 0 && (m_flags & 2))
            result = KeepShadowCopy();
        return result;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c4e1b0_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    char pcm[16]; CAudioSoundBuffer b; b.m_buffer = 7; b.m_flags = 0;
    SWaveData w; w.m_sampleFormat = 1; w.m_channels = 3; w.m_frequency = 22050; w.m_size = 16; w.m_pData = pcm;
    CHECK(b.SetData(&w) == -14 && g_creates == 0 && g_uploads == 0);
    w.m_channels = 1;
    CHECK(b.SetData(&w) == 55 && g_creates == 1 && g_uploads == 0 && g_getErrors == 0);
    b.m_flags = 1;
    CHECK(b.SetData(&w) == 0 && g_getErrors == 1 && g_uploads == 1 && g_lastFormat == 0x1101 && g_lastBuffer == 7 && g_lastData == pcm && g_lastSize == 16 && g_lastFreq == 22050);
    w.m_channels = 2; CHECK(b.SetData(&w) == 0 && g_lastFormat == 0x1103);
    w.m_sampleFormat = 2; CHECK(b.SetData(&w) == 0 && g_lastFormat == 0x1102);
    w.m_channels = 1; CHECK(b.SetData(&w) == 0 && g_lastFormat == 0x1100);
    w.m_sampleFormat = 9; CHECK(b.SetData(&w) == 0 && g_lastFormat == 0);
    CHECK(g_shadows == 0);
    b.m_flags = 3; g_shadowResult = -5;
    CHECK(b.SetData(&w) == -5 && g_shadows == 1);
    g_checkResult = -9;
    CHECK(b.SetData(&w) == -9 && g_shadows == 1);
    std::printf("00c4e1b0_TEST PASS\n");
    return 0;
}