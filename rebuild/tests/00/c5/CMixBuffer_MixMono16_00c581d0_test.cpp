// Behaviour test for 00c581d0: CMixBuffer::MixMono16 -- one MixAdd16 per channel with that channel's gain
#include <cstdio>
static int g_calls = 0; static const short* g_src[8]; static long* g_dst[8]; static unsigned short g_gain[8];
class CMixBuffer {
public:
    virtual ~CMixBuffer() { } virtual void Reset() { } virtual void Clear() { }
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain) { g_src[g_calls] = src; g_dst[g_calls] = dst; g_gain[g_calls] = gain; ++g_calls; }
    long* m_pChannel[6]; long m_active; unsigned long m_numChannels;
    void MixMono16(const short* src, const unsigned short* gains) { for (unsigned long i = 0; i < m_numChannels; ++i) MixAdd16(src, m_pChannel[i], gains[i]); }
};
int main()
{
    static long acc[6][128]; short src[128] = { 0 }; unsigned short gains[6] = { 100, 200, 300, 40000, 5, 6 };
    CMixBuffer m; for (int c = 0; c < 6; ++c) m.m_pChannel[c] = acc[c]; m.m_active = 1; m.m_numChannels = 4;
    m.MixMono16(src, gains);
    int ok = g_calls == 4 && g_src[0] == src && g_dst[2] == acc[2] && g_gain[3] == 40000 && g_gain[0] == 100;
    m.m_numChannels = 0; m.MixMono16(src, gains); ok = ok && g_calls == 4;
    std::printf(ok ? "00c581d0_TEST PASS\n" : "00c581d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
