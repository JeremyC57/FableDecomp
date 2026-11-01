// Behaviour test for CEnvironmentThemePriorityStack::ProcessTransitions (0x00887B00): process every entry's transitions (the
// count re-read each pass), then give entry 0 a locked zero base weight and rebalance it.
#include <cstdio>
struct CEntryModel {
    float BaseWeight; bool BaseWeightLocked; int Processed; int Rebalanced;
    void RebalanceWeights() { ++Rebalanced; }
    void ProcessTransitions() { ++Processed; }
    void SetDefaultThemeWeight(float weight, bool rebalance)
    {
        if (weight < 0.0f) weight = 0.0f; else if (weight > 1.0f) weight = 1.0f;
        BaseWeight = weight;
        BaseWeightLocked = true;
        if (rebalance) RebalanceWeights();
    }
};
struct CStackModel {
    CEntryModel* Start; CEntryModel* Finish;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    void ProcessTransitions()
    {
        for (unsigned long i = 0; i < size(); ++i)
            Start[i].ProcessTransitions();
        Start[0].SetDefaultThemeWeight(0.0f, true);
    }
};
int main()
{
    CEntryModel e[3];
    for (int i = 0; i < 3; ++i) { e[i].BaseWeight = 0.5f; e[i].BaseWeightLocked = false; e[i].Processed = 0; e[i].Rebalanced = 0; }
    CStackModel s = { e, e + 3 };
    s.ProcessTransitions();
    if (e[0].Processed != 1 || e[1].Processed != 1 || e[2].Processed != 1 || e[0].BaseWeight != 0.0f || !e[0].BaseWeightLocked
        || e[0].Rebalanced != 1 || e[1].Rebalanced != 0 || e[1].BaseWeight != 0.5f) {
        std::printf("00887b00_TEST FAIL\n"); return 1;
    }
    std::printf("00887b00_TEST PASS\n");
    return 0;
}