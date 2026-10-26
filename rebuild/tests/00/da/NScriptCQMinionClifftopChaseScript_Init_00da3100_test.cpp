#include <cstdio>
#include <cstring>
// behaviour test: CQ_MinionClifftopChaseScript::Init -- register the quest's four regions, reset the mission state
// and both timers, and register the three standard boasts
struct SScriptDef { long Cost[3]; };
static SScriptDef g_def;
static int g_regions = 0, g_boasts = 0; static long g_costSum = 0;
static const char* g_names[4] = { "BowerstoneJail", "DesertedFarm", "GibbetWoods", "PrisonPath" };
struct CTimer { long Value; void Set(long v) { Value = v; } };
struct CGameScriptInterface {
    void AddQuestRegion(const char* q, const char* r) const { if (!std::strcmp(q, "Q_MinionClifftopChase") && !std::strcmp(r, g_names[g_regions])) ++g_regions; }
    void AddBoast(long id, long cost) const { if (id == 1 || id == 3 || id == 6) { ++g_boasts; g_costSum += cost; } }
};
struct CQ_MinionClifftopChaseScript {
    CGameScriptInterface* Game; CTimer MakeUniqueComment, MissionTimer; long MissionTimerID, KilledGuardingMinions, MinionCount;
    bool MissionFailed, RockTrollAppeared;
    void Init();
};
void CQ_MinionClifftopChaseScript::Init()
{
    for (int i = 0; i < 4; ++i) Game->AddQuestRegion("Q_MinionClifftopChase", g_names[i]);
    MissionFailed = false; RockTrollAppeared = false; KilledGuardingMinions = 0;
    MakeUniqueComment.Set(0); MissionTimer.Set(0); MissionTimerID = 0; MinionCount = 0;
    Game->AddBoast(1, g_def.Cost[0]);
    Game->AddBoast(3, g_def.Cost[1]);
    Game->AddBoast(6, g_def.Cost[2]);
}
int main()
{
    g_def.Cost[0] = 1; g_def.Cost[1] = 2; g_def.Cost[2] = 4;
    CGameScriptInterface game; CQ_MinionClifftopChaseScript s; s.Game = &game; s.MissionTimer.Value = 5; s.MissionTimerID = 3;
    s.MinionCount = 7; s.MissionFailed = true;
    s.Init();
    bool ok = g_regions == 4 && g_boasts == 3 && g_costSum == 7 && s.MissionTimer.Value == 0 && s.MakeUniqueComment.Value == 0
           && s.MissionTimerID == 0 && s.MinionCount == 0 && !s.MissionFailed;
    if (!ok) { std::printf("00da3100_TEST FAIL\n"); return 1; }
    std::printf("00da3100_TEST PASS\n");
    return 0;
}