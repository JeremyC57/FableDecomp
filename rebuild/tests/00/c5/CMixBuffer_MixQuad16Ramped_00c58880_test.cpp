// Behaviour test for CMixBuffer::MixQuad16Ramped (00C58880): the planes reach the accumulator channels with the gains and steps
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
    void MixQuad16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps)
    {
        if (m_numChannels == 1) {
            MixAdd16Ramp(src, m_pChannel[0], gains[0] >> 2, (short)steps[0] / 4);
            MixAdd16Ramp(src + 128, m_pChannel[0], gains[1] >> 2, (short)steps[1] / 4);
            MixAdd16Ramp(src + 256, m_pChannel[0], gains[2] >> 2, (short)steps[2] / 4);
            MixAdd16Ramp(src + 384, m_pChannel[0], gains[3] >> 2, (short)steps[3] / 4);
        } else if (m_numChannels == 2) {
            MixAdd16Ramp(src, m_pChannel[0], gains[0] >> 1, (short)steps[0] / 2);
            MixAdd16Ramp(src + 128, m_pChannel[1], gains[1] >> 1, (short)steps[1] / 2);
            MixAdd16Ramp(src + 256, m_pChannel[0], gains[2] >> 1, (short)steps[2] / 2);
            MixAdd16Ramp(src + 384, m_pChannel[1], gains[3] >> 1, (short)steps[3] / 2);
        } else {
            MixAdd16Ramp(src, m_pChannel[0], gains[0], steps[0]);
            MixAdd16Ramp(src + 128, m_pChannel[1], gains[1], steps[1]);
            MixAdd16Ramp(src + 256, m_pChannel[2], gains[2], steps[2]);
            MixAdd16Ramp(src + 384, m_pChannel[3], gains[3], steps[3]);
        }
    }
};
int main()
{
    static short src[6 * 128]; g_src = src;
    unsigned short gains[6] = { 400, 800, 1200, 1600, 2000, 2400 };
    unsigned short steps[6] = { (unsigned short)-40, 80, (unsigned short)-120, 160, (unsigned short)-200, 240 };
    CMixBuffer m; bool ok = true;
    m.Init(1); m.MixQuad16Ramped(src, gains, steps);
    ok = ok && g_n == 4 && g_calls[0].plane == 0 && g_calls[0].channel == 0 && g_calls[0].gain == 100 && g_calls[0].step == -10 && g_calls[1].plane == 1 && g_calls[1].channel == 0 && g_calls[1].gain == 200 && g_calls[1].step == 20 && g_calls[2].plane == 2 && g_calls[2].channel == 0 && g_calls[2].gain == 300 && g_calls[2].step == -30 && g_calls[3].plane == 3 && g_calls[3].channel == 0 && g_calls[3].gain == 400 && g_calls[3].step == 40;
    m.Init(2); m.MixQuad16Ramped(src, gains, steps);
    ok = ok && g_n == 4 && g_calls[0].plane == 0 && g_calls[0].channel == 0 && g_calls[0].gain == 200 && g_calls[0].step == -20 && g_calls[1].plane == 1 && g_calls[1].channel == 1 && g_calls[1].gain == 400 && g_calls[1].step == 40 && g_calls[2].plane == 2 && g_calls[2].channel == 0 && g_calls[2].gain == 600 && g_calls[2].step == -60 && g_calls[3].plane == 3 && g_calls[3].channel == 1 && g_calls[3].gain == 800 && g_calls[3].step == 80;
    m.Init(4); m.MixQuad16Ramped(src, gains, steps);
    ok = ok && g_n == 4 && g_calls[0].plane == 0 && g_calls[0].channel == 0 && g_calls[0].gain == 400 && g_calls[0].step == -40 && g_calls[1].plane == 1 && g_calls[1].channel == 1 && g_calls[1].gain == 800 && g_calls[1].step == 80 && g_calls[2].plane == 2 && g_calls[2].channel == 2 && g_calls[2].gain == 1200 && g_calls[2].step == -120 && g_calls[3].plane == 3 && g_calls[3].channel == 3 && g_calls[3].gain == 1600 && g_calls[3].step == 160;
    if (!ok) { std::printf("00c58880_TEST FAIL\n"); return 1; }
    std::printf("00c58880_TEST PASS\n");
    return 0;
}