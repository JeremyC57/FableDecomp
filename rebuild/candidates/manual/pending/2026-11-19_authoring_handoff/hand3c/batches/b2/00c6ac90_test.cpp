// Behaviour test for CSoundMixer::MixBlockRamped (0x00C6AC90): a copy of the row's logic on stand-alone types -- the mixed flag is set
// and one block of 16 quads (128 samples) is scaled by a 4-lane volume ramp starting at `initial` (lane i = initial + i*step, each
// lane moving by 4*step per quad, saturating) and added (high word of the product, doubled) into the mixer's accumulator.
#include <stdio.h>
static int g_calls; static unsigned long g_count;
static void Kernel(const short* src, unsigned long count, long* dst, short initial, short step)
{
    ++g_calls; g_count = count;
    int gains[4]; for (int i = 0; i < 4; ++i) gains[i] = (short)(initial + i * step);
    for (unsigned long block = 0; block < count * 2; ++block) {
        for (int lane = 0; lane < 4; ++lane) { int product = (int)src[block * 4 + lane] * gains[lane]; dst[block * 4 + lane] += (product >> 16) * 2; }
        for (int lane = 0; lane < 4; ++lane) { int next = gains[lane] + (short)(step * 4); gains[lane] = next > 32767 ? 32767 : (next < -32768 ? -32768 : next); }
    }
}
struct Mixer {
    unsigned char m_bMixed; long* m_pAccumulator;
    void MixBlockRamped(const short* source, short initial, short step) { m_bMixed = 1; Kernel(source, 0x10, m_pAccumulator, initial, step); }
};
int main()
{
    int fails = 0;
    static short src[128 * 2]; static long acc[128 * 2];
    for (int i = 0; i < 256; ++i) src[i] = 16384;
    Mixer m = { 0, acc };
    m.MixBlockRamped(src, 8192, 4);
    if (!m.m_bMixed || g_calls != 1 || g_count != 16) ++fails;
    if (acc[0] != (16384 * 8192 >> 16) * 2) ++fails;                 // lane 0 at the initial gain: 4096
    if (acc[1] != (16384 * 8196 >> 16) * 2) ++fails;                 // lane 1 one step up
    if (acc[4] != (16384 * (8192 + 16) >> 16) * 2) ++fails;          // next quad: + 4*step
    if (acc[127] == 0 || acc[128] != 0) ++fails;                     // exactly 128 samples touched
    printf(fails ? "00c6ac90_TEST FAIL\n" : "00c6ac90_TEST PASS\n");
    return fails ? 1 : 0;
}
