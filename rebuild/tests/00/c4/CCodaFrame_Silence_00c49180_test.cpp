// Behaviour test for 00c49180: CCodaFrame::Silence -- zero channels*samples 16-bit samples and set valid = samples
#include <cstdio>
#include <cstring>

struct CCodaFrame {
    unsigned int m_channels, m_samples, m_validSamples, m_position;
    short*       m_pData;
    void Silence()
    {
        std::memset(m_pData, 0, m_channels * m_samples * sizeof(short));
        m_validSamples = m_samples;
    }
};

int main()
{
    short data[2 * 7 + 1];
    for (int i = 0; i < 15; ++i) data[i] = (short)(i + 100);
    CCodaFrame f; f.m_channels = 2; f.m_samples = 7; f.m_validSamples = 3; f.m_position = 0; f.m_pData = data;
    f.Silence();
    for (int i = 0; i < 14; ++i) if (data[i] != 0) { std::printf("00c49180_TEST FAIL\n"); return 1; }
    if (data[14] != 114 || f.m_validSamples != 7) { std::printf("00c49180_TEST FAIL\n"); return 1; }
    std::printf("00c49180_TEST PASS\n");
    return 0;
}