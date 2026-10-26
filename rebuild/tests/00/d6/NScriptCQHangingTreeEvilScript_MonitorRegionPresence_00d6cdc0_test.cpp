#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeEvilScript::MonitorRegionPresence -- record BowerstoneJail as visited; on every
// region change wait for the new region, then spawn there only if it was never visited (recording it), flag the
// bandits for removal when going back, and fail the mission when the party spawn point is gone
static int g_frames = 0, g_endAt = 5, g_changeAt = 3, g_loadMsgAt = 4; static bool g_spawnPointAlive = true;
static const char* g_newLevel = "DesertedFarm";
struct CNames {
    const char* Items[8]; unsigned int Count;
    CNames() : Count(0) { }
    void push_back(const char* s) { Items[Count++] = s; }
    unsigned int size() const { return Count; }
    const char* operator[](unsigned int i) const { return Items[i]; }
    const char* back() const { return Items[Count - 1]; }
};
class CScriptThing { public: bool alive; bool IsAlive() const { return alive; } };
struct CQ_HangingTreeEvilScript;
class CGameScriptInterface { public:
    CQ_HangingTreeEvilScript* Owner;
    void NewScriptFrame() const;
    bool IsRegionLoaded(const char*) const { return g_frames < g_changeAt; }
    bool MsgOnRegionLoaded(const char*) const { return g_frames >= g_loadMsgAt; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.alive = !std::strcmp(n, "HTE_PartySpawnPoint") && g_spawnPointAlive; return t; }
};
struct CQ_HangingTreeEvilScript {
    CGameScriptInterface* Game; const char* CurrentLevel; CNames AreasSpawnedIn; const char* ReasonForFailing;
    bool SwitchingRegions, SpawnHere, RemoveBandits, MissionFailed, SawRemoveBandits;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void MonitorRegionPresence();
};
void CGameScriptInterface::NewScriptFrame() const { if (++g_frames == g_changeAt) Owner->CurrentLevel = g_newLevel; }
void CQ_HangingTreeEvilScript::MonitorRegionPresence()
{
    CurrentLevel = "BowerstoneJail";
    AreasSpawnedIn.push_back(CurrentLevel);
    if (IsActiveThreadTerminating()) return;
    do {
        while (!Game->IsRegionLoaded(CurrentLevel)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        SwitchingRegions = false;
        while (Game->IsRegionLoaded(CurrentLevel)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        SwitchingRegions = true;
        while (!Game->MsgOnRegionLoaded(CurrentLevel)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        CScriptThing spawnPoint = Game->GetThingWithScriptName("HTE_PartySpawnPoint");
        if (spawnPoint.IsAlive()) {
            SpawnHere = true;
            for (unsigned int i = 0; i < AreasSpawnedIn.size(); ++i) {
                if (!std::strcmp(CurrentLevel, AreasSpawnedIn[i])) SpawnHere = false;
                if (!SpawnHere && std::strcmp(CurrentLevel, AreasSpawnedIn.back())) RemoveBandits = true;
            }
            if (SpawnHere) AreasSpawnedIn.push_back(CurrentLevel);
        } else {
            SpawnHere = false;
            ReasonForFailing = "TEXT_QST_029_FAILURE_LEFT_MAIN_ROUTE";
            MissionFailed = true;
        }
        if (RemoveBandits) { SawRemoveBandits = true; RemoveBandits = false; }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
static void Reset(CQ_HangingTreeEvilScript& s)
{
    g_frames = 0; s.AreasSpawnedIn.Count = 0; s.SwitchingRegions = false; s.SpawnHere = false; s.RemoveBandits = false;
    s.MissionFailed = false; s.SawRemoveBandits = false; s.ReasonForFailing = "";
}
int main()
{
    CGameScriptInterface game; CQ_HangingTreeEvilScript s; game.Owner = &s; s.Game = &game;
    Reset(s); s.MonitorRegionPresence();               // moved on to a new region: spawn there and record it
    bool ok = s.AreasSpawnedIn.size() == 2 && !std::strcmp(s.AreasSpawnedIn.back(), "DesertedFarm") && s.SpawnHere
           && s.SwitchingRegions && !s.MissionFailed && !s.SawRemoveBandits;
    Reset(s); g_spawnPointAlive = false; s.MonitorRegionPresence();   // spawn point gone: mission failed
    ok = ok && s.MissionFailed && !s.SpawnHere && !std::strcmp(s.ReasonForFailing, "TEXT_QST_029_FAILURE_LEFT_MAIN_ROUTE");
    Reset(s); g_spawnPointAlive = true; g_newLevel = "BowerstoneJail"; s.MonitorRegionPresence();   // back to a visited region
    ok = ok && !s.SpawnHere && s.AreasSpawnedIn.size() == 1 && !s.MissionFailed;
    if (!ok) { std::printf("00d6cdc0_TEST FAIL\n"); return 1; }
    std::printf("00d6cdc0_TEST PASS\n");
    return 0;
}