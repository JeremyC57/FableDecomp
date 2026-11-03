#include <cstdio>
// Model of CGameScriptInterface::PrepareQuestsWhenFinalQuestIsCompleted -> CQuestManager 0x004AF270, which sets
// World->MapTableShowQuestCardsOnUsed (+0xDA) = true.
struct World { bool MapTableShowQuestCardsOnUsed; };
struct QuestManager { World* world; void PrepareQuestsWhenFinalQuestIsCompleted() { world->MapTableShowQuestCardsOnUsed = true; } };
static World g_world = { false };
static QuestManager g_manager = { &g_world };
static QuestManager* g_pClass = &g_manager;
struct Interface { void PrepareQuestsWhenFinalQuestIsCompleted() const { g_pClass->PrepareQuestsWhenFinalQuestIsCompleted(); } };
int main()
{
    Interface gsi; gsi.PrepareQuestsWhenFinalQuestIsCompleted();
    if (!g_world.MapTableShowQuestCardsOnUsed) { std::printf("00892f30_TEST FAIL\n"); return 1; }
    std::printf("00892f30_TEST PASS\n"); return 0;
}