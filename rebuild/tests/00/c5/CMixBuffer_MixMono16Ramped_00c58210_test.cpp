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
    void MixMono16Ramped(const short* src, const unsigned short* gains, const short* steps)
    {
        for (unsigned int i = 0; i < m_numChannels; ++i)
            MixAdd16Ramp(src, m_pChannel[i], gains[i], steps[i]);
    }
};
static CMixBuffer g_mix;
int main()
{
    g_mix.Init(); g_mix.m_numChannels = 3;
    short src[128]; for (int i = 0; i < 128; ++i) src[i] = 1000;
    unsigned short gains[3] = { 0x8000, 0x4000, 0 }; short steps[3] = { 0, -0x40, 0x100 };
    g_mix.MixMono16Ramped(src, gains, steps);
    int ok = g_mix.m_calls == 3 && g_mix.m_pChannel[0][0] == 1000 && g_mix.m_pChannel[0][127] == 1000
          && g_mix.m_pChannel[1][0] == 500 && g_mix.m_pChannel[1][1] == ((1000 * (0x4000 - 0x40)) >> 15)
          && g_mix.m_pChannel[2][0] == 0 && g_mix.m_pChannel[2][1] == ((1000 * 0x100) >> 15) && g_mix.m_pChannel[3][0] == 0;
    if (!ok) { std::printf("00c58210_TEST FAIL\n"); return 1; }
    std::printf("00c58210_TEST PASS\n");
    return 0;
}