// Behaviour test for CV_FisticuffsClubScript::Main (0x00E82220).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CGameScriptInterface { public: void PostAddScriptedEntities() { Log("gpost"); } };
struct CV_FisticuffsClubScript {
    CGameScriptInterface* Game;
    void InitialiseVariables() { Log("init"); }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void AddEntityScriptBinding(const char* name) { Log("bind"); Log(name); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        InitialiseVariables();
        AddEntityScriptBinding("FisticuffsClubRing");
        AddEntityScriptBinding("FisticuffsMember");
        AddEntityScriptBinding("Tyler");
        AddEntityScriptBinding("RingFighter");
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        SpawnFunction("KeepVariablesUpToDate");
        SpawnFunction("ActivateFisticuffsAtNight");
    }
};
int main()
{
    g_log[0] = 0;
    CGameScriptInterface game;
    CV_FisticuffsClubScript s; s.Game = &game;
    s.Main();
    bool ok = std::strcmp(g_log, "init;bind;FisticuffsClubRing;bind;FisticuffsMember;bind;Tyler;bind;RingFighter;post;gpost;"
        "spawn;KeepVariablesUpToDate;spawn;ActivateFisticuffsAtNight;") == 0;
    if (!ok) { std::printf("00e82220_TEST FAIL\n"); return 1; }
    std::printf("00e82220_TEST PASS\n");
    return 0;
}