// Behaviour test for CV_MayorsInvitationScript::Main (0x00EB6D70).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CGameScriptInterface { public: void PostAddScriptedEntities() { Log("gpost"); } };
struct CV_MayorsInvitationScript {
    CGameScriptInterface* Game;
    bool HeroHasDefeatedThunder, HeroHasLostToThunder;
    int WhichQuest;
    bool IsActiveThreadTerminating() { return false; }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void AddEntityScriptBinding(const char* name) { Log("bind"); Log(name); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        if (WhichQuest == 5 && !HeroHasDefeatedThunder && !HeroHasLostToThunder) {
            if (IsActiveThreadTerminating()) return;
            SpawnFunction("MonitorThunderHeroDuel");
        }
        AddEntityScriptBinding("LadyGrey");
        AddEntityScriptBinding("WiseVillager");
        AddEntityScriptBinding("MI_InfoKeeper");
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        SpawnFunction("WatchForTermination");
        SpawnFunction("WatchExposeMayorProgress");
        SpawnFunction("DoMission");
    }
};
static const char* kTail = "bind;LadyGrey;bind;WiseVillager;bind;MI_InfoKeeper;post;gpost;spawn;WatchForTermination;spawn;WatchExposeMayorProgress;spawn;DoMission;";
static bool Run(int which, bool won, bool lost, bool monitor)
{
    g_log[0] = 0;
    CGameScriptInterface game;
    CV_MayorsInvitationScript s; s.Game = &game; s.WhichQuest = which;
    s.HeroHasDefeatedThunder = won; s.HeroHasLostToThunder = lost;
    s.Main();
    char expect[1024]; expect[0] = 0;
    if (monitor) std::strcat(expect, "spawn;MonitorThunderHeroDuel;");
    std::strcat(expect, kTail);
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(5, false, false, true) && Run(5, true, false, false)
        && Run(5, false, true, false) && Run(4, false, false, false);
    if (!ok) { std::printf("00eb6d70_TEST FAIL\n"); return 1; }
    std::printf("00eb6d70_TEST PASS\n");
    return 0;
}