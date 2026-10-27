// Behaviour test for 0x00C58420 CMixBuffer::MixStereo16Ramped
#include <cstdio>
struct Call { const short* src; int ch; unsigned short gain; short step; };
static Call g_calls[8]; static int g_n = 0;
struct Mix {
    int m_chan[6]; unsigned int m_numChannels;
    void MixAdd16Ramp(const short* src, int ch, unsigned short gain, short step) { Call c = { src, ch, gain, step }; g_calls[g_n++] = c; }
    void MixStereo16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps)
    {
        if (m_numChannels == 1) {
            MixAdd16Ramp(src, 0, gains[0] >> 1, (short)steps[0] / 2);
            MixAdd16Ramp(src + 128, 0, gains[1] >> 1, (short)steps[1] / 2);
            return;
        }
        MixAdd16Ramp(src, 0, gains[0], steps[0]);
        MixAdd16Ramp(src + 128, 1, gains[1], steps[1]);
        if (m_numChannels >= 4) { MixAdd16Ramp(src, 2, gains[2], steps[2]); MixAdd16Ramp(src + 128, 3, gains[3], steps[3]); }
    }
};
static int Fail() { std::printf("00c58420_TEST FAIL\n"); return 1; }
int main()
{
    short src[256]; unsigned short gains[4] = { 0x8000, 0x4000, 0x2000, 0x1000 }; unsigned short steps[4] = { 0xfffd, 7, 1, 2 };
    Mix m; m.m_numChannels = 1;
    m.MixStereo16Ramped(src, gains, steps);       // mono: both halves into channel 0 at half gain, signed half step
    if (g_n != 2 || g_calls[0].gain != 0x4000 || g_calls[0].step != -1 || g_calls[1].src != src + 128 || g_calls[1].ch != 0 || g_calls[1].step != 3) return Fail();
    g_n = 0; m.m_numChannels = 2; m.MixStereo16Ramped(src, gains, steps);
    if (g_n != 2 || g_calls[1].ch != 1 || g_calls[0].gain != 0x8000) return Fail();
    g_n = 0; m.m_numChannels = 6; m.MixStereo16Ramped(src, gains, steps);
    if (g_n != 4 || g_calls[3].ch != 3 || g_calls[3].src != src + 128 || g_calls[2].gain != 0x2000) return Fail();
    std::printf("00c58420_TEST PASS\n");
    return 0;
}