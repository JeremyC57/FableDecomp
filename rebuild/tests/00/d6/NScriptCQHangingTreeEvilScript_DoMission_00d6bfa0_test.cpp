#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeEvilScript::DoMission -- spawn the mission-end and region watchers, wait for
// BowerstoneJail, stage 1 spawns the party (when SpawnHere) and runs the introduction, waits for the bandits to start
// and shows the mission clock; stage 2 waits for HeroDualArea (stage 6), then the conclusion decides success
static int g_frames = 0, g_endAt = 1000, g_jailAt = 1, g_dualAt = 7, g_spawned = 0, g_parties = 0, g_intros = 0, g_conclusions = 0;
static long g_partySize = 0; static float g_timer = 0; static bool g_shown = false;
struct SScriptDef { float HangingTreeEvilMissionTimer; };
static SScriptDef g_def;
class CTimer { public: float Time; void SetTime(float t) { Time = t; } };
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const char* n) const
    { return (!std::strcmp(n, "BowerstoneJail") && g_frames >= g_jailAt) || (!std::strcmp(n, "HeroDualArea") && g_frames >= g_dualAt); }
    long AddQuestInfoTimer(const CTimer& t, const char* icon, float s) const { g_timer = t.Time; return (!std::strcmp(icon, "HUD_CLOCK_ICON") && s == 1.0f) ? 12 : 0; }
    void DisplayQuestInfo(bool d) const { g_shown = d; }
};
struct CQ_HangingTreeEvilScript {
    CGameScriptInterface* Game; long HTE_State, CurrentBandits, TimerGUIIndex; CTimer MissionTimer;
    bool SwitchingRegions, SpawnHere, BanditsStarted, MissionFailed, MissionSucceeded;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void SpawnFunction(const char* n) { if (!std::strcmp(n, "WatchForMissionEnd") || !std::strcmp(n, "MonitorRegionPresence")) ++g_spawned; }
    void SpawnParty(long n) { ++g_parties; g_partySize = n; }
    void Introduction() { ++g_intros; }
    void Conclusion() { ++g_conclusions; }
    void DoMission();
};
void CQ_HangingTreeEvilScript::DoMission()
{
    SpawnFunction("WatchForMissionEnd");
    SpawnFunction("MonitorRegionPresence");
    while (!Game->IsLevelLoaded("BowerstoneJail")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    HTE_State = 1;
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (HTE_State == 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            while (SwitchingRegions) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (SpawnHere) { SpawnParty(CurrentBandits); Introduction(); }
            while (!BanditsStarted) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            MissionTimer.SetTime(g_def.HangingTreeEvilMissionTimer);
            TimerGUIIndex = Game->AddQuestInfoTimer(MissionTimer, "HUD_CLOCK_ICON", 1.0f);
            Game->DisplayQuestInfo(true);
            HTE_State = 2;
        }
        while (HTE_State == 2) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Game->IsLevelLoaded("HeroDualArea")) HTE_State = 6;
        }
    } while (HTE_State != 6);
    if (IsActiveThreadTerminating()) return;
    Conclusion();
    if (MissionFailed) return;
    MissionSucceeded = true;
}
int main()
{
    g_def.HangingTreeEvilMissionTimer = 300.0f;
    CGameScriptInterface game; CQ_HangingTreeEvilScript s; s.Game = &game; s.CurrentBandits = 4; s.HTE_State = 0;
    s.SwitchingRegions = false; s.SpawnHere = true; s.BanditsStarted = true; s.MissionFailed = false; s.MissionSucceeded = false;
    s.DoMission();
    bool ok = g_spawned == 2 && g_parties == 1 && g_partySize == 4 && g_intros == 1 && g_timer == 300.0f && g_shown
           && s.TimerGUIIndex == 12 && s.HTE_State == 6 && g_conclusions == 1 && s.MissionSucceeded;
    g_frames = 0; s.MissionSucceeded = false; s.MissionFailed = true; s.SpawnHere = false;
    s.DoMission();                                      // failed in the conclusion; no party when not SpawnHere
    ok = ok && g_parties == 1 && g_conclusions == 2 && !s.MissionSucceeded;
    if (!ok) { std::printf("00d6bfa0_TEST FAIL\n"); return 1; }
    std::printf("00d6bfa0_TEST PASS\n");
    return 0;
}