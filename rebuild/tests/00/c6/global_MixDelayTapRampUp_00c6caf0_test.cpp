#include <cstdio>
struct SDelayTap {                        // 8 bytes, four per tap set (the effect's +0x1dc / +0x1e0 sets)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step per sample (ramps)
};
void MixDelayTapRampUp(const short* source, const SDelayTap* tap, long* accumulator)
{
    short step = tap->m_step;
    if (step == 0)
        return;
    const short* samples = source - tap->m_delay;
    long gain = 0;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}static short g_history[256];
static long g_acc[128], g_ref[128];
static void fill()
{
    for (int i = 0; i < 256; ++i) g_history[i] = (short)((i * 977) % 32768 - 16384);
    for (int i = 0; i < 128; ++i) { g_acc[i] = i * 3 - 100; g_ref[i] = g_acc[i]; }
}
static int same() { for (int i = 0; i < 128; ++i) if (g_acc[i] != g_ref[i]) return 0; return 1; }
static int fail() { std::printf("00c6caf0_TEST FAIL\n"); return 1; }
int main()
{
    fill();
    SDelayTap tap = { 40, 0, 0 };
    MixDelayTapRampUp(g_history + 128, &tap, g_acc);         // zero step: untouched
    if (!same()) return fail();
    tap.m_step = 200;
    MixDelayTapRampUp(g_history + 128, &tap, g_acc);
    long g = 0;
    for (int i = 0; i < 128; ++i) { g_ref[i] += (g_history[128 - 40 + i] * g) >> 15; g += 200; }
    if (!same()) return fail();
    tap.m_step = -150; tap.m_delay = 100;
    MixDelayTapRampUp(g_history + 128, &tap, g_acc);
    g = 0;
    for (int i = 0; i < 128; ++i) { g_ref[i] += (g_history[28 + i] * g) >> 15; g -= 150; }
    if (!same()) return fail();
    std::printf("00c6caf0_TEST PASS\n");
    return 0;
}