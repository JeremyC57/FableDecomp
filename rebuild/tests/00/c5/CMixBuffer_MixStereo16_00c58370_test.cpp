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
    void MixStereo16(const short* src, const unsigned short* gains)
    {
        if (m_numChannels == 1) {
            MixAdd16(src, m_pChannel[0], gains[0] >> 1);
            MixAdd16(src + 128, m_pChannel[0], gains[1] >> 1);
            return;
        }
        MixAdd16(src, m_pChannel[0], gains[0]);
        MixAdd16(src + 128, m_pChannel[1], gains[1]);
        if (m_numChannels >= 4) {
            MixAdd16(src, m_pChannel[2], gains[2]);
            MixAdd16(src + 128, m_pChannel[3], gains[3]);
        }
    }
};
static CMixBuffer g_mix;
int main()
{
    short src[256]; for (int i = 0; i < 128; ++i) { src[i] = 1000; src[128 + i] = 400; }
    unsigned short gains[4] = { 0x8000, 0x4000, 0x2000, 0x1000 };
    g_mix.Init(); g_mix.m_numChannels = 1; g_mix.MixStereo16(src, gains);
    int ok = g_mix.m_calls == 2 && g_mix.m_pChannel[0][0] == 500 + 100;
    g_mix.Init(); g_mix.m_numChannels = 2; g_mix.MixStereo16(src, gains);
    ok = ok && g_mix.m_calls == 2 && g_mix.m_pChannel[0][0] == 1000 && g_mix.m_pChannel[1][0] == 200;
    g_mix.Init(); g_mix.m_numChannels = 5; g_mix.MixStereo16(src, gains);
    ok = ok && g_mix.m_calls == 4 && g_mix.m_pChannel[2][0] == 250 && g_mix.m_pChannel[3][0] == 50 && g_mix.m_pChannel[4][0] == 0;
    if (!ok) { std::printf("00c58370_TEST FAIL\n"); return 1; }
    std::printf("00c58370_TEST PASS\n");
    return 0;
}