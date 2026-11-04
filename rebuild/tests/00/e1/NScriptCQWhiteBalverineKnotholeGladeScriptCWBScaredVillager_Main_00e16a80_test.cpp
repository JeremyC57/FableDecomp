// Behaviour test for CWB_ScaredVillager::Main (0x00E16A80): predicate over Me registered first, one frame, then the
// scripted-thing handle lives across the wait loop until the thread terminates.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public: const char* Name; };
struct CIsThingConscious {
    CScriptThing Thing;
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { Log("Pred+"); }
    ~CIsThingConscious() { Log("Pred-"); }
};
class CScriptGameResourceObjectScriptedThingBase { public:
    CScriptGameResourceObjectScriptedThingBase() { Log("Handle+"); }
    ~CScriptGameResourceObjectScriptedThingBase() { Log("Handle-"); }
};
class CGameScriptInterface { public: void NewScriptFrame() { Log("Frame"); } };
class CEntityScriptBase { public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    long TerminateAfter, Calls;
    void SetPredicate(CIsThingConscious& p) { Log(p.Thing.Name); }
    bool IsActiveThreadTerminating() { return ++Calls > TerminateAfter; }
};
struct CWB_ScaredVillager : CEntityScriptBase { void Main(); };
void CWB_ScaredVillager::Main()
{
    {
        CIsThingConscious predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
int main()
{
    CGameScriptInterface g; CWB_ScaredVillager s; s.Game = &g; s.Me.Name = "VILLAGER";
    s.TerminateAfter = 3; s.Calls = 0; g_log[0] = 0;
    s.Main();
    int bad = std::strcmp(g_log, "Pred+;VILLAGER;Pred-;Frame;Handle+;Frame;Frame;Handle-;") != 0;
    s.TerminateAfter = 0; s.Calls = 0; g_log[0] = 0;
    s.Main();
    bad |= std::strcmp(g_log, "Pred+;VILLAGER;Pred-;Frame;") != 0;
    if (bad) { std::printf("00e16a80_TEST FAIL\n"); return 1; }
    std::printf("00e16a80_TEST PASS\n");
    return 0;
}