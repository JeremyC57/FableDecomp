#include <cstdio>
#include <cstring>
// Test copy of the GameCODA mixer accumulator with working kernels (128-sample planar blocks).
struct CMixBuffer {
    int*          m_pChannel[6];
    int           m_active;
    unsigned int  m_numChannels;
    unsigned char m_storage[0xc10];
    int           m_calls;
    void MixAdd16(const short* src, int* dst, unsigned short gain)
    { ++m_calls; for (int i = 0; i < 128; ++i) dst[i] += (src[i] * (int)gain) >> 15; }
    void MixAdd32Gain(const int* src, int* dst, unsigned short gain)
    { ++m_calls; for (int i = 0; i < 128; ++i) dst[i] += (src[i] * (int)gain) >> 15; }
    void MixAdd32(const int* src, int* dst)
    { ++m_calls; for (int i = 0; i < 128; ++i) dst[i] += src[i]; }
    void MixAddFloatGain(const float* src, int* dst, float gain)
    { ++m_calls; for (int i = 0; i < 128; ++i) dst[i] += (int)(src[i] * gain); }
    void MixAddFloat(const float* src, int* dst)
    { ++m_calls; for (int i = 0; i < 128; ++i) dst[i] += (int)src[i]; }
    void MixAdd16Ramp(const short* src, int* dst, unsigned short gain, short step)
    { ++m_calls; for (int i = 0; i < 128; ++i) { dst[i] += (src[i] * (int)gain) >> 15; gain = (unsigned short)(gain + step); } }
    void Init()
    {
        m_active = 1; m_numChannels = 0; m_calls = 0;
        int* p = (int*)(((size_t)m_storage + 15) & ~(size_t)15);
        for (int i = 0; i < 6; ++i)
            m_pChannel[i] = p + 128 * i;
        std::memset(m_storage, 0, sizeof(m_storage));
    }
    void MixStereoFloat(const float* src)
    {
        if (m_numChannels == 1) {
            MixAddFloatGain(src, m_pChannel[0], 0.5f);
            MixAddFloatGain(src + 128, m_pChannel[0], 0.5f);
            return;
        }
        MixAddFloat(src, m_pChannel[0]);
        MixAddFloat(src + 128, m_pChannel[1]);
        if (m_numChannels >= 4) {
            MixAddFloat(src, m_pChannel[2]);
            MixAddFloat(src + 128, m_pChannel[3]);
        }
    }
};
static CMixBuffer g_mix;
int main()
{
    float src[256]; for (int i = 0; i < 128; ++i) { src[i] = 100.0f; src[128 + i] = 40.0f; }
    g_mix.Init(); g_mix.m_numChannels = 1; g_mix.MixStereoFloat(src);
    int ok = g_mix.m_calls == 2 && g_mix.m_pChannel[0][5] == 70 && g_mix.m_pChannel[1][5] == 0;
    g_mix.Init(); g_mix.m_numChannels = 2; g_mix.MixStereoFloat(src);
    ok = ok && g_mix.m_calls == 2 && g_mix.m_pChannel[0][5] == 100 && g_mix.m_pChannel[1][5] == 40 && g_mix.m_pChannel[2][5] == 0;
    g_mix.Init(); g_mix.m_numChannels = 6; g_mix.MixStereoFloat(src);
    ok = ok && g_mix.m_calls == 4 && g_mix.m_pChannel[2][5] == 100 && g_mix.m_pChannel[3][5] == 40 && g_mix.m_pChannel[4][5] == 0;
    if (!ok) { std::printf("00c58270_TEST FAIL\n"); return 1; }
    std::printf("00c58270_TEST PASS\n");
    return 0;
}