#include <cstdio>
// Model of CGameScriptInterface::SetQuestCardGoldReward: forward (name, gold) to the quest manager singleton.
struct QuestManager { const char* name; long gold; void SetQuestCardGoldReward(const char* n, long g) { name = n; gold = g; } };
static QuestManager g_manager;
static QuestManager* g_pClass = &g_manager;
struct Interface { void SetQuestCardGoldReward(const char* n, long g) const { g_pClass->SetQuestCardGoldReward(n, g); } };
int main()
{
    Interface gsi; gsi.SetQuestCardGoldReward("Q_Test", 250);
    if (g_manager.gold != 250 || g_manager.name[0] != 'Q') { std::printf("008914f0_TEST FAIL\n"); return 1; }
    std::printf("008914f0_TEST PASS\n"); return 0;
}