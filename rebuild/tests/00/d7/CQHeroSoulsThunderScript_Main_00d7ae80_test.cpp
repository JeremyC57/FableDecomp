#include <cstdio>
#include <cstring>
// behaviour test: CQ_HeroSoulsThunderScript::Main -- first-run start block (gate, line pick, cut-scene, quests, objective,
// fade), Jack's one-off conversation, wait for Thunder's defeat, success cut-scene + morality + log + completion
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0, g_endAt = 1000;
struct CData { bool ThunderDefeated; long ArenaState; bool LineA; bool LineB; };
static CData g_data;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; if (g_frames >= 3) g_data.ThunderDefeated = true; }
    bool IsRegionLoaded(const char*) const { return g_frames >= 1; }
    void PostAddScriptedEntities() const {}
    void OpenDoor(const char* d) const { L(d); }
    void ActivateQuest(const char* q) const { L(q); }
    void DeactivateQuest(const char* q, unsigned long) const { L(q); }
    void SetSoundThemesAsEnabledForRegion(const char* r, bool) const { L(r); }
    void SetQuestCardObjective(const char*, const char* t, const char*, const char*) const { L(t); }
    void FadeScreenIn() { L("fade"); }
    long AddNewConversation(const char*, bool, bool) const { return 7; }
    void AddLineToConversation(long c, const char* t, bool, const char*, const char*) const { if (c == 7) L(t); }
    void GiveHeroMorality(float m) const { if (m == 2.5f) L("morality"); }
    void SetQuestAsCompleted(const char*, bool, bool, bool) const { L("complete"); }
};
struct CQ_HeroSoulsThunderScript {
    CGameScriptInterface* Game; CData* PMasterData; bool StartDone, SkipStart, JackSpoke; const char* Line1;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void PlayCutScene(const char* n) { L(n); }
    void Main();
};
static float g_reward = 2.5f;
void CQ_HeroSoulsThunderScript::Main()
{
    while (!Game->IsRegionLoaded("KnotholeGlade")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (!StartDone && !SkipStart) {
        if (IsActiveThreadTerminating()) return;
        Game->OpenDoor("KG_Gate");
        if (PMasterData->LineA) Line1 = "A"; else if (PMasterData->LineB) Line1 = "B"; else Line1 = "C";
        PlayCutScene("START");
        StartDone = true;
        Game->ActivateQuest("Q_HeroSoulsArena"); Game->ActivateQuest("Q_HeroSoulsPermanentArena");
        Game->DeactivateQuest("Q_ArenaHoldingScript", 0);
        Game->SetSoundThemesAsEnabledForRegion("ArenaExterior", false);
        Game->SetSoundThemesAsEnabledForRegion("ArenaCells", false);
        Game->SetSoundThemesAsEnabledForRegion("Arena", false);
        PMasterData->ArenaState = 2;
        Game->SetQuestCardObjective("Q_HeroSouls", "OBJ03", "Arena", "KnotholeGlade");
        Game->FadeScreenIn();
    }
    if (!JackSpoke) {
        if (IsActiveThreadTerminating()) return;
        long c = Game->AddNewConversation("hero", false, false);
        Game->AddLineToConversation(c, "JACK10", false, "hero", "hero");
        if (PMasterData->LineA) { if (IsActiveThreadTerminating()) return; Game->AddLineToConversation(c, "JACK20", false, "hero", "hero"); }
        JackSpoke = true;
    }
    while (!PMasterData->ThunderDefeated) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    PlayCutScene("SUCCESS");
    Game->GiveHeroMorality(g_reward);
    L("log355");
    Game->SetQuestAsCompleted("Q", true, true, true);
}
int main()
{
    CGameScriptInterface gsi; CQ_HeroSoulsThunderScript s; s.Game = &gsi; s.PMasterData = &g_data;
    s.StartDone = false; s.SkipStart = false; s.JackSpoke = false; s.Line1 = "";
    g_data.ThunderDefeated = false; g_data.ArenaState = 0; g_data.LineA = false; g_data.LineB = true;
    s.Main();
    const char* want = "KG_Gate;START;Q_HeroSoulsArena;Q_HeroSoulsPermanentArena;Q_ArenaHoldingScript;ArenaExterior;ArenaCells;Arena;OBJ03;fade;JACK10;SUCCESS;morality;log355;complete;";
    if (std::strcmp(g_log, want) != 0 || std::strcmp(s.Line1, "B") != 0 || g_data.ArenaState != 2 || !s.StartDone || !s.JackSpoke) { std::printf("00d7ae80_TEST FAIL\n"); return 1; }
    // second run: start block and Jack skipped, LineA path not reached
    g_log[0] = 0; s.Main();
    if (std::strcmp(g_log, "SUCCESS;morality;log355;complete;") != 0) { std::printf("00d7ae80_TEST FAIL\n"); return 1; }
    std::printf("00d7ae80_TEST PASS\n");
    return 0;
}