#include <cstdio>
#include <cstring>
// behaviour test: CQ_WaspBossScript::Main -- set the helper timer, wait for LookoutPoint, tutorial, nine bindings, the
// objective and guild-master message, wait for PicnicArea, override the music and spawn three threads
static int g_frames, g_endAt, g_lookoutAt, g_picnicAt, g_timerSet, g_tutorial, g_bound, g_obj, g_msg, g_music;
static char g_spawned[256];
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const char* n) const { return !std::strcmp(n, "PicnicArea") && g_frames >= g_picnicAt; }
    bool IsRegionLoaded(const char* n) const { return !std::strcmp(n, "LookoutPoint") && g_frames >= g_lookoutAt; }
    void PostAddScriptedEntities() const {}
    void SetTimer(long timer, long time) { if (timer == 4) g_timerSet = (int)time; }
    bool GiveHeroTutorial(int c) const { g_tutorial = c; return true; }
    void HeroReceiveMessageFromGuildMaster(const char* t, const char* e, bool a, bool b) const { if (!std::strcmp(t, "TEXT_QST_028_GUILDSEAL_WASP_MAP") && !*e && a && b) ++g_msg; }
    void SetQuestCardObjective(const char* q, const char* t, const char* r, const char* l) const
    {
        if (!std::strcmp(q, "Q_WaspBoss") && !std::strcmp(t, "TEXT_QUEST_WASP_MENACE_OBJECTIVE_01") && !*r && !std::strcmp(l, "HeroGuildComplexInside")) ++g_obj;
    }
    void Pause(float) const { ++g_frames; }
    const char* GetActiveQuestName() const { return "Q_WaspBoss"; }
    void OverrideMusic(int m, bool a, bool b) const { if (m == 23 && !a && !b) ++g_music; }
};
static CGameScriptInterface g_game;
static CGameScriptInterface* g_pGameScriptInterface = &g_game;
struct CTimer { long TimerIndex; void SetTime(long t) { g_pGameScriptInterface->SetTimer(TimerIndex, t); } };
struct CQ_WaspBossScript {
    CGameScriptInterface* Game; CTimer ReachedWaspHelper;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void AddEntityScriptBinding(const char*) { ++g_bound; }
    void PostAddScriptedEntities() {}
    void AddSpawnedFunction(const char* n, const char* region) { std::strcat(g_spawned, n); std::strcat(g_spawned, "@"); std::strcat(g_spawned, region); std::strcat(g_spawned, ";"); }
    void SpawnFunction(const char* n) { AddSpawnedFunction(n, ""); }
    void Main();
};
void CQ_WaspBossScript::Main()
{
    ReachedWaspHelper.SetTime(120);
    while (!Game->IsRegionLoaded("LookoutPoint")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->Pause(1.0f);
    Game->GiveHeroTutorial(6);
    static const char* const names[9] = { "GratefulVillagerSpawn", "WaspChaser", "WaspChaseWoman", "WaspAttacker", "WaspVictim",
        "FleeingWoman", "WaspHelper", "QueenHornet", "HornetDrone" };
    for (int i = 0; i < 9; ++i) AddEntityScriptBinding(names[i]);
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WASP_MENACE_OBJECTIVE_01", "", "HeroGuildComplexInside");
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_028_GUILDSEAL_WASP_MAP", "", true, true);
    while (!Game->IsLevelLoaded("PicnicArea")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->OverrideMusic(23, false, false);
    SpawnFunction("WatchForTermination");
    AddSpawnedFunction("DoMission", "PicnicArea");
    SpawnFunction("WatchForCutscene");
}
int main()
{
    CQ_WaspBossScript s; s.Game = &g_game; s.ReachedWaspHelper.TimerIndex = 4;
    g_frames = 0; g_endAt = 100; g_lookoutAt = 2; g_picnicAt = 6; g_spawned[0] = 0;
    s.Main();
    bool ok = g_timerSet == 120 && g_tutorial == 6 && g_bound == 9 && g_obj == 1 && g_msg == 1 && g_music == 1
        && !std::strcmp(g_spawned, "WatchForTermination@;DoMission@PicnicArea;WatchForCutscene@;");
    g_frames = 0; g_endAt = 1; g_timerSet = 0; g_bound = 0; g_spawned[0] = 0;
    s.Main();                                           // thread ends before LookoutPoint loads
    ok = ok && g_timerSet == 120 && g_bound == 0 && g_spawned[0] == 0;
    if (!ok) { std::printf("00e0ea40_TEST FAIL\n"); return 1; }
    std::printf("00e0ea40_TEST PASS\n");
    return 0;
}
