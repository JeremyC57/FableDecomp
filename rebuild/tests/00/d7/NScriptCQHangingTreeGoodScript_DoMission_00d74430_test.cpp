// Behaviour test for NScript::CQ_HangingTreeGoodScript::DoMission (0x00D74430): a standalone model of the escort state machine --
// three watcher threads, wait for BowerstoneJail, opening cutscene 0, state 1 spawns the party at HTG_PartySpawnPoint (waypoint from
// its data string) and starts DoConversations, state 2 advances the waypoints (ending in state 7 after waypoint 14), then the final
// round and, unless failed, closing cutscene 1 and success.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
static int g_frames = 0, g_endAt = 100000, g_jailAt = 2, g_spawned = 0, g_parties = 0, g_final = 0, g_cut0 = 0, g_cut1 = 0, g_conv = 0;
struct Thing { std::string data; };
static bool* g_arrive = 0;          // the party reaches its waypoint every frame
struct Game {
    void NewScriptFrame() { ++g_frames; if (g_arrive) *g_arrive = true; }
    bool IsLevelLoaded(const char* n) { return !std::strcmp(n, "BowerstoneJail") && g_frames >= g_jailAt; }
    Thing GetThingWithScriptName(const char* n) { Thing t; t.data = !std::strcmp(n, "HTG_PartySpawnPoint") ? "3" : ""; return t; }
};
struct Script {
    Game* G; long HTG_State, CurrentWayPoint, CurrentGuards; Thing PartySpawnPoint;
    bool SwitchingRegions, PartyDefeated, SpawnHere, ReassignLeaderNow, AtWayPoint, PartySuccessfullySetUp, PartyReadyToTransition;
    bool MissionFailed, MissionSucceeded;
    bool Term() { return g_frames >= g_endAt; }
    void Spawn(const char* n) { if (!std::strcmp(n, "MonitorRegionPresence") || !std::strcmp(n, "WatchForMissionEnd") || !std::strcmp(n, "MonitorHeroProximity")) ++g_spawned; if (!std::strcmp(n, "DoConversations")) ++g_conv; }
    void DoCutscene(long n) { if (n == 0) ++g_cut0; else ++g_cut1; }
    bool UpdateWayPoint() { return CurrentWayPoint <= 14; }
    void SpawnParty(long n, Thing&) { g_parties += (int)n; }
    void FinalRound() { ++g_final; }
    void DoMission()
    {
        Spawn("MonitorRegionPresence"); Spawn("WatchForMissionEnd"); Spawn("MonitorHeroProximity");
        while (!G->IsLevelLoaded("BowerstoneJail")) { G->NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        DoCutscene(0);
        PartySpawnPoint = G->GetThingWithScriptName("HTG_PartySpawnPoint");
        HTG_State = 1;
        UpdateWayPoint();
        while (HTG_State != 7) {
            G->NewScriptFrame(); if (Term()) return;
            while (HTG_State == 1) {
                G->NewScriptFrame(); if (Term()) return;
                while (SwitchingRegions) { G->NewScriptFrame(); if (Term()) return; }
                if (!PartyDefeated) {
                    if (SpawnHere) {
                        PartySpawnPoint = G->GetThingWithScriptName("HTG_PartySpawnPoint");
                        CurrentWayPoint = std::atol(PartySpawnPoint.data.c_str());
                        SpawnParty(CurrentGuards, PartySpawnPoint);
                        Spawn("DoConversations");
                    }
                    HTG_State = 2;
                }
            }
            while (HTG_State == 2) {
                G->NewScriptFrame(); if (Term()) return;
                if (ReassignLeaderNow) { PartySuccessfullySetUp = false; if (SwitchingRegions) HTG_State = 1; }
                else if (AtWayPoint) {
                    ++CurrentWayPoint;
                    if (!UpdateWayPoint()) { if (CurrentWayPoint > 14) HTG_State = 7; else PartyReadyToTransition = true; }
                    AtWayPoint = false;
                }
                if (SwitchingRegions) HTG_State = 1;
            }
        }
        if (Term()) return;
        FinalRound();
        G->NewScriptFrame(); if (Term()) return;
        G->NewScriptFrame(); if (Term()) return;
        if (MissionFailed) return;
        DoCutscene(1);
        MissionSucceeded = true;
    }
};
int main()
{
    Game g; Script s; std::memset((void*)&s.HTG_State, 0, sizeof(long) * 3);
    s.G = &g; s.CurrentGuards = 4; s.SwitchingRegions = s.PartyDefeated = s.ReassignLeaderNow = s.AtWayPoint = false; s.SpawnHere = true;
    s.PartySuccessfullySetUp = s.PartyReadyToTransition = s.MissionFailed = s.MissionSucceeded = false;
    g_arrive = &s.AtWayPoint;
    s.DoMission();
    bool ok = g_spawned == 3 && g_cut0 == 1 && g_parties == 4 && g_conv == 1 && s.CurrentWayPoint == 15 && s.HTG_State == 7
           && g_final == 1 && g_cut1 == 1 && s.MissionSucceeded;
    g_frames = 0; g_endAt = 1;                          // terminating while waiting for the jail: nothing else runs
    Script t = s; g_arrive = &t.AtWayPoint; t.MissionSucceeded = false; t.DoMission();
    ok = ok && g_cut0 == 1 && !t.MissionSucceeded;
    std::printf(ok ? "00d74430_TEST PASS\n" : "00d74430_TEST FAIL\n");
    return ok ? 0 : 1;
}