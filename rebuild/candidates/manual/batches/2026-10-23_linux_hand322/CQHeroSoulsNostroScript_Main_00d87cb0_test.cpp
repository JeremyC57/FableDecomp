#include <cstdio>
#include <cstring>
// behaviour test: CQ_HeroSoulsNostroScript::Main -- bindings, wait for the region, start cut-scene + quest card + start
// screen + objective, spawn two threads, wait for the defeat flag, then success cut-scene, logbook entry, completion
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0, g_loadedAt = 2, g_defeatAt = 4, g_endAt = 1000;
struct CData { bool NostroDefeated; };
static CData g_data;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; if (g_frames >= g_defeatAt) g_data.NostroDefeated = true; }
    bool IsRegionLoaded(const char*) const { return g_frames >= g_loadedAt; }
    void PostAddScriptedEntities() const { L("gpost"); }
    void SetQuestAsCompleted(const char* q, bool, bool, bool) const { L("complete"); L(q); }
    void GiveHeroQuestCardDirectly(const char* card, const char* q, bool) const { L(card); L(q); }
    void SetQuestCardObjective(const char* q, const char* t, const char* r1, const char* r2) const { L(q); L(t); L(r1); L(r2); }
    void KickOffQuestStartScreen(const char* q, bool, bool) const { L("start"); L(q); }
    void FadeScreenIn() { L("fade"); }
    void Pause(float) const { L("pause"); }
    const char* GetActiveQuestName() const { return "Q"; }
};
struct CQ_HeroSoulsNostroScript {
    CGameScriptInterface* Game; CData* PMasterData; bool QuestStartScreenShown;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void Bind(const char* n, unsigned long) { L(n); }
    void PostAddScriptedEntities() { L("post"); }
    void PlayCutScene(const char* n) { L(n); }
    void Spawn(const char* n) { L(n); }
    void Main();
};
static void AddLogbookStoryEntryString(const char* t) { L(t); }
void CQ_HeroSoulsNostroScript::Main()
{
    Bind("SummonerToKill", 0); Bind("GuardNearGraveyardPath", 1); Bind("UndeadSoldier", 0); Bind("Nostro", 0); Bind("SUMMONED_CREATURE", 0);
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("GraveyardCircle")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    PlayCutScene("CS3START");
    Game->GiveHeroQuestCardDirectly("CARD", Game->GetActiveQuestName(), true);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    QuestStartScreenShown = true;
    Game->SetQuestCardObjective("Q_HeroSouls", "OBJ13", "GraveyardCircle", "Graveyard");
    Spawn("DealWithCreatureGeneration"); Spawn("WatchForNostroHit");
    Game->FadeScreenIn();
    while (!PMasterData->NostroDefeated) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->Pause(1.0f);
    PlayCutScene("CS3SUCCESS");
    AddLogbookStoryEntryString("LOG370");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
    Game->FadeScreenIn();
}
int main()
{
    CGameScriptInterface gsi; CQ_HeroSoulsNostroScript s; s.Game = &gsi; s.PMasterData = &g_data; s.QuestStartScreenShown = false;
    g_data.NostroDefeated = false;
    s.Main();
    const char* want = "SummonerToKill;GuardNearGraveyardPath;UndeadSoldier;Nostro;SUMMONED_CREATURE;post;gpost;CS3START;CARD;Q;start;Q;Q_HeroSouls;OBJ13;GraveyardCircle;Graveyard;DealWithCreatureGeneration;WatchForNostroHit;fade;pause;CS3SUCCESS;LOG370;complete;Q;fade;";
    if (std::strcmp(g_log, want) != 0 || !s.QuestStartScreenShown || g_frames != 4) { std::printf("00d87cb0_TEST FAIL\n"); return 1; }
    // terminating during the first wait: nothing after the bindings
    g_log[0] = 0; g_frames = 0; g_endAt = 1; g_data.NostroDefeated = false; s.QuestStartScreenShown = false;
    s.Main();
    if (std::strcmp(g_log, "SummonerToKill;GuardNearGraveyardPath;UndeadSoldier;Nostro;SUMMONED_CREATURE;post;gpost;") != 0 || s.QuestStartScreenShown) { std::printf("00d87cb0_TEST FAIL\n"); return 1; }
    std::printf("00d87cb0_TEST PASS\n");
    return 0;
}
