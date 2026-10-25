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
    void Unused() {}
};
static CMixBuffer g_mix;
int main()
{
    g_mix.Init();
    int ok = g_mix.m_active == 1 && g_mix.m_numChannels == 0 && ((size_t)g_mix.m_pChannel[0] & 15) == 0
          && (unsigned char*)g_mix.m_pChannel[0] >= g_mix.m_storage && (unsigned char*)g_mix.m_pChannel[0] < g_mix.m_storage + 16;
    for (int i = 1; i < 6; ++i) ok = ok && (char*)g_mix.m_pChannel[i] - (char*)g_mix.m_pChannel[i - 1] == 0x200;
    ok = ok && (unsigned char*)(g_mix.m_pChannel[5] + 128) <= g_mix.m_storage + sizeof(g_mix.m_storage);
    if (!ok) { std::printf("00c591f0_TEST FAIL\n"); return 1; }
    std::printf("00c591f0_TEST PASS\n");
    return 0;
}