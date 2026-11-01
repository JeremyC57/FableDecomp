// Behaviour test for 0x00C6CC60 CVtable_012bed7c::Process (libpk multi-tap delay, C path)
#include <cstdio>
#include <cstring>
struct SDelayTap { long m_delay; unsigned short m_gain; short m_step; };
struct SRecord { SDelayTap m_taps[6]; };
static unsigned long gOutputChannels = 2;
static void MixDelayTapRampUp(const short* source, const SDelayTap* tap, long* acc)
{ short step = tap->m_step; if (step == 0) return; const short* s = source - tap->m_delay; long g = 0;
  for (int i = 0; i < 128; ++i) { acc[i] += (*s++ * g) >> 15; g += step; } }
static void MixDelayTapRampDown(const short* source, const SDelayTap* tap, long* acc)
{ unsigned short start = tap->m_gain; if (start == 0) return; short step = -tap->m_step; const short* s = source - tap->m_delay; long g = start;
  for (int i = 0; i < 128; ++i) { acc[i] += (*s++ * g) >> 15; g += step; } }
static void MixDelayTap(const short* source, unsigned long count, const SDelayTap* tap, long* acc)
{ unsigned short g = tap->m_gain; if (g == 0) return; const short* s = source - tap->m_delay; for (unsigned long i = 0; i < count; ++i) *acc++ += (*s++ * g) >> 15; }
struct CEffect {
    bool m_crossfade; unsigned char* m_pAlignedBuffer; SRecord m_first, m_second; SRecord* m_pCurrent; SRecord* m_pPrevious;
    void Process(const short* input, long* const* outputs)
    {
        memcpy(m_pAlignedBuffer, input, 128 * sizeof(short));
        if (m_crossfade) {
            m_crossfade = false;
            MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
            MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
            if (gOutputChannels >= 4) {
                MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
                MixDelayTapRampDown((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
            }
            SRecord* current = m_pCurrent; m_pCurrent = m_pPrevious; m_pPrevious = current;
            MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[0], outputs[0]);
            MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[1], outputs[1]);
            if (gOutputChannels >= 4) {
                MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[2], outputs[2]);
                MixDelayTapRampUp((const short*)m_pAlignedBuffer, &m_pCurrent->m_taps[3], outputs[3]);
            }
        } else {
            MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[0], outputs[0]);
            MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[1], outputs[1]);
            if (gOutputChannels >= 4) {
                MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[2], outputs[2]);
                MixDelayTap((const short*)m_pAlignedBuffer, 128, &m_pCurrent->m_taps[3], outputs[3]);
            }
        }
        memcpy(m_pAlignedBuffer - 0x40, m_pAlignedBuffer + 0xc0, 0x40);
    }
};
int main()
{
    static unsigned char storage[0x40 + 256];
    static long out0[128], out1[128], out2[128], out3[128];
    long* outs[4] = { out0, out1, out2, out3 };
    CEffect e; memset(&e, 0, sizeof(e));
    e.m_pAlignedBuffer = storage + 0x40; e.m_pCurrent = &e.m_first; e.m_pPrevious = &e.m_second;
    e.m_first.m_taps[0].m_delay = 0;  e.m_first.m_taps[0].m_gain = 0x4000;
    e.m_first.m_taps[1].m_delay = 32; e.m_first.m_taps[1].m_gain = 0x2000;
    short in[128]; for (int i = 0; i < 128; ++i) in[i] = (short)(i * 100);
    e.Process(in, outs);
    // tap 0: half gain, no delay
    if (out0[10] != (1000 * 0x4000) >> 15 || out0[127] != (12700 * 0x4000) >> 15) { std::printf("00c6cc60_TEST FAIL\n"); return 1; }
    // tap 1 reads 32 samples back: the first 32 come from the (zero) history, then input[i-32]
    if (out1[5] != 0 || out1[40] != (800 * 0x2000) >> 15) { std::printf("00c6cc60_TEST FAIL\n"); return 1; }
    // the last 32 samples slid below the aligned buffer
    if (((short*)storage)[0] != 9600 || ((short*)storage)[31] != 12700) { std::printf("00c6cc60_TEST FAIL\n"); return 1; }
    if (out2[0] != 0) { std::printf("00c6cc60_TEST FAIL\n"); return 1; }
    // cross-fade swaps the tap sets
    e.m_crossfade = true; e.m_second.m_taps[0].m_step = 0x100;
    e.Process(in, outs);
    if (e.m_pCurrent != &e.m_second || e.m_crossfade) { std::printf("00c6cc60_TEST FAIL\n"); return 1; }
    std::printf("00c6cc60_TEST PASS\n");
    return 0;
}