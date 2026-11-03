#include <cstdio>
// Model of CGameScriptInterface::SetQuestCardRenownReward: forward (name, renown) to the quest manager singleton.
struct QuestManager { const char* name; long renown; void SetQuestCardRenownReward(const char* n, long g) { name = n; renown = g; } };
static QuestManager g_manager;
static QuestManager* g_pClass = &g_manager;
struct Interface { void SetQuestCardRenownReward(const char* n, long g) const { g_pClass->SetQuestCardRenownReward(n, g); } };
int main()
{
    Interface gsi; gsi.SetQuestCardRenownReward("Q_Test", 250);
    if (g_manager.renown != 250 || g_manager.name[0] != 'Q') { std::printf("00891500_TEST FAIL\n"); return 1; }
    std::printf("00891500_TEST PASS\n"); return 0;
}
