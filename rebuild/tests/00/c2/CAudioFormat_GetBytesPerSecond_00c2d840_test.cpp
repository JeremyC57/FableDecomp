// Behaviour test for CAudioFormat::GetBytesPerSecond (0x00C2D840): PCM = channels*2*rate/channels, Xbox ADPCM =
// 36*channels*rate/(64*channels).
#include <cstdio>

struct CAudioFormat {
    unsigned long  m_length;
    unsigned long  m_loopStart;
    unsigned long  m_loopLength;
    unsigned short m_channels;
    unsigned short m_codec;
    unsigned long  m_sampleRate;
    unsigned long GetSamplesPerBlock();
    unsigned long GetBlockAlign();
    float GetBytesPerSecond();
};
unsigned long CAudioFormat::GetSamplesPerBlock()
{
    switch (m_codec) {
    case 0: return m_channels;
    case 1: case 2: return m_channels * 64;
    }
    return 0;
}
unsigned long CAudioFormat::GetBlockAlign()
{
    switch (m_codec) {
    case 0: return m_channels * 2;
    case 1: case 2: return m_channels * 36;
    }
    return 0;
}
float CAudioFormat::GetBytesPerSecond()
{
    return (float)GetBlockAlign() * (float)m_sampleRate / (float)GetSamplesPerBlock();
}

static bool Near(float a, float b) { float d = a - b; return d < 0.01f && d > -0.01f; }

int main()
{
    CAudioFormat pcm; pcm.m_channels = 2; pcm.m_codec = 0; pcm.m_sampleRate = 22050;
    if (!Near(pcm.GetBytesPerSecond(), 44100.0f)) { std::printf("00c2d840_TEST FAIL\n"); return 1; }
    CAudioFormat adpcm; adpcm.m_channels = 1; adpcm.m_codec = 1; adpcm.m_sampleRate = 44100;
    if (!Near(adpcm.GetBytesPerSecond(), 44100.0f * 36.0f / 64.0f)) { std::printf("00c2d840_TEST FAIL\n"); return 1; }
    std::printf("00c2d840_TEST PASS\n");
    return 0;
}