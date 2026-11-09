// Behaviour test for MixGainRampUp 0x00C6E790: a zero step leaves the accumulator untouched; otherwise every one of the
// 128 samples adds (source * gain) >> 15 with the gain starting at 0 and rising by the step after each sample (negative
// samples and a negative step included), and nothing past the block is written.
#include <cstdio>
#include <cmath>

struct SGainRamp { unsigned short m_gain; short m_step; };

void MixGainRampUp(const short* source, const SGainRamp* ramp, long* accumulator)
{
    short step = ramp->m_step;
    if (step == 0)
        return;
    long gain = 0;
    for (int i = 0; i < 128; ++i) {
        const int sample = i;
        accumulator[sample] += (source[i] * gain) >> 15;
        gain += step;
    }
}

static int Check(short step, int seed)
{
    short source[128]; long acc[129], expect[129];
    for (int i = 0; i < 128; ++i) source[i] = (short)(((i * 977 + seed * 131) % 65536) - 32768);
    for (int i = 0; i < 129; ++i) acc[i] = expect[i] = 1000 * i - 50000 + seed;
    SGainRamp ramp; ramp.m_gain = 12345; ramp.m_step = step;
    if (step != 0) {
        long gain = 0;
        for (int i = 0; i < 128; ++i) { double p = (double)source[i] * (double)gain; expect[i] += (long)std::floor(p / 32768.0); gain += step; }
    }
    MixGainRampUp(source, &ramp, acc);
    for (int i = 0; i < 129; ++i)
        if (acc[i] != expect[i]) { std::printf("FAIL step %d seed %d at %d: %ld != %ld\n", step, seed, i, acc[i], expect[i]); return 1; }
    return 0;
}

int main()
{
    int fails = 0;
    fails += Check(0, 1);
    fails += Check(256, 2);
    fails += Check(1, 3);
    fails += Check(-200, 4);
    fails += Check(100, 5);
    if (fails) return 1;
    std::printf("00c6e790_TEST PASS\n");
    return 0;
}