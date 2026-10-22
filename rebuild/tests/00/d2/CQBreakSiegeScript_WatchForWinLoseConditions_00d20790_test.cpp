#include <cstdio>
#include <cstring>
// behaviour test: CQ_BreakSiegeScript::WatchForWinLoseConditions waits for every attacker (plus the siege leader once
// attacked) to die while KnotholeGlade is loaded, then (second wave started or after waiting for it) makes the
// imprisoned leader unkillable and succeeds -- unless the mission failed or the thread is ending
static int g_frames = 0, g_deadAt = 3, g_endAt = 1000, g_leaderAdds = 0, g_killable = 0;
struct CScriptThingVector { int count; };
struct CGameScriptInterface {
    void GetAllThingsWithScriptName(const char* name, CScriptThingVector& v) const
    { if (std::strcmp(name, "BanditSiegeLeader") == 0) ++g_leaderAdds; ++v.count; }
    bool IsRegionLoaded(const char* name) const { return std::strcmp(name, "KnotholeGlade") == 0; }
    void NewScriptFrame() const { ++g_frames; }
    int GetThingWithScriptName(const char* name) const { return std::strcmp(name, "ImprisonedBanditLeader") == 0 ? 7 : 0; }
    void EntitySetAsKillable(int thing, bool a, bool b) const { if (thing == 7 && !a && !b) ++g_killable; }
};
static bool AreAllThingsInVectorDead(const CScriptThingVector&) { return g_frames >= g_deadAt; }
struct CQ_BreakSiegeScript {
    CGameScriptInterface* Game;
    bool AttackedBanditLeader, StartSecondWave, MissionFailed, BanditSiegeStart, MissionSucceeded;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void WatchForWinLoseConditions();
};
void CQ_BreakSiegeScript::WatchForWinLoseConditions()
{
    CScriptThingVector attackers; attackers.count = 0;
    Game->GetAllThingsWithScriptName("BanditAttacker", attackers);
    bool leaderAdded = false;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        if (AreAllThingsInVectorDead(attackers) && Game->IsRegionLoaded("KnotholeGlade")) break;
        if (!leaderAdded && AttackedBanditLeader) {
            if (IsActiveThreadTerminating()) return;
            Game->GetAllThingsWithScriptName("BanditSiegeLeader", attackers);
            leaderAdded = true;
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating()) return;
    if (StartSecondWave && !MissionFailed) {
        Game->EntitySetAsKillable(Game->GetThingWithScriptName("ImprisonedBanditLeader"), false, false);
        MissionSucceeded = true;
    } else {
        if (BanditSiegeStart)
            while (!StartSecondWave) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (!MissionFailed) {
            Game->EntitySetAsKillable(Game->GetThingWithScriptName("ImprisonedBanditLeader"), false, false);
            MissionSucceeded = true;
        }
    }
}
int main()
{
    CGameScriptInterface game; CQ_BreakSiegeScript s; s.Game = &game;
    s.AttackedBanditLeader = true; s.StartSecondWave = true; s.MissionFailed = false; s.BanditSiegeStart = false;
    s.MissionSucceeded = false;
    s.WatchForWinLoseConditions();
    bool ok = s.MissionSucceeded && g_killable == 1 && g_leaderAdds == 1 && g_frames == 3;
    g_frames = 0; g_killable = 0; s.MissionSucceeded = false; s.MissionFailed = true; s.StartSecondWave = false;
    s.WatchForWinLoseConditions();                      // failed mission: no success
    ok = ok && !s.MissionSucceeded && g_killable == 0;
    g_frames = 0; g_endAt = 2; s.MissionFailed = false; s.StartSecondWave = true;
    s.WatchForWinLoseConditions();                      // thread ends before the attackers die
    ok = ok && !s.MissionSucceeded && g_killable == 0;
    if (!ok) { std::printf("00d20790_TEST FAIL\n"); return 1; }
    std::printf("00d20790_TEST PASS\n");
    return 0;
}