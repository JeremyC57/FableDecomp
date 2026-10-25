#include <cstdio>
#include <cstring>
struct CAudioMaster {                                  // libpk's master object (global 0x0143BC2C)
    unsigned char _pad000[0x1bc];
    unsigned long m_outputChannels;                    // +0x1bc (4 and up: the rear pair is mixed too)
};
extern CAudioMaster* g_pAudioMaster;                   // 0x0143BC2C
struct SResampleVoice {                                // 0x124-byte voice record (two per set, sets at effect +0x64c / +0x650)
    unsigned char  _pad000[0x110];
    unsigned long  m_phase;                            // +0x110
    unsigned char  _pad114[0x4];
    long           m_offset;                           // +0x118 samples back from the block start
    unsigned long  m_increment;                        // +0x11c
    unsigned char  m_muted;                            // +0x120
    unsigned char  m_swapOutputs;                      // +0x121
    unsigned short m_pan;                              // +0x122
};
void ResampleStereoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                           long* first, long* second, unsigned short pan, long startGain, long gainStep);   // 0x00C7AB20
void ResampleMonoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                         long* output, long startGain, long gainStep);                                      // 0x00C79440
void ResampleStereoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                       long* first, long* second, unsigned short pan);                                     // 0x00C79FD0
void ResampleMonoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                     long* output);                                                                         // 0x00C78A10
void MixVoice(const short* buffer, unsigned long count, const SResampleVoice* voice, long* front, long* rear)
{
    if (voice->m_muted)
        return;
    if (g_pAudioMaster->m_outputChannels >= 4) {
        if (voice->m_swapOutputs)
            ResampleStereoMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, rear, front, voice->m_pan);
        else
            ResampleStereoMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, front, rear, voice->m_pan);
    } else
        ResampleMonoMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, front);
}static CAudioMaster g_master;
CAudioMaster* g_pAudioMaster = &g_master;
static int g_which; static const short* g_src; static unsigned long g_phase, g_inc, g_count;
static long* g_first; static long* g_second; static unsigned short g_pan; static long g_start, g_step;
void ResampleStereoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                           long* first, long* second, unsigned short pan, long startGain, long gainStep)
{ g_which = 1; g_src = source; g_phase = phase; g_inc = increment; g_count = count; g_first = first; g_second = second; g_pan = pan; g_start = startGain; g_step = gainStep; }
void ResampleMonoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                         long* output, long startGain, long gainStep)
{ g_which = 2; g_src = source; g_phase = phase; g_inc = increment; g_count = count; g_first = output; g_second = 0; g_start = startGain; g_step = gainStep; }
void ResampleStereoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                       long* first, long* second, unsigned short pan)
{ g_which = 3; g_src = source; g_phase = phase; g_inc = increment; g_count = count; g_first = first; g_second = second; g_pan = pan; }
void ResampleMonoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                     long* output)
{ g_which = 4; g_src = source; g_phase = phase; g_inc = increment; g_count = count; g_first = output; g_second = 0; }
static int fail() { std::printf("00c70390_TEST FAIL\n"); return 1; }
int main()
{
    static short history[256];
    static long front[128], rear[128];
    static SResampleVoice v;
    std::memset(&v, 0, sizeof(v));
    v.m_phase = 7; v.m_increment = 9; v.m_offset = 3; v.m_pan = 0x4000;
    g_master.m_outputChannels = 6;
    v.m_muted = 1;                                      // muted: nothing
    MixVoice(history + 128, 0x20, &v, front, rear);
    if (g_which != 0) return fail();
    v.m_muted = 0;
    MixVoice(history + 128, 0x20, &v, front, rear);
    if (g_which != 3 || g_src != history + 125 || g_count != 0x20 || g_first != front || g_second != rear || g_pan != 0x4000) return fail();
    v.m_swapOutputs = 1;
    MixVoice(history + 128, 0x10, &v, front, rear);
    if (g_which != 3 || g_first != rear || g_second != front || g_count != 0x10) return fail();
    g_master.m_outputChannels = 1;
    MixVoice(history + 128, 0x20, &v, front, rear);
    if (g_which != 4 || g_first != front || g_phase != 7 || g_inc != 9) return fail();
    std::printf("00c70390_TEST PASS\n");
    return 0;
}