// Test for CEngineWeatherRenderer::FadeSnowToTarget (00b52900): snap to the target within 0.01, else step 0.01 toward it, and
// hand the result to StartSnowing. Both retail bodies read the CURRENT RAIN strength (+0x5C8).
#include <cstdio>
#include <cmath>
struct Renderer {
    float CurrentRainStrength, CurrentSnowStrength;
    float applied; int calls;
    void StartSnowing(float s) { applied = s; ++calls; }
    void FadeSnowToTarget(float target)
    {
        float strength = CurrentRainStrength;
        if ((float)std::fabs(strength - target) < 0.01f) strength = target;
        else if (strength > target) strength -= 0.01f;
        else if (strength < target) strength += 0.01f;
        StartSnowing(strength);
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    Renderer r; r.CurrentRainStrength = 0.5f; r.CurrentSnowStrength = 0.9f; r.calls = 0;
    r.FadeSnowToTarget(0.505f); if (!Near(r.applied, 0.505f)) { std::printf("00b52900_TEST FAIL\n"); return 1; }
    r.FadeSnowToTarget(0.2f);   if (!Near(r.applied, 0.49f))  { std::printf("00b52900_TEST FAIL\n"); return 1; }
    r.FadeSnowToTarget(0.8f);   if (!Near(r.applied, 0.51f))  { std::printf("00b52900_TEST FAIL\n"); return 1; }
    if (r.calls != 3) { std::printf("00b52900_TEST FAIL\n"); return 1; }
    std::printf("00b52900_TEST PASS\n");
    return 0;
}