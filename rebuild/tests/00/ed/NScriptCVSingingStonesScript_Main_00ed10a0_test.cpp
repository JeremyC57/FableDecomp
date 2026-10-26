// Behaviour test for CV_SingingStonesScript::Main (0x00ED10A0).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CGameScriptInterface { public:
    int framesUntilLoaded;
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "Witchwood2") == 0 && framesUntilLoaded <= 0; }
    void NewScriptFrame() { Log("frame"); --framesUntilLoaded; }
    void PostAddScriptedEntities() { Log("gpost"); }
};
struct CV_SingingStonesScript {
    CGameScriptInterface* Game;
    int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void AddEntityScriptBinding(const char* name) { Log("bind"); Log(name); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        while (!Game->IsRegionLoaded("Witchwood2")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        AddEntityScriptBinding("SingingStone");
        AddEntityScriptBinding("ManWithDoorName");
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        SpawnFunction("WatchForCompleteTune");
        SpawnFunction("WatchForSpokenToDemonDoors");
        SpawnFunction("WatchForRegionLeaving");
    }
};
static bool Run(int frames, int termAfter, const char* expect)
{
    g_log[0] = 0;
    CGameScriptInterface game; game.framesUntilLoaded = frames;
    CV_SingingStonesScript s; s.Game = &game; s.termAfter = termAfter;
    s.Main();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(2, 100, "frame;frame;bind;SingingStone;bind;ManWithDoorName;post;gpost;spawn;WatchForCompleteTune;"
                          "spawn;WatchForSpokenToDemonDoors;spawn;WatchForRegionLeaving;")
        && Run(3, 1, "frame;frame;")
        && Run(0, 0, "");
    if (!ok) { std::printf("00ed10a0_TEST FAIL\n"); return 1; }
    std::printf("00ed10a0_TEST PASS\n");
    return 0;
}