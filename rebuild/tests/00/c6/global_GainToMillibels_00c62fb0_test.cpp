// behaviour test for 0x00C62FB0 GainToMillibels: 2000*log10(max(gain, 1e-5))
#include <cstdio>
#include <cmath>
static float GainToMillibels(float gain)
{
    float clamped = gain > 1e-5f ? gain : 1e-5f;
    return (float)std::log(clamped) * 868.589f;
}
int main()
{
    if (std::fabs(GainToMillibels(1.0f)) > 1e-3f || std::fabs(GainToMillibels(0.1f) + 2000.0f) > 0.1f
        || std::fabs(GainToMillibels(0.5f) + 602.06f) > 0.1f || std::fabs(GainToMillibels(0.0f) + 10000.0f) > 0.5f
        || std::fabs(GainToMillibels(-3.0f) + 10000.0f) > 0.5f || std::fabs(GainToMillibels(10.0f) - 2000.0f) > 0.1f)
    { std::printf("00c62fb0_TEST FAIL\n"); return 1; }
    std::printf("00c62fb0_TEST PASS\n");
    return 0;
}