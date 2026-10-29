// Behaviour test for 0x00F08E90 NScript::CQ_AmbushTradersScript::CAT_Sentry::Init(): the source's body over a logging
// game interface and parent script (three bandits, one dead), checked as an event log, plus an early thread stop.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
static char gLog[4096];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }
static char gPool[16][128];
static int gPoolNext = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { (void)length; }
    ~CCharString() { PStringData = 0; }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right)
{
    char* out = gPool[gPoolNext++ & 15];
    std::sprintf(out, "%s%s", left.PStringData, right.PStringData);
    return CCharString(out);
}
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
static int gThingsAlive = 0;
class CScriptThing { public:
    int Id;
    bool Alive;
    C3DVector Pos;
    struct InlineCopy { };
    CScriptThing(int id, bool alive) : Id(id), Alive(alive), Pos(1.0f * id, 2.0f, 3.0f) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other) : Id(other.Id), Alive(other.Alive), Pos(other.Pos) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other, InlineCopy) : Id(other.Id), Alive(other.Alive), Pos(other.Pos) { ++gThingsAlive; }
    ~CScriptThing() { --gThingsAlive; }
    const C3DVector& GetPos() const { return Pos; }
    bool IsAlive() const { return Alive; }
};
typedef long EScriptingStateGroups;
class CGameScriptInterfaceBase { public:
    CScriptThing Hero;
    CGameScriptInterfaceBase() : Hero(0, true) { }
    CScriptThing& GetHero() { return Hero; }
    void EntitySetThingAsEnemyOfThing(const CScriptThing& thing, const CScriptThing& enemy) const { char t[64]; std::sprintf(t, "Enemy %d>%d", thing.Id, enemy.Id); Log(t); }
    void SetWanderCentrePoint(CScriptThing thing, C3DVector centre) { char t[64]; std::sprintf(t, "Centre %d %g,%g,%g", thing.Id, centre.X, centre.Y, centre.Z); Log(t); }
    void SetWanderMinDistance(CScriptThing thing, float distance) { char t[64]; std::sprintf(t, "Min %d %g", thing.Id, distance); Log(t); }
    void SetWanderMaxDistance(CScriptThing thing, float distance) { char t[64]; std::sprintf(t, "Max %d %g", thing.Id, distance); Log(t); }
    void SetScriptingStateGroup(CScriptThing thing, EScriptingStateGroups group) { char t[64]; std::sprintf(t, "Group %d %ld", thing.Id, group); Log(t); }
};
class CScriptDef { public:
    float AmbushTradersSentryWanderDistance;
};
static CScriptDef gDef = { 25.0f };
static CScriptDef* pGlobalScriptDef = &gDef;
class CSpawnedFuncBase { public:
    unsigned long ID;
    char Name[128];
    CSpawnedFuncBase(const CCharString& name, unsigned long id) : ID(id) { std::strcpy(Name, name.PStringData); }
    virtual ~CSpawnedFuncBase() { }
    virtual void SuspendableProcess() = 0;
};
template <class TClass, class TPar1> class CSpawnedFunc1 : public CSpawnedFuncBase { public:
    void (TClass::*Func)(TPar1);
    TClass* PClass;
    TPar1   Par1;
    CSpawnedFunc1(const CCharString& name, unsigned long id, TClass* pclass, void (TClass::*func)(TPar1), TPar1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};
class CScriptBase { public:
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& name)
    {
        char t[200]; std::sprintf(t, "Spawn %s id%lu '%s'", func->Name, func->ID, name.PStringData); Log(t);
        func->SuspendableProcess();
        delete func;
    }
};
template <class T> class CThingList { public:              // the test's stand-in for the parent's STLport vector
    T* Items[4];
    unsigned int Count;
    unsigned int size() const { return Count; }
    T& operator[](unsigned int n) { return *Items[n]; }
};
static int gTerminateAfter = 1000;
static int gTerminateChecks = 0;
class CEntityScriptBase { public:
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
    CEntityScriptBase() : Game(0), Me(9, true) { }
    virtual ~CEntityScriptBase() { }
    bool IsActiveThreadTerminating() const { return ++gTerminateChecks > gTerminateAfter; }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CScriptBase { public:
    CThingList<CScriptThing> AllBandits;
    void WatchForEnemyDeath(CScriptThing thing) { char t[64]; std::sprintf(t, "WatchForEnemyDeath(%d)", thing.Id); Log(t); }
    void WatchForHeroAttackOnSentry(CScriptThing thing) { char t[64]; std::sprintf(t, "WatchForHeroAttackOnSentry(%d)", thing.Id); Log(t); }
    class CAT_Sentry;
};
class CQ_AmbushTradersScript::CAT_Sentry : public CEntityScriptBase {
public:
    CQ_AmbushTradersScript* ParentClass;
    CQ_SunnyvaleMasterData* PMasterData;
    bool                    SentrySeenSpy;
    void Init();
};
}

