#include <cstdio>
// MinionSitAtSchool::IsRunnable: villager && village && IsTimeToGoToSchool(creature, world, true) && base
struct Village { bool schoolTime; bool lastFlag; bool IsTimeToGoToSchool(void*, void*, bool f) { lastFlag = f; return schoolTime; } };
struct Group {
    bool hasVillager; Village* village; bool baseRunnable; int baseCalls;
    bool BaseIsRunnable() { ++baseCalls; return baseRunnable; }
    bool IsRunnable() { return hasVillager && village != 0 && village->IsTimeToGoToSchool(0, 0, true) && BaseIsRunnable(); }
};
int main()
{
    Village v; v.schoolTime = true; v.lastFlag = false; Group g; g.hasVillager = true; g.village = &v; g.baseRunnable = true; g.baseCalls = 0;
    if (!g.IsRunnable() || !v.lastFlag || g.baseCalls != 1) { std::printf("0090fd20_TEST FAIL\n"); return 1; }
    g.baseRunnable = false; if (g.IsRunnable()) { std::printf("0090fd20_TEST FAIL\n"); return 1; }
    v.schoolTime = false; if (g.IsRunnable() || g.baseCalls != 2) { std::printf("0090fd20_TEST FAIL\n"); return 1; }
    g.village = 0; if (g.IsRunnable()) { std::printf("0090fd20_TEST FAIL\n"); return 1; }
    g.village = &v; g.hasVillager = false; if (g.IsRunnable()) { std::printf("0090fd20_TEST FAIL\n"); return 1; }
    std::printf("0090fd20_TEST PASS\n"); return 0;
}