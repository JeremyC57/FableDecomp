// Behaviour test for 0x00EEEC00 NScript::CTestTheInventoryScript::TestForLampOn: wait until the hero's lamp is lit, show the
// lamp-on message for 3 s, pause 3 s and spawn the "TestForLampOff" thread (this script, TestForLampOff, id 0, region "").
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* Text; CCharString(const char* t, long = -1) : Text(t) {} };
struct CTestTheInventoryScript;
struct CSpawnedFunc {
    CCharString Name; unsigned long Id; void (CTestTheInventoryScript::*Func)(); CTestTheInventoryScript* PClass;
    CSpawnedFunc(const CCharString& n, unsigned long id, CTestTheInventoryScript* p, void (CTestTheInventoryScript::*f)())
        : Name(n), Id(id), Func(f), PClass(p) { }
};
class CGameScriptInterface { public:
    int Frames; int LitAt;
    void NewScriptFrame() { ++Frames; }
    void ShowOnScreenMessage(const CCharString& m, float s) const { char x[96]; std::sprintf(x, "msg(%s,%d)", m.Text, (int)s); Log(x); }
    bool IsHeroHandLampLit() const { return Frames >= LitAt; }
    void Pause(float s) const { char x[32]; std::sprintf(x, "pause(%d)", (int)s); Log(x); }
};
struct CTestTheInventoryScript {
    CGameScriptInterface* Game; int TermAt, TermCalls; CSpawnedFunc* Spawned; const char* Region; int OffRuns;
    bool IsActiveThreadTerminating() { return TermAt && ++TermCalls >= TermAt; }
    void AddSpawnedFunction(CSpawnedFunc* f, const CCharString& r) { Spawned = f; Region = r.Text; Log("spawn"); }
    void TestForLampOff() { ++OffRuns; }
    void TestForLampOn()
    {
        while (!Game->IsHeroHandLampLit()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->ShowOnScreenMessage("You've turned the lamp on. Bully for you.", 3.0f);
        Game->Pause(3.0f);
        AddSpawnedFunction(new CSpawnedFunc("TestForLampOff", 0, this, &CTestTheInventoryScript::TestForLampOff),
                           CCharString(""));
    }
};

int main()
{
    CGameScriptInterface game; game.Frames = 0; game.LitAt = 3;
    CTestTheInventoryScript s; s.Game = &game; s.TermAt = 0; s.TermCalls = 0; s.Spawned = 0; s.Region = 0; s.OffRuns = 0;
    s.TestForLampOn();
    int ok = !std::strcmp(g_log, "msg(You've turned the lamp on. Bully for you.,3);pause(3);spawn;") && game.Frames == 3
             && s.Spawned && !std::strcmp(s.Spawned->Name.Text, "TestForLampOff") && s.Spawned->Id == 0
             && s.Spawned->PClass == &s && s.Region && !s.Region[0];
    if (ok) { (s.Spawned->PClass->*s.Spawned->Func)(); ok = s.OffRuns == 1; }
    delete s.Spawned;
    g_log[0] = 0; game.Frames = 0; s.Spawned = 0; s.TermAt = 1; s.TermCalls = 0;
    s.TestForLampOn();
    ok = ok && g_log[0] == 0 && !s.Spawned && game.Frames == 1;
    if (!ok) { std::printf("00eeec00_TEST FAIL\n"); return 1; }
    std::printf("00eeec00_TEST PASS\n");
    return 0;
}