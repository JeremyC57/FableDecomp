// Behaviour test for 0x0070BD80 GFGetInterpolatedLoopingValue (directional wrap-around interpolation).
#include <cstdio>
#include <cmath>

static float CalcValue(bool forwards, float last, float current, float alpha, float range)
{
    if (forwards) {
        if (current >= last)
            return (current - last) * alpha + last;
        return (float)std::fmod((double)((range - last + current) * alpha + last), 1.0);
    }
    if (current <= last)
        return last - (last - current) * alpha;
    float value = last - (range - current + last) * alpha;
    while (value < 0.0f)
        value += range;
    return value;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    bool ok = Near(CalcValue(true, 0.25f, 0.75f, 0.5f, 1.0f), 0.5f)          // plain forward lerp
           && Near(CalcValue(true, 0.75f, 0.25f, 0.5f, 1.0f), 0.0f)          // forward wrap: 0.75 + 0.25 = 1.0 -> fmod -> 0
           && Near(CalcValue(true, 0.8f, 0.2f, 0.25f, 1.0f), 0.9f)           // 0.8 + 0.4 * 0.25
           && Near(CalcValue(false, 0.75f, 0.25f, 0.5f, 1.0f), 0.5f)         // plain backward lerp
           && Near(CalcValue(false, 0.4f, 0.8f, 0.5f, 1.0f), 0.1f)           // 0.4 - (0.2 + 0.4) * 0.5
           && Near(CalcValue(false, 0.1f, 0.9f, 1.0f, 1.0f), 0.9f);          // 0.1 - 0.2 = -0.1 -> + range
    if (!ok) { std::printf("0070bd80_TEST FAIL\n"); return 1; }
    std::printf("0070bd80_TEST PASS\n");
    return 0;
}