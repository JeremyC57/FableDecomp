// Behaviour test for 0x00C6FE10 CVtable_012b8ac8::Process (libpk MMX gain-ramp mixer): which kernel runs with which gain/step
#include <cstdio>
struct SGainRamp { unsigned short m_gain; short m_step; };
struct Call { int kind; long* dst; int initial; int step; unsigned long count; };
static Call gCalls[16]; static int gN = 0; static unsigned long gOutputChannels = 2;
static void Kernel(const short*, unsigned long count, long* dst, short initial, short step)
{ Call c = { 1, dst, initial, step, count }; gCalls[gN++] = c; }
static void ScaleAccumulate(const short*, unsigned long count, long* dst, short gain)
{ Call c = { 2, dst, gain, 0, count }; gCalls[gN++] = c; }
static void MixGainRampUpMMX(const short* s, const SGainRamp* r, long* a) { if (r->m_step != 0) Kernel(s, 16, a, 0, r->m_step); }
static void MixGainRampDownMMX(const short* s, const SGainRamp* r, long* a) { if (r->m_gain != 0) Kernel(s, 16, a, r->m_gain, -r->m_step); }
static void MixGainMMX(const short* s, unsigned long n, const SGainRamp* r, long* a) { if (r->m_gain != 0) ScaleAccumulate(s, n, a, r->m_gain); }
struct CEffect {
    bool m_crossfade; SGainRamp m_first, m_second; SGainRamp* m_pCurrent; SGainRamp* m_pPrevious;
    void Process(const short* input, long* const* outputs)
    {
        if (m_crossfade) {
            m_crossfade = false;
            MixGainRampDownMMX(input, m_pCurrent, outputs[0]);
            MixGainRampDownMMX(input, m_pCurrent, outputs[1]);
            if (gOutputChannels >= 4) { MixGainRampDownMMX(input, m_pCurrent, outputs[2]); MixGainRampDownMMX(input, m_pCurrent, outputs[3]); }
            SGainRamp* current = m_pCurrent; m_pCurrent = m_pPrevious; m_pPrevious = current;
            MixGainRampUpMMX(input, m_pCurrent, outputs[0]);
            MixGainRampUpMMX(input, m_pCurrent, outputs[1]);
            if (gOutputChannels >= 4) { MixGainRampUpMMX(input, m_pCurrent, outputs[2]); MixGainRampUpMMX(input, m_pCurrent, outputs[3]); }
        } else {
            MixGainMMX(input, 16, m_pCurrent, outputs[0]);
            MixGainMMX(input, 16, m_pCurrent, outputs[1]);
            if (gOutputChannels >= 4) { MixGainMMX(input, 16, m_pCurrent, outputs[2]); MixGainMMX(input, 16, m_pCurrent, outputs[3]); }
        }
    }
};
int main()
{
    static short in[128]; static long o0[128], o1[128], o2[128], o3[128]; long* outs[4] = { o0, o1, o2, o3 };
    CEffect e; e.m_crossfade = false; e.m_first.m_gain = 0x1000; e.m_first.m_step = 0x20; e.m_second.m_gain = 0x3000; e.m_second.m_step = 0x60;
    e.m_pCurrent = &e.m_first; e.m_pPrevious = &e.m_second;
    e.Process(in, outs);
    if (gN != 2 || gCalls[0].kind != 2 || gCalls[0].initial != 0x1000 || gCalls[1].dst != o1 || gCalls[0].count != 16) { std::printf("00c6fe10_TEST FAIL\n"); return 1; }
    gN = 0; gOutputChannels = 4; e.m_crossfade = true;
    e.Process(in, outs);
    if (gN != 8 || gCalls[0].initial != 0x1000 || gCalls[0].step != -0x20 || gCalls[3].dst != o3
        || gCalls[4].initial != 0 || gCalls[4].step != 0x60 || e.m_pCurrent != &e.m_second || e.m_crossfade) { std::printf("00c6fe10_TEST FAIL\n"); return 1; }
    std::printf("00c6fe10_TEST PASS\n");
    return 0;
}