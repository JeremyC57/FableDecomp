// Behaviour test for NScript::CV_BeardyBaldyScript::Main (0x00E4FA20): bind BB_BeardyBaldyMan, wait for the
// BowerstoneSlumsWarehouses region-loaded message (other regions are skipped), spawn the seven watchers, activate the quest
// once, send the hero to Beardy Baldy, wait for the quest to finish (or complete with the region unloaded), clear the marker
// and information on whichever of the two NPCs is alive, then complete / remove / deactivate the quest.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { bool alive; bool IsAlive() const { return alive; } };
struct CGame {
    const char* regions[4]; int regionIdx; int frames; bool regionLoaded; int finishAfter;
    bool barberAlive, beardyAlive;
    void PostAddScriptedEntities() { Log("gpost"); }
    void NewScriptFrame() { ++frames; }
    bool MsgOnRegionLoaded(const char*& name) {       // one region message every other frame
        if (frames % 2 == 0 && regions[regionIdx]) { name = regions[regionIdx++]; return true; }
        return false;
    }
    bool IsRegionLoaded(const char*) { return regionLoaded; }
    CScriptThing GetThingWithScriptName(const char*) { CScriptThing t; t.alive = beardyAlive; return t; }
    CScriptThing GetHero() { CScriptThing t; t.alive = true; return t; }
    CScriptThing GetNearestWithScriptName(CScriptThing, const char*) { CScriptThing t; t.alive = barberAlive; return t; }
    void MiniMapRemoveMarker(CScriptThing) { Log("rmmarker"); }
    void ClearThingHasInformation(CScriptThing) { Log("clearinfo"); }
    const char* GetActiveQuestName() { return "V_BeardyBaldy"; }
    void SetQuestAsCompleted(const char* q, bool, bool, bool) { Log("complete"); Log(q); }
    void RemoveQuestCardFromHero(const char* q) { Log("removecard"); Log(q); }
    void DeactivateQuestLater(const char* q, long) { Log("deactivate"); Log(q); }
};
struct CV_BeardyBaldyScript {
    CGame* Game; bool QuestFinished, QuestComplete, QuestActivated; int termAfter, termCalls;
    bool IsActiveThreadTerminating() { return ++termCalls > termAfter; }
    void Bind(const char* n, int f) { Log("bind"); Log(n); Log(f ? "1" : "0"); }
    void PostAddScriptedEntities() { Log("post"); }
    void Spawn(const char* n) { Log("spawn"); Log(n); }
    void GoTalkToBeardyBaldy() { Log("gotalk"); }
    void Main()
    {
        Bind("BB_BeardyBaldyMan", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        const char* regionName = "";
        if (IsActiveThreadTerminating()) return;
        for (;;) {
            while (!Game->MsgOnRegionLoaded(regionName)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            if (std::strcmp(regionName, "BowerstoneSlumsWarehouses") == 0) break;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (IsActiveThreadTerminating()) return;
        Spawn("WatchForQuestFinished"); Spawn("WatchForQuestCardConditions"); Spawn("WatchForBarberLadyDeath");
        Spawn("WatchForBeardyBaldyDeath"); Spawn("WatchForNewHairdo"); Spawn("WatchForNewBeard"); Spawn("WatchForNewTash");
        if (!QuestActivated) { if (IsActiveThreadTerminating()) return; QuestActivated = true; }
        GoTalkToBeardyBaldy();
        while (!QuestFinished && (!QuestComplete || Game->IsRegionLoaded("BowerstoneSlumsWarehouses"))) {
            Game->NewScriptFrame();
            if (Game->frames >= Game->finishAfter) QuestFinished = true;
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing beardyBaldy = Game->GetThingWithScriptName("BB_BeardyBaldyMan");
        CScriptThing barber = Game->GetNearestWithScriptName(Game->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
        if (barber.IsAlive()) { if (IsActiveThreadTerminating()) return; Game->MiniMapRemoveMarker(barber); Game->ClearThingHasInformation(barber); }
        if (beardyBaldy.IsAlive()) { if (IsActiveThreadTerminating()) return; Game->MiniMapRemoveMarker(beardyBaldy); Game->ClearThingHasInformation(beardyBaldy); }
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
static bool Run(bool barberAlive, bool beardyAlive, bool questComplete, bool regionLoaded, int termAfter, const char* expected, int minFrames)
{
    CGame g; g.regions[0] = "Lookout"; g.regions[1] = "BowerstoneSlumsWarehouses"; g.regions[2] = 0; g.regions[3] = 0;
    g.regionIdx = 0; g.frames = 0; g.regionLoaded = regionLoaded; g.finishAfter = 12; g.barberAlive = barberAlive; g.beardyAlive = beardyAlive;
    CV_BeardyBaldyScript s; s.Game = &g; s.QuestFinished = false; s.QuestComplete = questComplete; s.QuestActivated = false;
    s.termAfter = termAfter; s.termCalls = 0;
    g_log[0] = 0;
    s.Main();
    if (std::strcmp(g_log, expected) != 0) { std::printf("log: %s\n", g_log); return false; }
    return g.frames >= minFrames;
}
int main()
{
    const char* head = "bind;BB_BeardyBaldyMan;0;post;gpost;spawn;WatchForQuestFinished;spawn;WatchForQuestCardConditions;"
        "spawn;WatchForBarberLadyDeath;spawn;WatchForBeardyBaldyDeath;spawn;WatchForNewHairdo;spawn;WatchForNewBeard;spawn;WatchForNewTash;gotalk;";
    char e1[1024]; std::strcpy(e1, head); std::strcat(e1, "rmmarker;clearinfo;rmmarker;clearinfo;complete;V_BeardyBaldy;removecard;V_BeardyBaldy;deactivate;V_BeardyBaldy;");
    char e2[1024]; std::strcpy(e2, head); std::strcat(e2, "rmmarker;clearinfo;complete;V_BeardyBaldy;removecard;V_BeardyBaldy;deactivate;V_BeardyBaldy;");
    char e3[1024]; std::strcpy(e3, head); std::strcat(e3, "complete;V_BeardyBaldy;removecard;V_BeardyBaldy;deactivate;V_BeardyBaldy;");
    bool ok = true;
    ok = ok && Run(true, true, false, true, 1000, e1, 12);     // both alive: both cleared, waited for QuestFinished
    ok = ok && Run(false, true, false, true, 1000, e2, 12);    // only Beardy Baldy alive
    ok = ok && Run(false, false, true, false, 1000, e3, 2);    // complete + region unloaded: no finish wait
    ok = ok && Run(true, true, false, true, 3, "bind;BB_BeardyBaldyMan;0;post;gpost;", 0);   // terminated in the region wait
    if (!ok) { std::printf("00e4fa20_TEST FAIL\n"); return 1; }
    std::printf("00e4fa20_TEST PASS\n");
    return 0;
}
