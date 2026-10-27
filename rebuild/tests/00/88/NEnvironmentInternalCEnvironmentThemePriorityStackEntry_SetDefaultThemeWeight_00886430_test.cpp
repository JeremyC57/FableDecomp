// Behaviour test for 0x00886430 CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight(float, bool): clamp to
// [0, 1], store, lock, and rebalance only when asked.
#include <cstdio>

class StackEntry {
public:
    float BaseWeight; bool BaseWeightLocked; int rebalances;
    void RebalanceWeights() { ++rebalances; }
    void SetDefaultThemeWeight(float weight, bool rebalance)
    {
        if (weight < 0.0f) weight = 0.0f;
        else if (weight > 1.0f) weight = 1.0f;
        BaseWeight = weight;
        BaseWeightLocked = true;
        if (rebalance) RebalanceWeights();
    }
};

int main()
{
    StackEntry e; e.BaseWeight = 0.5f; e.BaseWeightLocked = false; e.rebalances = 0;
    e.SetDefaultThemeWeight(-3.0f, false);
    if (e.BaseWeight != 0.0f || !e.BaseWeightLocked || e.rebalances != 0) { std::printf("00886430_TEST FAIL\n"); return 1; }
    e.SetDefaultThemeWeight(7.0f, true);
    if (e.BaseWeight != 1.0f || e.rebalances != 1) { std::printf("00886430_TEST FAIL\n"); return 1; }
    e.SetDefaultThemeWeight(0.25f, true);
    if (e.BaseWeight != 0.25f || e.rebalances != 2) { std::printf("00886430_TEST FAIL\n"); return 1; }
    std::printf("00886430_TEST PASS\n");
    return 0;
}