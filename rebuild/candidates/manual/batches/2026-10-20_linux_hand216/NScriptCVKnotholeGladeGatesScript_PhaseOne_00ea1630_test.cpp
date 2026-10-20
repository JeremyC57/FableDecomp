// Behaviour test for 0x00EA1630 CV_KnotholeGladeGatesScript::PhaseOne: spawns the banter thread with the right name,
// region and method, runs the attack phases, then waits frame by frame for the gates; termination aborts early.
#include <cstdio>
#include <cstring>
struct CCharString { char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
struct CSpawnedFuncBase {
    char name[32]; unsigned long id;
    virtual ~CSpawnedFuncBase() { }
    virtual void SuspendableProcess() = 0;
    CSpawnedFuncBase(const CCharString& n, unsigned long i) : id(i) { std::strcpy(name, n.text); }
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)(); T* PClass;
    CSpawnedFunc(const CCharString& n, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(n, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
struct CV_KnotholeGladeGatesScript;
struct CGameScriptInterface { CV_KnotholeGladeGatesScript* owner; int frames; virtual void NewScriptFrame(); };
struct CV_KnotholeGladeGatesScript {
    CGameScriptInterface* Game; bool GatesOpened, BalvsKilled;
    int terminateAfter, checks, attacks, controls, banters; CSpawnedFuncBase* spawned; char region[32];
    bool IsActiveThreadTerminating() { return ++checks > terminateAfter; }
    void AddSpawnedFunction(CSpawnedFuncBase* f, const CCharString& r) { spawned = f; std::strcpy(region, r.text); }
    void DoPanicBanter() { ++banters; }
    void DoInitialBalverineAttack() { ++attacks; }
    void ControlMainBalverines() { ++controls; }
    void PhaseOne()
    {
        AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("DoPanicBanter", 0, this,
                                                                        &CV_KnotholeGladeGatesScript::DoPanicBanter),
                           "KnotholeGlade");
        DoInitialBalverineAttack();
        for (long round = 0; round < 2; ++round) {
            if (IsActiveThreadTerminating()) return;
            ControlMainBalverines();
        }
        if (IsActiveThreadTerminating()) return;
        BalvsKilled = true;
        while (!GatesOpened) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
};
void CGameScriptInterface::NewScriptFrame() { if (++frames == 3) owner->GatesOpened = true; }
static void Reset(CV_KnotholeGladeGatesScript& s, CGameScriptInterface& g, int term)
{
    g.owner = &s; g.frames = 0; s.Game = &g; s.GatesOpened = s.BalvsKilled = false;
    s.terminateAfter = term; s.checks = s.attacks = s.controls = s.banters = 0; s.spawned = 0; s.region[0] = 0;
}
int main()
{
    CGameScriptInterface g; CV_KnotholeGladeGatesScript s;
    Reset(s, g, 100);
    s.PhaseOne();
    bool ok = s.spawned && std::strcmp(s.spawned->name, "DoPanicBanter") == 0 && std::strcmp(s.region, "KnotholeGlade") == 0
        && s.attacks == 1 && s.controls == 2 && s.BalvsKilled && s.GatesOpened && g.frames == 3 && s.checks == 7;
    s.spawned->SuspendableProcess();
    ok = ok && s.banters == 1;
    delete s.spawned;
    Reset(s, g, 1);                                     // terminate at the second round check
    s.PhaseOne();
    ok = ok && s.controls == 1 && !s.BalvsKilled && g.frames == 0;
    delete s.spawned;
    if (!ok) { std::printf("00ea1630_TEST FAIL\n"); return 1; }
    std::printf("00ea1630_TEST PASS\n");
    return 0;
}
