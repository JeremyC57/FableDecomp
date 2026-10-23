// Behaviour test for CV_KnotholeGladeGatesScript::EndMission (0x00EA1360): which things are removed, in what order.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing {
    const char* name;
    bool alive;
    bool IsAlive() const { return alive; }
};
struct CThingList {                      // stands in for std::vector<CScriptThing>
    CScriptThing items[8];
    unsigned int count;
    unsigned int size() const { return count; }
    CScriptThing& back() { return items[count - 1]; }
    void pop_back() { --count; }
};
static const char* const g_villagers[3] = { "V0", "V1", "V2" };
struct CGameScriptInterface {
    unsigned int villagers;
    bool guardAlive;
    void GetAllThingsWithScriptName(const char*, CThingList& list)
    {
        list.count = villagers;
        for (unsigned int i = 0; i < villagers; ++i) { list.items[i].name = g_villagers[i]; list.items[i].alive = true; }
    }
    CScriptThing GetThingWithScriptName(const char* name)
    {
        CScriptThing t;
        t.name = name;
        t.alive = std::strcmp(name, "KG_GateGuard") != 0 || guardAlive;
        return t;
    }
    void RemoveThing(const CScriptThing& t, bool, bool) { Log(t.name); }
};
struct CV_KnotholeGladeGatesScript {
    CGameScriptInterface* Game;
    bool MissionSucceeded;
    int terminateAt, checks;
    bool IsActiveThreadTerminating() { return ++checks == terminateAt; }
    void EndMission()
    {
        CThingList villagers;
        Game->GetAllThingsWithScriptName("WB_Villager", villagers);
        for (unsigned int i = 0; i < villagers.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(villagers.back(), false, true);
            villagers.pop_back();
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing runner = Game->GetThingWithScriptName("KGG_GuardRunner");
        Game->RemoveThing(runner, false, true);
        CScriptThing guard = Game->GetThingWithScriptName("KG_GateGuard");
        if (guard.IsAlive())
            Game->RemoveThing(guard, false, true);
        CScriptThing hornBlower = Game->GetThingWithScriptName("WB_HornBlower");
        Game->RemoveThing(hornBlower, false, true);
        MissionSucceeded = true;
    }
};
static bool Run(unsigned int villagers, bool guardAlive, int terminateAt, const char* expect, bool succeeded)
{
    g_log[0] = 0;
    CGameScriptInterface game;
    game.villagers = villagers;
    game.guardAlive = guardAlive;
    CV_KnotholeGladeGatesScript s;
    s.Game = &game;
    s.MissionSucceeded = false;
    s.terminateAt = terminateAt;
    s.checks = 0;
    s.EndMission();
    return std::strcmp(g_log, expect) == 0 && s.MissionSucceeded == succeeded;
}
int main()
{
    // the index counts up while the list shrinks: of three villagers only the last two are removed
    bool ok = Run(3, true, 0, "V2;V1;KGG_GuardRunner;KG_GateGuard;WB_HornBlower;", true)
        && Run(0, false, 0, "KGG_GuardRunner;WB_HornBlower;", true)
        && Run(3, true, 2, "V2;", false)
        && Run(1, true, 2, "V0;", false);
    if (!ok) { std::printf("00ea1360_TEST FAIL\n"); return 1; }
    std::printf("00ea1360_TEST PASS\n");
    return 0;
}
