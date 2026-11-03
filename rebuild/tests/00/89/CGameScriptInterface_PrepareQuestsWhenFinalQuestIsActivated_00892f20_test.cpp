#include <cstdio>
// Model of CGameScriptInterface::PrepareQuestsWhenFinalQuestIsActivated: forward the final quest name to the quest manager.
struct QuestManager { const char* final; QuestManager() : final(0) { } void PrepareQuestsWhenFinalQuestIsActivated(const char* n) { final = n; } };
static QuestManager g_manager;
static QuestManager* g_pClass = &g_manager;
struct Interface { void PrepareQuestsWhenFinalQuestIsActivated(const char* n) const { g_pClass->PrepareQuestsWhenFinalQuestIsActivated(n); } };
int main()
{
    Interface gsi; gsi.PrepareQuestsWhenFinalQuestIsActivated("Q_Final");
    if (!g_manager.final || g_manager.final[2] != 'F') { std::printf("00892f20_TEST FAIL\n"); return 1; }
    std::printf("00892f20_TEST PASS\n"); return 0;
}