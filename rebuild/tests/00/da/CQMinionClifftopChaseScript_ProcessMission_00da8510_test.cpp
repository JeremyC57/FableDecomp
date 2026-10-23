#include <cstdio>
#include <cstring>
// behaviour test: CQ_MinionClifftopChaseScript::ProcessMission -- region waits + objectives, troll comment, escape timer;
// a timer that runs out fails the mission with reason 1
static char g_log[1024];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0;
static long g_timers[4];
struct CQ_MinionClifftopChaseScript;
static CQ_MinionClifftopChaseScript* g_s;
struct CGameScriptInterface {
    void NewScriptFrame() const;
    bool IsRegionLoaded(const char*) const { return g_frames % 2 == 1; }
    void SetTimer(long t, long v) { g_timers[t] = v; }
    long GetTimer(long t) const { return g_timers[t]; }
    void SetQuestCardObjective(const char*, const char* t, const char*, const char*) const { L(t); }
    void DisplayQuestInfo(bool) const { L("info"); }
    long AddQuestInfoTimer(long timer, const char*, float) const { return 40 + timer; }
    void StopOverrideMusic(bool) const { L("music"); }
};
static CGameScriptInterface g_gsi;
struct CQ_MinionClifftopChaseScript {
    CGameScriptInterface* Game; bool MapKeeperSaidDialog, MissionFailed, RockTrollSeen; long EscapeTimer, TimerBarID, ReasonForFailure;
    bool IsActiveThreadTerminating() { return false; }
    void GuardComment(const char* c) { L(c); }
    void ProcessMission()
    {
        while (!Game->IsRegionLoaded("DesertedFarm")) Game->NewScriptFrame();
        Game->SetQuestCardObjective("Q", "OBJ03", "GibbetWoods", "BowerstoneSlums");
        while (!Game->IsRegionLoaded("GibbetWoods")) Game->NewScriptFrame();
        Game->SetQuestCardObjective("Q", "OBJ04", "PrisonPath", "BowerstoneSlums");
        while (!RockTrollSeen) Game->NewScriptFrame();
        GuardComment("ROCK_TROLL");
        while (!Game->IsRegionLoaded("PrisonPath")) Game->NewScriptFrame();
        Game->StopOverrideMusic(false);
        g_gsi.SetTimer(EscapeTimer, 3);
        Game->DisplayQuestInfo(true);
        TimerBarID = Game->AddQuestInfoTimer(EscapeTimer, "HUD_CLOCK_ICON", 1.0f);
        Game->SetQuestCardObjective("Q", "OBJ05", "PrisonPath", "BowerstoneSlums");
        while (!MapKeeperSaidDialog && !MissionFailed && g_gsi.GetTimer(EscapeTimer) != 0) Game->NewScriptFrame();
        if (g_gsi.GetTimer(EscapeTimer) == 0) { ReasonForFailure = 1; MissionFailed = true; }
    }
};
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_frames == 4) g_s->RockTrollSeen = true; if (g_timers[2] > 0) --g_timers[2]; }
int main()
{
    CQ_MinionClifftopChaseScript s; g_s = &s; s.Game = &g_gsi; s.MapKeeperSaidDialog = false; s.MissionFailed = false; s.RockTrollSeen = false;
    s.EscapeTimer = 2; s.TimerBarID = 0; s.ReasonForFailure = 0;
    s.ProcessMission();
    if (std::strcmp(g_log, "OBJ03;OBJ04;ROCK_TROLL;music;info;OBJ05;") != 0 || !s.MissionFailed || s.ReasonForFailure != 1 || s.TimerBarID != 42) { std::printf("00da8510_TEST FAIL\n"); return 1; }
    std::printf("00da8510_TEST PASS\n");
    return 0;
}