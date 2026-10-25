// Behaviour test for ExponentialDecayStep (0x00C59D80): the per-sample factor that decays 32768:1 over the length;
// raising it to the sample count gives 1/32768; zero samples gives 0 (instant cut).
#include <cstdio>
#include <math.h>
static long SecondsToMixBlocks(float seconds) { return (long)(seconds * 1000.0f + 0.5f); }   // stub: 1000 samples/s
static float ExponentialDecayStep(float from, float seconds)
{
    (void)from;
    seconds = (float)SecondsToMixBlocks(seconds);
    if (seconds != 0.0f)
        return (float)exp(log(32768.0) * (-1.0f / seconds));
    return 0.0f;
}
static bool Near(double a, double b, double tol) { double d = a - b; if (d < 0) d = -d; return d <= tol; }
int main()
{
    float k = ExponentialDecayStep(1.0f, 0.25f);                  // 250 samples
    double level = 1.0; for (int i = 0; i < 250; ++i) level *= k;
    int ok = k > 0.0f && k < 1.0f && Near(level, 1.0 / 32768.0, 1e-7)
          && Near(ExponentialDecayStep(0.5f, 0.001f), exp(-log(32768.0)), 1e-6)   // 1 sample: straight to 1/32768
          && ExponentialDecayStep(1.0f, 0.0f) == 0.0f;
    if (!ok) { std::printf("00c59d80_TEST FAIL\n"); return 1; }
    std::printf("00c59d80_TEST PASS\n");
    return 0;
}