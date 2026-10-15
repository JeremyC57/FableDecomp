#include <cstdio>
// CAIStateGroup_MinionGuardBribed::IsRunnable: villager present && village exists && frame < village->bribeEnd
struct Village { long frameBribeEnds; };
static Village* g_village = 0; static long g_frame = 100; static bool g_hasVillager = true;
struct Group {
    bool IsRunnable()
    {
        if (!g_hasVillager) return false;
        if (g_village == 0) return false;
        return g_frame < g_village->frameBribeEnds;
    }
};
int main()
{
    Village v; v.frameBribeEnds = 150; g_village = &v; Group g;
    if (!g.IsRunnable()) { std::printf("00929aa0_TEST FAIL\n"); return 1; }
    g_frame = 150; if (g.IsRunnable()) { std::printf("00929aa0_TEST FAIL\n"); return 1; }
    g_frame = 100; g_village = 0; if (g.IsRunnable()) { std::printf("00929aa0_TEST FAIL\n"); return 1; }
    g_village = &v; g_hasVillager = false; if (g.IsRunnable()) { std::printf("00929aa0_TEST FAIL\n"); return 1; }
    std::printf("00929aa0_TEST PASS\n"); return 0;
}