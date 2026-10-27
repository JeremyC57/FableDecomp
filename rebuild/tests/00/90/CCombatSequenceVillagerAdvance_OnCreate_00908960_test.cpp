// behaviour test for 0x00908960 CCombatSequence_VillagerAdvance::OnCreate (model: two actions queued in order, then the base runs)
#include <cstdio>
#include <vector>
static std::vector<int> g_actions; static int g_base = 0;
static void AddAction(int a) { g_actions.push_back(a); }
static void OnCreate() { AddAction(1); AddAction(2); ++g_base; }
int main()
{
    OnCreate();
    int ok = g_actions.size() == 2 && g_actions[0] == 1 && g_actions[1] == 2 && g_base == 1;
    if (!ok) { std::printf("00908960_TEST FAIL\n"); return 1; }
    std::printf("00908960_TEST PASS\n");
    return 0;
}