void NScript::CQ_AmbushTradersScript::CAT_Sentry::Init()
{
    Game->SetWanderCentrePoint(CScriptThing(Me, CScriptThing::InlineCopy()), Me.GetPos());
    Game->SetWanderMinDistance(CScriptThing(Me, CScriptThing::InlineCopy()), 0.0f);
    Game->SetWanderMaxDistance(CScriptThing(Me, CScriptThing::InlineCopy()), pGlobalScriptDef->AmbushTradersSentryWanderDistance);
    Game->SetScriptingStateGroup(CScriptThing(Me, CScriptThing::InlineCopy()), 4);
    Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsEnemyOfThing(Game->GetHero(), Me);
    for (unsigned long i = 0; i < ParentClass->AllBandits.size(); ++i) {
        if (ParentClass->AllBandits[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsEnemyOfThing(Me, ParentClass->AllBandits[i]);
            Game->EntitySetThingAsEnemyOfThing(ParentClass->AllBandits[i], Me);
        }
    }
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("WatchForHeroAttackOnSentry"), 0, ParentClass,
            &CQ_AmbushTradersScript::WatchForHeroAttackOnSentry, CScriptThing(Me, CScriptThing::InlineCopy())),
        CCharString(""));
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("WatchForEnemyDeath"), 0, ParentClass,
            &CQ_AmbushTradersScript::WatchForEnemyDeath, CScriptThing(Me, CScriptThing::InlineCopy())),
        CCharString(""));
    SentrySeenSpy = false;
}

static const char* Run(int terminateAfter, bool* seen)
{
    gLog[0] = 0; gTerminateChecks = 0; gTerminateAfter = terminateAfter;
    {
        CGameScriptInterfaceBase game;
        CScriptThing b1(11, true), b2(12, false), b3(13, true);
        NScript::CQ_AmbushTradersScript parent;
        parent.AllBandits.Items[0] = &b1; parent.AllBandits.Items[1] = &b2; parent.AllBandits.Items[2] = &b3; parent.AllBandits.Count = 3;
        NScript::CQ_AmbushTradersScript::CAT_Sentry sentry;
        sentry.Game = &game; sentry.ParentClass = &parent; sentry.PMasterData = 0; sentry.SentrySeenSpy = true;
        sentry.Init();
        *seen = sentry.SentrySeenSpy;
    }
    return gLog;
}

int main()
{
    int fails = 0;
    bool seen = true;
    const char* a = Run(1000, &seen);
    const char* ea = "Centre 9 9,2,3;Min 9 0;Max 9 25;Group 9 4;Enemy 9>0;Enemy 0>9;Enemy 9>11;Enemy 11>9;Enemy 9>13;Enemy 13>9;"
                     "Spawn ParentClass.WatchForHeroAttackOnSentry id0 '';WatchForHeroAttackOnSentry(9);"
                     "Spawn ParentClass.WatchForEnemyDeath id0 '';WatchForEnemyDeath(9);";
    if (std::strcmp(a, ea) || seen || gThingsAlive != 0) { std::printf("A: %s %d\n", a, gThingsAlive); ++fails; }
    const char* b = Run(1, &seen);
    const char* eb = "Centre 9 9,2,3;Min 9 0;Max 9 25;Group 9 4;Enemy 9>0;Enemy 0>9;Enemy 9>11;Enemy 11>9;";
    if (std::strcmp(b, eb) || !seen || gThingsAlive != 0) { std::printf("B: %s %d\n", b, gThingsAlive); ++fails; }
    if (fails == 0) std::printf("00f08e90_TEST PASS\n");
    return fails;
}