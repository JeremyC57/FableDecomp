// Behaviour test for 0x0070BE40 CAnimateThing::GetProgressFromAlpha.
#include <cstdio>

static float g_epsilon = 0.0001f;
static float GetProgressFromAlpha(float alpha, float totalAlpha)
{
    if (totalAlpha >= g_epsilon && g_epsilon + 1.0f >= totalAlpha)
        return alpha / totalAlpha;
    return 1.0f;
}

int main()
{
    bool ok = GetProgressFromAlpha(0.25f, 0.5f) == 0.5f
           && GetProgressFromAlpha(0.3f, 0.0f) == 1.0f           // below epsilon
           && GetProgressFromAlpha(0.3f, 2.0f) == 1.0f           // above 1 + epsilon
           && GetProgressFromAlpha(0.5f, 1.0f) == 0.5f
           && GetProgressFromAlpha(0.0f, 0.0001f) == 0.0f;       // at the lower bound
    if (!ok) { std::printf("0070be40_TEST FAIL\n"); return 1; }
    std::printf("0070be40_TEST PASS\n");
    return 0;
}