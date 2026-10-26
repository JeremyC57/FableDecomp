// Behaviour test for CQ_WizardBattleScript::Main (0x00E1A990).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGameScriptInterface {
    void PostAddScriptedEntities() { Log("gpost"); }
    void CacheMusicSet(long t) { char b[16]; std::sprintf(b, "music%ld", t); Log(b); }
};
struct CQ_WizardBattleScript {
    CGameScriptInterface* Game;
    void Init() { Log("init"); }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void AddEntityScriptBinding(const char* name, int flags) { Log("bind"); Log(name); Log(flags ? "1" : "0"); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        Init();
        AddEntityScriptBinding("HKC_VillagerScreamer", 1);
        AddEntityScriptBinding("HKC_ScreamerPoint", 1);
        AddEntityScriptBinding("ChasingScreamer", 1);
        AddEntityScriptBinding("InitialScreamerPoint", 1);
        AddEntityScriptBinding("HKC_Maze", 1);
        AddEntityScriptBinding("EndGameBarrier", 1);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        Game->CacheMusicSet(0x33);
        Game->CacheMusicSet(0x34);
        SpawnFunction("WatchForTermination");
        SpawnFunction("DoMission");
    }
};
int main()
{
    g_log[0] = 0;
    CGameScriptInterface game;
    CQ_WizardBattleScript s; s.Game = &game;
    s.Main();
    bool ok = std::strcmp(g_log, "init;bind;HKC_VillagerScreamer;1;bind;HKC_ScreamerPoint;1;bind;ChasingScreamer;1;bind;InitialScreamerPoint;1;bind;HKC_Maze;1;bind;EndGameBarrier;1;post;gpost;music51;music52;spawn;WatchForTermination;spawn;DoMission;") == 0;
    if (!ok) { std::printf("00e1a990_TEST FAIL\n"); return 1; }
    std::printf("00e1a990_TEST PASS\n");
    return 0;
}