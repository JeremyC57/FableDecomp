#include <cstdio>
// Behaviour test for CSoundFormat::GetPCMBufferSize (0x00C2D920): same logic over a copy of the model,
// with the two converters reproduced from their retail bodies (0x00C2D790/0x00C2D7D0/0x00C2D800/0x00C2D820).
struct CSoundFormat {
    unsigned long Size, m_reserved1, m_reserved2;
    unsigned short Channels, Encoding;
    unsigned long SampleRate;
    CSoundFormat(unsigned short channels) : Size(0), m_reserved1(0), m_reserved2(0) { SampleRate = 44100; Channels = channels; Encoding = 0; }
    unsigned long BlockSamples() const { if (Encoding == 0) return Channels; if (Encoding <= 2) return (unsigned long)Channels << 6; return 0; }
    unsigned long BlockBytes() const { if (Encoding == 0) return (unsigned long)Channels << 1; if (Encoding <= 2) return (unsigned long)Channels * 36; return 0; }
    unsigned long BytesToSamples(unsigned long bytes) { return bytes / BlockBytes() * BlockSamples(); }
    unsigned long SamplesToBytes(unsigned long samples) { return samples / BlockSamples() * BlockBytes(); }
    unsigned long GetPCMBufferSize()
    {
        unsigned long size = Size;
        CSoundFormat pcm(Channels);
        unsigned long pcmBytes = pcm.SamplesToBytes(BytesToSamples(size));
        return pcmBytes > size ? pcmBytes : size;
    }
};
int main()
{
    CSoundFormat adpcm(2); adpcm.Encoding = 1; adpcm.Size = 72 * 10;      // 10 stereo ADPCM blocks of 72 bytes
    CSoundFormat pcm(1); pcm.Size = 1000;
    // ADPCM: 720 bytes -> 10 blocks * 128 samples = 1280 samples -> 1280 / 2 * 4 = 2560 PCM bytes
    if (adpcm.GetPCMBufferSize() != 2560 || pcm.GetPCMBufferSize() != 1000) { std::printf("00c2d920_TEST FAIL\n"); return 1; }
    std::printf("00c2d920_TEST PASS\n");
    return 0;
}