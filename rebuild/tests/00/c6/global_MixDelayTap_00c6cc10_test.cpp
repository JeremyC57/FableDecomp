#include <cstdio>
struct SDelayTap {                        // 8 bytes, four per tap set (the effect's +0x1dc / +0x1e0 sets)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step per sample (ramps)
};
void MixDelayTap(const short* source, unsigned long count, const SDelayTap* tap, long* accumulator)
{
    unsigned short gain = tap->m_gain;
    if (gain == 0)
        return;
    long* out = accumulator;
    const short* samples = source - tap->m_delay;
    for (unsigned long i = 0; i < count; ++i)
        *out++ += (*samples++ * gain) >> 15;
}static short g_history[256];
static long g_acc[128], g_ref[128];
static void fill()
{
    for (int i = 0; i < 256; ++i) g_history[i] = (short)((i * 977) % 32768 - 16384);
    for (int i = 0; i < 128; ++i) { g_acc[i] = i * 3 - 100; g_ref[i] = g_acc[i]; }
}
static int same() { for (int i = 0; i < 128; ++i) if (g_acc[i] != g_ref[i]) return 0; return 1; }
static int fail() { std::printf("00c6cc10_TEST FAIL\n"); return 1; }
int main()
{
    fill();
    SDelayTap tap = { 64, 0, 0 };
    MixDelayTap(g_history + 128, 128, &tap, g_acc);          // zero gain: untouched
    if (!same()) return fail();
    tap.m_gain = 40000;                                       // above 0x7fff: the unsigned word is kept
    MixDelayTap(g_history + 128, 100, &tap, g_acc);
    for (int i = 0; i < 100; ++i) g_ref[i] += (g_history[64 + i] * 40000L) >> 15;
    if (!same()) return fail();
    MixDelayTap(g_history + 128, 0, &tap, g_acc);            // empty block
    if (!same()) return fail();
    std::printf("00c6cc10_TEST PASS\n");
    return 0;
}