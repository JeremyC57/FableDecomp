// Behaviour test for 0x0071C120 AdjustMorphsOfLowerPriority: the weight of each of the first count morphs whose priority is
// below the threshold is multiplied; later entries and higher priorities are untouched (covers the unrolled and tail loops).
#include <cstdio>

struct SEntry { int MorphName; short Priority; bool Supresses; };
struct SWeighted { SEntry first; float second; };
static void AdjustMorphsOfLowerPriority(int count, SWeighted* morphs, int priority, float multiplier)
{
    for (int i = 0; i < count; ++i)
        if (morphs[i].first.Priority < priority)
            morphs[i].second *= multiplier;
}
int main()
{
    SWeighted m[7];
    short pr[7] = { 1, 5, 2, 9, 0, 3, 1 };
    for (int i = 0; i < 7; ++i) { m[i].first.MorphName = i; m[i].first.Priority = pr[i]; m[i].first.Supresses = false; m[i].second = 1.0f; }
    AdjustMorphsOfLowerPriority(6, m, 3, 0.5f);
    float expect[7] = { 0.5f, 1.0f, 0.5f, 1.0f, 0.5f, 1.0f, 1.0f };
    bool ok = true;
    for (int i = 0; i < 7; ++i) ok = ok && m[i].second == expect[i];
    AdjustMorphsOfLowerPriority(0, m, 100, 0.0f);
    ok = ok && m[0].second == 0.5f;
    if (!ok) { std::printf("0071c120_TEST FAIL\n"); return 1; }
    std::printf("0071c120_TEST PASS\n");
    return 0;
}