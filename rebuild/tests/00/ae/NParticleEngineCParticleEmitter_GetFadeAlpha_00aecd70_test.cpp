// behaviour test for 0x00AECD70 NParticleEngine::CParticleEmitter::GetFadeAlpha(float distance) const: 0 outside [FadeInStart, MaxDrawDistance], ramp up to FadeInEnd, 1 until FadeOutStart, ramp down to MaxDrawDistance
#include <cstdio>
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
struct SEmitter {
    float MaxDrawDistance, FadeOutStart, FadeInStart, FadeInEnd;
    float GetFadeAlpha(float distance) const
    {
        if (distance < FadeInStart || distance > MaxDrawDistance)
            return 0.0f;
        if (distance < FadeInEnd)
            return (distance - FadeInStart) / (FadeInEnd - FadeInStart);
        if (distance < FadeOutStart)
            return 1.0f;
        return 1.0f - (distance - FadeOutStart) / (MaxDrawDistance - FadeOutStart);
    }
};
int main()
{

    SEmitter e;
    e.FadeInStart = 10.0f; e.FadeInEnd = 20.0f; e.FadeOutStart = 80.0f; e.MaxDrawDistance = 100.0f;
    float d[9] = { 5.0f, 10.0f, 15.0f, 20.0f, 50.0f, 80.0f, 90.0f, 100.0f, 120.0f };
    float x[9] = { 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f, 0.0f };
    for (int i = 0; i < 9; ++i) if (!Near(e.GetFadeAlpha(d[i]), x[i])) { std::printf("00aecd70_TEST FAIL\n"); return 1; }
    std::printf("00aecd70_TEST PASS\n");
    return 0;
}