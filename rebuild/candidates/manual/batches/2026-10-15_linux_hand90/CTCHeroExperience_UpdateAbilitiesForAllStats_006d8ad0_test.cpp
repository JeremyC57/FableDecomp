#include <cstdio>
// Behaviour of CTCHeroExperience::UpdateAbilitiesForAllStats (0x006D8AD0): stats 0..9 updated with the caller's
// flag, then OnStatLevelChange(false).
static long g_stats[16]; static bool g_flags[16]; static int g_calls = 0; static int g_levelChange = 0; static bool g_levelFlag = true;
struct CTCHeroExperience_Methods {
    void UpdateAbilitiesForStat(long stat, bool silent) { g_stats[g_calls] = stat; g_flags[g_calls] = silent; ++g_calls; }
    void OnStatLevelChange(bool silent) { ++g_levelChange; g_levelFlag = silent; if (g_calls != 10) g_levelChange = 100; }
    void UpdateAbilitiesForAllStats(bool silent)
    {
        for (long stat = 0; stat < 10; ++stat)
            UpdateAbilitiesForStat(stat, silent);
        OnStatLevelChange(false);
    }
};
int main()
{
    CTCHeroExperience_Methods e;
    e.UpdateAbilitiesForAllStats(true);
    if (g_calls != 10 || g_levelChange != 1 || g_levelFlag) { std::printf("006d8ad0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 10; ++i)
        if (g_stats[i] != i || !g_flags[i]) { std::printf("006d8ad0_TEST FAIL\n"); return 1; }
    std::printf("006d8ad0_TEST PASS\n");
    return 0;
}
