#include <cstdio>
// Behaviour test for 00e7f7d0: GetNextPrizeCategoryWeight returns FishPrizeWeights[PrizeNumber + 1], or entry 1 while PrizeNumber == -1.
static float g_weights[5] = { 1.5f, 2.5f, 4.0f, 6.5f, 9.0f };
class MockScriptDef { public: float* FishPrizeWeights; };   // host mock (NF162: renamed off the shared-header class name)
static MockScriptDef g_def = { g_weights };
static MockScriptDef* g_pScriptDef = &g_def;
struct Script {
    long PrizeNumber;
    float GetNextPrizeCategoryWeight()
    {
        long next = PrizeNumber + 1;
        if (PrizeNumber == -1)
            ++next;
        return g_pScriptDef->FishPrizeWeights[next];
    }
};
static int near_(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    Script s;
    s.PrizeNumber = -1; if (!near_(s.GetNextPrizeCategoryWeight(), 2.5f)) { std::printf("00e7f7d0_TEST FAIL\n"); return 1; }
    s.PrizeNumber = 0;  if (!near_(s.GetNextPrizeCategoryWeight(), 2.5f)) { std::printf("00e7f7d0_TEST FAIL\n"); return 1; }
    s.PrizeNumber = 2;  if (!near_(s.GetNextPrizeCategoryWeight(), 6.5f)) { std::printf("00e7f7d0_TEST FAIL\n"); return 1; }
    s.PrizeNumber = 3;  if (!near_(s.GetNextPrizeCategoryWeight(), 9.0f)) { std::printf("00e7f7d0_TEST FAIL\n"); return 1; }
    std::printf("00e7f7d0_TEST PASS\n");
    return 0;
}