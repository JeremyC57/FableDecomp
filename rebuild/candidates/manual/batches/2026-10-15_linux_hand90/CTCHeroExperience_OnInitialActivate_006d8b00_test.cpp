#include <cstdio>
// Behaviour of CTCHeroExperience::OnInitialActivate (0x006D8B00): attack history resized to 10 records of {-1, 0},
// every stat updated with silent=true, then OnStatLevelChange(false).
struct SAttackRecord {
    long Attribute;
    long Count;
    SAttackRecord(long attribute, long count) : Attribute(attribute), Count(count) {}
};
static unsigned long g_size = 0; static long g_attr = 0, g_count = 7; static int g_stats = 0; static bool g_allSilent = true; static int g_change = 0; static bool g_changeFlag = true;
struct CAttackHistoryList {
    void* m_pHead;
    void resize(unsigned long size, const SAttackRecord& value) { g_size = size; g_attr = value.Attribute; g_count = value.Count; }
};
struct CTCHeroExperience_Retail {
    CAttackHistoryList AttackHistory;
    void UpdateAbilitiesForStat(long stat, bool silent) { if (stat != g_stats) g_allSilent = false; ++g_stats; if (!silent) g_allSilent = false; }
    void OnStatLevelChange(bool silent) { ++g_change; g_changeFlag = silent; }
    void OnInitialActivate()
    {
        AttackHistory.resize(10, SAttackRecord(-1, 0));
        for (long stat = 0; stat < 10; ++stat)
            UpdateAbilitiesForStat(stat, true);
        OnStatLevelChange(false);
    }
};
int main()
{
    CTCHeroExperience_Retail e;
    e.OnInitialActivate();
    if (g_size != 10 || g_attr != -1 || g_count != 0 || g_stats != 10 || !g_allSilent || g_change != 1 || g_changeFlag) {
        std::printf("006d8b00_TEST FAIL\n"); return 1;
    }
    std::printf("006d8b00_TEST PASS\n");
    return 0;
}
