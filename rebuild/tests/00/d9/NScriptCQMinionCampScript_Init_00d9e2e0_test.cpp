#include <cstdio>
#include <cstring>
// behaviour test: CQ_MinionCampScript::Init -- reset the mission state and both timers (at most 3 minions), copy the
// minion spawn periods from the script def, register Darkwood3 and the four boasts (the last one BRIARNODAMAGE, 0x2a)
struct SScriptDef { float MCMinMinionSpawnPeriod, MCMaxMinionSpawnPeriod; long Cost[4]; };
static SScriptDef g_def;
static int g_regions = 0, g_boasts = 0, g_lastId = 0; static long g_costSum = 0;
struct CTimer { long Value; void Set(long v) { Value = v; } };
struct CGameScriptInterface {
    void AddQuestRegion(const char* q, const char* r) const { if (!std::strcmp(q, "Q_MinionCamp") && !std::strcmp(r, "Darkwood3")) ++g_regions; }
    void AddBoast(const char*, long id, long cost) const { ++g_boasts; g_lastId = id; g_costSum += cost; }
};
struct CQ_MinionCampScript {
    CGameScriptInterface* Game; CTimer ChantTimer, MinionSpawnTimer; bool IntroDone, MissionFailed, ChantTimerPaused;
    long NumberOfMinions, MaxNumberOfMinions, CurrentStage, BriarState; float MinMinionSpawnPeriod, MaxMinionSpawnPeriod;
    void Init();
};
void CQ_MinionCampScript::Init()
{
    IntroDone = false; MissionFailed = false; NumberOfMinions = 0;
    ChantTimer.Set(0); ChantTimerPaused = false; MinionSpawnTimer.Set(0);
    MaxNumberOfMinions = 3;
    MinMinionSpawnPeriod = g_def.MCMinMinionSpawnPeriod;
    MaxMinionSpawnPeriod = g_def.MCMaxMinionSpawnPeriod;
    CurrentStage = 0; BriarState = 0;
    Game->AddQuestRegion("Q_MinionCamp", "Darkwood3");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_def.Cost[0]);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_def.Cost[1]);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_def.Cost[2]);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_BRIARNODAMAGE", 0x2a, g_def.Cost[3]);
}
int main()
{
    g_def.MCMinMinionSpawnPeriod = 4.0f; g_def.MCMaxMinionSpawnPeriod = 9.0f;
    for (int i = 0; i < 4; ++i) g_def.Cost[i] = i + 1;
    CGameScriptInterface game; CQ_MinionCampScript s; s.Game = &game; s.ChantTimer.Value = 5; s.MinionSpawnTimer.Value = 6;
    s.CurrentStage = 4; s.MissionFailed = true; s.ChantTimerPaused = true;
    s.Init();
    bool ok = s.ChantTimer.Value == 0 && s.MinionSpawnTimer.Value == 0 && s.MaxNumberOfMinions == 3 && s.CurrentStage == 0
           && s.MinMinionSpawnPeriod == 4.0f && s.MaxMinionSpawnPeriod == 9.0f && !s.MissionFailed && !s.ChantTimerPaused
           && g_regions == 1 && g_boasts == 4 && g_lastId == 0x2a && g_costSum == 10;
    if (!ok) { std::printf("00d9e2e0_TEST FAIL\n"); return 1; }
    std::printf("00d9e2e0_TEST PASS\n");
    return 0;
}