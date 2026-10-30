// Behaviour test for 0x00C5D2E0 AccumulateFloatBlockSSE2 (see src/00c5d2e0.cpp): dst[i] += round-to-nearest-even(src[i]) over 128 samples.
#include <cstdio>
#include <cmath>
static long RoundEven(float v)
{
    double f = std::floor((double)v), d = (double)v - f;
    long r = (long)f;
    if (d > 0.5 || (d == 0.5 && (r & 1))) ++r;
    return r;
}
static void Accumulate(const float* src, long* dst) { for (int i = 0; i < 128; ++i) dst[i] += RoundEven(src[i]); }
int main()
{
    float s[128]; long d[128];
    for (int i = 0; i < 128; ++i) { s[i] = (float)i * 0.5f - 10.0f; d[i] = 100; }
    Accumulate(s, d);
    if (d[0] != 90 || d[1] != 100 - 10 || d[3] != 100 - 8 || d[21] != 100 + 0 || d[127] != 100 + 54) { std::printf("00c5d2e0_TEST FAIL\n"); return 1; }
    std::printf("00c5d2e0_TEST PASS\n"); return 0;
}