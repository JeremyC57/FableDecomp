// Behaviour test for CMixBuffer::MixSurround16 (00C58AB0): the planes reach the accumulator channels with the gains and steps
// of the speaker routing for 1 / 2 / 4 / other channels.
#include <cstdio>
struct SCall { int plane; int channel; unsigned short gain; int step; };
static SCall g_calls[16]; static int g_n = 0;
static const short* g_src = 0;
struct CMixBuffer {
    long* m_pChannel[6]; unsigned long m_numChannels; long m_acc[6][128];
    int Index(long* dst) { for (int i = 0; i < 6; ++i) if (dst == m_pChannel[i]) return i; return -1; }
    void MixAdd16(const short* src, long* dst, unsigned short gain)
    { SCall c = { (int)(src - g_src) / 128, Index(dst), gain, 0 }; g_calls[g_n++] = c; }
    void MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step)
    { SCall c = { (int)(src - g_src) / 128, Index(dst), gain, step }; g_calls[g_n++] = c; }
    void Init(unsigned long n) { for (int i = 0; i < 6; ++i) m_pChannel[i] = m_acc[i]; m_numChannels = n; g_n = 0; }
    void MixSurround16(const short* src, const unsigned short* gains)
    {
        if (m_numChannels == 1) {
            MixAdd16(src, m_pChannel[0], gains[0] >> 2);
            MixAdd16(src + 128, m_pChannel[0], gains[1] >> 2);
            MixAdd16(src + 256, m_pChannel[0], gains[4] >> 1);
            MixAdd16(src + 512, m_pChannel[0], gains[2] >> 2);
            MixAdd16(src + 640, m_pChannel[0], gains[3] >> 2);
        } else if (m_numChannels == 2) {
            MixAdd16(src, m_pChannel[0], gains[0] >> 1);
            MixAdd16(src + 128, m_pChannel[1], gains[1] >> 1);
            MixAdd16(src + 256, m_pChannel[0], gains[4] >> 2);
            MixAdd16(src + 256, m_pChannel[1], gains[4] >> 2);
            MixAdd16(src + 512, m_pChannel[0], gains[2] >> 1);
            MixAdd16(src + 640, m_pChannel[1], gains[3] >> 1);
        } else if (m_numChannels == 4) {
            MixAdd16(src, m_pChannel[0], gains[0]);
            MixAdd16(src + 128, m_pChannel[1], gains[1]);
            MixAdd16(src + 256, m_pChannel[0], gains[4] >> 1);
            MixAdd16(src + 256, m_pChannel[1], gains[4] >> 1);
            MixAdd16(src + 512, m_pChannel[2], gains[2]);
            MixAdd16(src + 640, m_pChannel[3], gains[3]);
        } else {
            MixAdd16(src, m_pChannel[0], gains[0]);
            MixAdd16(src + 128, m_pChannel[1], gains[1]);
            MixAdd16(src + 256, m_pChannel[4], gains[4]);
            MixAdd16(src + 384, m_pChannel[5], gains[5]);
            MixAdd16(src + 512, m_pChannel[2], gains[2]);
            MixAdd16(src + 640, m_pChannel[3], gains[3]);
        }
    }
};
int main()
{
    static short src[6 * 128]; g_src = src;
    unsigned short gains[6] = { 400, 800, 1200, 1600, 2000, 2400 };
    unsigned short steps[6] = { (unsigned short)-40, 80, (unsigned short)-120, 160, (unsigned short)-200, 240 };
    CMixBuffer m; bool ok = true;
    m.Init(1); m.MixSurround16(src, gains);
    ok = ok && g_n == 5 && g_calls[0].plane == 0 && g_calls[0].channel == 0 && g_calls[0].gain == 100 && g_calls[0].step == 0 && g_calls[1].plane == 1 && g_calls[1].channel == 0 && g_calls[1].gain == 200 && g_calls[1].step == 0 && g_calls[2].plane == 2 && g_calls[2].channel == 0 && g_calls[2].gain == 1000 && g_calls[2].step == 0 && g_calls[3].plane == 4 && g_calls[3].channel == 0 && g_calls[3].gain == 300 && g_calls[3].step == 0 && g_calls[4].plane == 5 && g_calls[4].channel == 0 && g_calls[4].gain == 400 && g_calls[4].step == 0;
    m.Init(2); m.MixSurround16(src, gains);
    ok = ok && g_n == 6 && g_calls[0].plane == 0 && g_calls[0].channel == 0 && g_calls[0].gain == 200 && g_calls[0].step == 0 && g_calls[1].plane == 1 && g_calls[1].channel == 1 && g_calls[1].gain == 400 && g_calls[1].step == 0 && g_calls[2].plane == 2 && g_calls[2].channel == 0 && g_calls[2].gain == 500 && g_calls[2].step == 0 && g_calls[3].plane == 2 && g_calls[3].channel == 1 && g_calls[3].gain == 500 && g_calls[3].step == 0 && g_calls[4].plane == 4 && g_calls[4].channel == 0 && g_calls[4].gain == 600 && g_calls[4].step == 0 && g_calls[5].plane == 5 && g_calls[5].channel == 1 && g_calls[5].gain == 800 && g_calls[5].step == 0;
    m.Init(4); m.MixSurround16(src, gains);
    ok = ok && g_n == 6 && g_calls[0].plane == 0 && g_calls[0].channel == 0 && g_calls[0].gain == 400 && g_calls[0].step == 0 && g_calls[1].plane == 1 && g_calls[1].channel == 1 && g_calls[1].gain == 800 && g_calls[1].step == 0 && g_calls[2].plane == 2 && g_calls[2].channel == 0 && g_calls[2].gain == 1000 && g_calls[2].step == 0 && g_calls[3].plane == 2 && g_calls[3].channel == 1 && g_calls[3].gain == 1000 && g_calls[3].step == 0 && g_calls[4].plane == 4 && g_calls[4].channel == 2 && g_calls[4].gain == 1200 && g_calls[4].step == 0 && g_calls[5].plane == 5 && g_calls[5].channel == 3 && g_calls[5].gain == 1600 && g_calls[5].step == 0;
    m.Init(6); m.MixSurround16(src, gains);
    ok = ok && g_n == 6 && g_calls[0].plane == 0 && g_calls[0].channel == 0 && g_calls[0].gain == 400 && g_calls[0].step == 0 && g_calls[1].plane == 1 && g_calls[1].channel == 1 && g_calls[1].gain == 800 && g_calls[1].step == 0 && g_calls[2].plane == 2 && g_calls[2].channel == 4 && g_calls[2].gain == 2000 && g_calls[2].step == 0 && g_calls[3].plane == 3 && g_calls[3].channel == 5 && g_calls[3].gain == 2400 && g_calls[3].step == 0 && g_calls[4].plane == 4 && g_calls[4].channel == 2 && g_calls[4].gain == 1200 && g_calls[4].step == 0 && g_calls[5].plane == 5 && g_calls[5].channel == 3 && g_calls[5].gain == 1600 && g_calls[5].step == 0;
    if (!ok) { std::printf("00c58ab0_TEST FAIL\n"); return 1; }
    std::printf("00c58ab0_TEST PASS\n");
    return 0;
}