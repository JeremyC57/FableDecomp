#include <cstdio>
#include <cstring>
// behaviour test: CQ_OakValeBanditRaidScript::Main -- region wait, objective, persistent gate, hero-cast cut-scene,
// start screen, bindings, rumours, music, populate, three spawns, five frames, tutorial wait, idle until terminate
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0;
static int g_termAt = 40;
static bool g_paused = false;
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frames; }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "BanditCampPathEntrance") == 0 && g_frames >= 3; }
    void SetQuestCardObjective(const char* q, const char*, const char*, const char*) { L(q); }
    void SetThingPersistent(bool p) { L(p ? "persist" : "nopersist"); }
    bool StartScriptingEntity() { return g_frames >= 5; }
    void PauseAllNonScriptedEntities(bool p) { g_paused = p; }
    void KickOffQuestStartScreen(const char* q) { L("start"); L(q); }
    void PostAddScriptedEntities() { L("gpost"); }
    void AddRumourCategory(const char* c) { L(c); }
    void AddNewRumourToCategory(const char*, const char* t) { L(t); }
    void SetCategoryActivity(const char*, bool a) { L(a ? "active" : "inactive"); }
    void OverrideMusic(int m) { char b[16]; std::sprintf(b, "music%d", m); L(b); }
    bool DisplayTutorial(int t) { return t == 0x22; }
    bool MsgIsTutorialClickedPast() { return g_frames >= 15; }
};
struct CQ_OakValeBanditRaidScript {
    CGameScriptInterface* Game; int Bindings; int Spawns;
    bool IsActiveThreadTerminating() { return g_frames >= g_termAt; }
    void RunCutsceneMacro(const char* n) { if (g_paused) L(n); }
    void AddEntityScriptBinding(const char* n) { ++Bindings; L(n); }
    void PostAddScriptedEntities() { L("post"); }
    void PopulateLevel() { L("populate"); }
    void SpawnFunction(const char* n) { ++Spawns; L(n); }
    void Main()
    {
        while (!Game->IsRegionLoaded("BanditCampPathEntrance")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_02", "", "OakBay");
        Game->SetThingPersistent(true);
        while (!Game->StartScriptingEntity()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_BANDITRAID_GATESTART");
        Game->PauseAllNonScriptedEntities(false);
        Game->KickOffQuestStartScreen("Q_BanditCamp");
        AddEntityScriptBinding("OakValeBanditRaidBandit");
        AddEntityScriptBinding("M_RestartMarker");
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        Game->AddRumourCategory("OAKVALE_BANDITRAID");
        Game->AddNewRumourToCategory("OAKVALE_BANDITRAID", "TEXT_SCR_BANDITRAID");
        Game->SetCategoryActivity("OAKVALE_BANDITRAID", true);
        Game->OverrideMusic(1);
        PopulateLevel();
        SpawnFunction("WatchForLevelChanges"); SpawnFunction("WatchForGateTimer"); SpawnFunction("WatchForWhistling");
        for (int i = 0; i < 5; ++i) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (Game->DisplayTutorial(0x22)) {
            if (IsActiveThreadTerminating()) return;
            while (!Game->MsgIsTutorialClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
        }
        for (;;) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    }
};
int main()
{
    CGameScriptInterface gsi; CQ_OakValeBanditRaidScript s; s.Game = &gsi; s.Bindings = 0; s.Spawns = 0;
    s.Main();
    const char* want = "Q_BanditCamp;persist;CS_BANDITRAID_GATESTART;start;Q_BanditCamp;OakValeBanditRaidBandit;M_RestartMarker;post;gpost;OAKVALE_BANDITRAID;TEXT_SCR_BANDITRAID;active;music1;populate;WatchForLevelChanges;WatchForGateTimer;WatchForWhistling;";
    bool ok = std::strcmp(g_log, want) == 0 && s.Bindings == 2 && s.Spawns == 3 && g_frames == 40 && !g_paused;
    // early termination during the region wait stops before any side effect
    g_log[0] = 0; g_frames = 0; g_termAt = 2; s.Main();
    ok = ok && g_log[0] == 0 && g_frames == 2;
    if (!ok) { std::printf("00dbf2d0_TEST FAIL\n"); return 1; }
    std::printf("00dbf2d0_TEST PASS\n");
    return 0;
}
