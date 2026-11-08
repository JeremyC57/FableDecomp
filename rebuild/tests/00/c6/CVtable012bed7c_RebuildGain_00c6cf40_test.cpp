// Behaviour test for CVtable_012bed7c::RebuildGain (0x00C6CF40): under the lock, record the cross-fade flag and rebuild the
// selected tap set (the +0x1E0 set when cross-fading, else the live +0x1DC set): taps 0/1 always, taps 2/3 only with four or
// more output channels; each tap = delay, Q15 gain FtoL(gain * volume * 32767) capped at 0x7fff, step gain >> 7.
#include <cstdio>
#include <cmath>
static long FtoL(float f) { return (long)std::floor((double)f + 0.5); }   // fistp rounds to nearest (no ties in the inputs)
static int g_enter = 0, g_leave = 0;
struct CAudioMaster { unsigned long m_outputChannels; };
static CAudioMaster g_master;
static CAudioMaster* g_pAudioMaster = &g_master;
struct SDelayTap { long m_delay; unsigned short m_gain; short m_step; };
struct CEffect {
    float m_volume; bool m_crossfade;
    SDelayTap m_first[6], m_second[6];
    SDelayTap* m_pCurrent; SDelayTap* m_pPrevious;
    float m_gains[6]; long m_delays[6];
    void ComputeTapGain(SDelayTap* tap, float scale, long delay)
    {
        unsigned long gain = (unsigned long)FtoL(scale * m_volume * 32767.0f);
        if (gain > 0x7fff) gain = 0x7fff;
        tap->m_gain = (unsigned short)gain; tap->m_delay = delay; tap->m_step = (short)((unsigned short)gain >> 7);
    }
    void RebuildGain(bool crossfade)
    {
        ++g_enter;
        m_crossfade = crossfade;
        SDelayTap* set = crossfade ? m_pPrevious : m_pCurrent;
        ComputeTapGain(&set[0], m_gains[0], m_delays[0]);
        ComputeTapGain(&set[1], m_gains[1], m_delays[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            ComputeTapGain(&set[2], m_gains[2], m_delays[2]);
            ComputeTapGain(&set[3], m_gains[3], m_delays[3]);
        }
        ++g_leave;
    }
};
int main()
{
    CEffect e = {};
    e.m_pCurrent = e.m_first; e.m_pPrevious = e.m_second;
    e.m_volume = 0.5f;
    float g[6] = { 1.0f, 0.25f, 3.0f, 0.1f, 0, 0 }; long d[6] = { 10, 20, 30, 40, 0, 0 };
    for (int i = 0; i < 6; ++i) { e.m_gains[i] = g[i]; e.m_delays[i] = d[i]; }
    bool ok = true;
    g_master.m_outputChannels = 2;
    e.RebuildGain(false);
    ok = ok && !e.m_crossfade && e.m_first[0].m_delay == 10 && e.m_first[0].m_gain == 16384 && e.m_first[0].m_step == 128;
    ok = ok && e.m_first[1].m_delay == 20 && e.m_first[1].m_gain == 4096 && e.m_first[1].m_step == 32;
    ok = ok && e.m_first[2].m_delay == 0 && e.m_second[0].m_delay == 0;
    g_master.m_outputChannels = 4;
    e.RebuildGain(true);
    ok = ok && e.m_crossfade && e.m_second[0].m_delay == 10 && e.m_second[2].m_delay == 30 && e.m_second[2].m_gain == 0x7fff
            && e.m_second[2].m_step == 0xff && e.m_second[3].m_gain == 1638 && e.m_second[3].m_delay == 40;
    ok = ok && g_enter == 2 && g_leave == 2;
    if (!ok) { std::printf("00c6cf40_TEST FAIL\n"); return 1; }
    std::printf("00c6cf40_TEST PASS\n");
    return 0;
}