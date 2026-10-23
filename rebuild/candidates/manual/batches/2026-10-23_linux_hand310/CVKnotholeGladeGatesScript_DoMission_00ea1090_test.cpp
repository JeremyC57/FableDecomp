// Behaviour test for CV_KnotholeGladeGatesScript::DoMission (0x00EA1090): the call sequence through a logging game stub.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { const char* name; };
struct CGameScriptInterface {
    int framesUntilLoaded;
    void NewScriptFrame() { Log("frame"); }
    bool IsLevelLoaded(const char* level) { Log(level); return framesUntilLoaded-- <= 0; }
    CScriptThing GetThingWithScriptName(const char* name) { CScriptThing t; t.name = name; return t; }
    void CloseDoor(const CScriptThing& t) { Log("close"); Log(t.name); }
};
struct CV_KnotholeGladeGatesScript {
    CGameScriptInterface* Game;
    bool MissionFailed;
    int terminateAt, checks;
    CScriptThing TheGate;
    bool IsActiveThreadTerminating() { return ++checks == terminateAt; }
    void AddSpawnedFunction(const char* func, const char* region) { Log(func); Log(region); }
    void PhaseOne() { Log("PhaseOne"); }
    void EndMission() { Log("EndMission"); }
    void DoMission()
    {
        while (!Game->IsLevelLoaded("KnotholeGlade")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        TheGate = Game->GetThingWithScriptName("KG_Gate");
        Game->CloseDoor(TheGate);
        AddSpawnedFunction("WatchForGates", "KnotholeGlade");
        AddSpawnedFunction("WatchForCutscene1", "KnotholeGlade");
        PhaseOne();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!MissionFailed)
            EndMission();
    }
};
static bool Run(int frames, bool failed, int terminateAt, const char* expect)
{
    g_log[0] = 0;
    CGameScriptInterface game;
    game.framesUntilLoaded = frames;
    CV_KnotholeGladeGatesScript s;
    s.Game = &game;
    s.MissionFailed = failed;
    s.terminateAt = terminateAt;
    s.checks = 0;
    s.DoMission();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(0, false, 0, "KnotholeGlade;close;KG_Gate;WatchForGates;KnotholeGlade;WatchForCutscene1;KnotholeGlade;PhaseOne;frame;EndMission;")
        && Run(1, true, 0, "KnotholeGlade;frame;KnotholeGlade;close;KG_Gate;WatchForGates;KnotholeGlade;WatchForCutscene1;KnotholeGlade;PhaseOne;frame;")
        && Run(2, false, 1, "KnotholeGlade;frame;")
        && Run(0, false, 1, "KnotholeGlade;");
    if (!ok) { std::printf("00ea1090_TEST FAIL\n"); return 1; }
    std::printf("00ea1090_TEST PASS\n");
    return 0;
}
