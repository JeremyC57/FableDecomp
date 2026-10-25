// Behaviour test for LinearRampStep (0x00C59D40): (to - from) / samples, the whole step for a zero-length ramp.
#include <cstdio>
static int g_calls = 0;
static long SecondsToMixBlocks(float seconds) { ++g_calls; return (long)(seconds * 100.0f + 0.5f); }   // stub: 100 samples/s
static float LinearRampStep(float from, float to, float seconds)
{
    seconds = (float)SecondsToMixBlocks(seconds);
    if (seconds != 0.0f)
        return (to - from) / seconds;
    return to - from;
}
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d <= 1e-6f; }
int main()
{
    int ok = Near(LinearRampStep(0.0f, 1.0f, 1.0f), 0.01f)       // attack 0 -> 1 over 100 samples
          && Near(LinearRampStep(1.0f, 0.5f, 0.5f), -0.01f)      // falling ramp over 50 samples
          && Near(LinearRampStep(0.25f, 1.0f, 0.0f), 0.75f)      // zero samples: the whole step at once
          && g_calls == 3;
    if (!ok) { std::printf("00c59d40_TEST FAIL\n"); return 1; }
    std::printf("00c59d40_TEST PASS\n");
    return 0;
}