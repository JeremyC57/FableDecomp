// Behaviour test for OctaveRampStep (0x00C59DE0): 2^((to - from) / blocks), the whole 2^(to - from) for zero blocks;
// raising the step to the block count spans the octave distance.
#include <cstdio>
#include <math.h>
static long SecondsToMixBlocks(float seconds) { return (long)(seconds * 10.0f + 0.5f); }   // stub: 10 blocks/s
static float OctaveRampStep(float from, float to, float seconds)
{
    long blocks = SecondsToMixBlocks(seconds);
    to -= from;
    if (blocks != 0) {
        float step = (float)pow(2.0, (double)(to / blocks));
        return step;
    }
    float whole = (float)pow(2.0, (double)to);
    return whole;
}
static bool Near(double a, double b, double tol) { double d = a - b; if (d < 0) d = -d; return d <= tol; }
int main()
{
    float k = OctaveRampStep(0.0f, 1.0f, 2.0f);                 // one octave up over 20 blocks
    double v = 1.0; for (int i = 0; i < 20; ++i) v *= k;
    int ok = Near(v, 2.0, 1e-5) && Near(OctaveRampStep(1.0f, -1.0f, 0.0f), 0.25, 1e-7)   // zero blocks: 2^-2
          && Near(OctaveRampStep(0.5f, 0.5f, 1.0f), 1.0, 1e-7);                           // no change
    if (!ok) { std::printf("00c59de0_TEST FAIL\n"); return 1; }
    std::printf("00c59de0_TEST PASS\n");
    return 0;
}