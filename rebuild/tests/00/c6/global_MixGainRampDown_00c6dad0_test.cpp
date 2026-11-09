// Behaviour test for MixGainRampDown 0x00C6DAD0: a zero start gain leaves the accumulator untouched; otherwise every one of the
// 128 samples adds (source * gain) >> 15 with the gain starting at the record gain and falling by the step after each sample (negative
// samples and a negative step included), and nothing past the block is written.
#include <cstdio>
#include <cmath>

struct SGainRamp { unsigned short m_gain; short m_step; };

void MixGainRampDown(const short* source, const SGainRamp* ramp, long* accumulator)
{
    unsigned short start = ramp->m_gain;
    if (start == 0)
        return;
    short step = -ramp->m_step;
    long gain = start;
    for (int i = 0; i < 128; ++i) {
        const int sample = i;
        accumulator[sample] += (source[i] * gain) >> 15;
        gain += step;
    }
}

static int Check(unsigned short start, short step, int seed)
{
    short source[128]; long acc[129], expect[129];
    for (int i = 0; i < 128; ++i) source[i] = (short)(((i * 977 + seed * 131) % 65536) - 32768);
    for (int i = 0; i < 129; ++i) acc[i] = expect[i] = 1000 * i - 50000 + seed;
    SGainRamp ramp; ramp.m_gain = start; ramp.m_step = step;
    if (start != 0) {
        long gain = start;
        for (int i = 0; i < 128; ++i) { double p = (double)source[i] * (double)gain; expect[i] += (long)std::floor(p / 32768.0); gain -= step; }
    }
    MixGainRampDown(source, &ramp, acc);
    for (int i = 0; i < 129; ++i)
        if (acc[i] != expect[i]) { std::printf("FAIL start %u step %d seed %d at %d: %ld != %ld\n", start, step, seed, i, acc[i], expect[i]); return 1; }
    return 0;
}

int main()
{
    int fails = 0;
    fails += Check(0, 256, 1);
    fails += Check(32768, 256, 2);
    fails += Check(32768, 1, 3);
    fails += Check(100, 200, 4);
    fails += Check(20000, 0, 5);
    if (fails) return 1;
    std::printf("00c6dad0_TEST PASS\n");
    return 0;
}