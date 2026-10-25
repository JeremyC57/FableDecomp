// Behaviour test for CAudioFormat::GetBytesPerSample @ 0x00C2D970 (copy of the CAudioFormat model and logic).
#include <cstdio>

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

struct CAudioFormat {
    unsigned int   m_length;
    unsigned int   m_loopStart;
    unsigned int   m_loopEnd;
    unsigned short m_channels;
    unsigned short m_codec;                  // 0 = PCM, 1/2 = ADPCM (64 samples in 36 bytes per channel)
    unsigned int   m_sampleRate;

    unsigned int GetSamplesPerBlock()
    {
        switch (m_codec) {
        case 0: return m_channels;
        case 1: case 2: return m_channels * 64;
        }
        return 0;
    }
    unsigned int GetBlockAlign()
    {
        switch (m_codec) {
        case 0: return m_channels * 2;
        case 1: case 2: return m_channels * 36;
        }
        return 0;
    }
    unsigned int BytesToSamples(unsigned int bytes)
    {
        unsigned int blocks = bytes / GetBlockAlign();
        return GetSamplesPerBlock() * blocks;
    }
    float BytesToSeconds(unsigned int bytes) { return (float)BytesToSamples(bytes) / (float)m_sampleRate; }
    float GetBytesPerSample() { return (float)GetBlockAlign() / (float)GetSamplesPerBlock(); }
};

static CAudioFormat Make(unsigned short channels, unsigned short codec, unsigned int rate)
{
    CAudioFormat f;
    f.m_length = 0; f.m_loopStart = 0; f.m_loopEnd = 0;
    f.m_channels = channels; f.m_codec = codec; f.m_sampleRate = rate;
    return f;
}

int main()
{
    CAudioFormat pcm = Make(2, 0, 44100), x = Make(2, 1, 22050), ms = Make(1, 2, 8000);
    bool ok = Near(pcm.GetBytesPerSample(), 2.0f) && Near(x.GetBytesPerSample(), 0.5625f) && Near(ms.GetBytesPerSample(), 0.5625f);
    if (!ok) { std::printf("00c2d970_TEST FAIL\n"); return 1; }
    std::printf("00c2d970_TEST PASS\n");
    return 0;
}