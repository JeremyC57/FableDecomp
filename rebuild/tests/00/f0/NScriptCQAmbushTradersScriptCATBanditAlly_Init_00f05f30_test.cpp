// Behaviour test for 0x00F05F30 NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Init(): the source's body over a logging
// game interface and parent script -- ally of the hero, allowed to follow, has information, allegiances set, the invalid
// region timer zeroed through the global interface, the five state flags cleared, the sit-outside state group disabled
// and the WatchForHeroAttackOnBandit spawned function added with a copy of Me -- checked as an event log.
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
static int gThingsAlive = 0;
class CScriptThing { public:
    int Id;
    struct InlineCopy { };
    CScriptThing(int id) : Id(id) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other) : Id(other.Id) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other, InlineCopy) : Id(other.Id) { ++gThingsAlive; }
    ~CScriptThing() { --gThingsAlive; }
};
class CGameScriptInterfaceBase { public:
    CScriptThing Hero;
    CGameScriptInterfaceBase() : Hero(0) { }
    CScriptThing& GetHero() { return Hero; }
    void SetTimer(long index, long time) { char t[64]; std::sprintf(t, "SetTimer %ld=%ld", index, time); Log(t); }
    void EntitySetThingAsAllyOfThing(const CScriptThing& thing, const CScriptThing& ally) const { char t[64]; std::sprintf(t, "Ally %d>%d", thing.Id, ally.Id); Log(t); }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& thing, bool allowed) const { char t[64]; std::sprintf(t, "Follow %d %d", thing.Id, (int)allowed); Log(t); }
    void SetThingHasInformation(const CScriptThing& thing, bool a, bool b, bool c) const { char t[64]; std::sprintf(t, "Info %d %d%d%d", thing.Id, (int)a, (int)b, (int)c); Log(t); }
    void EntitySetStategroupEnabled(const CScriptThing& thing, const CCharString& group, bool enabled) const { char t[96]; std::sprintf(t, "Group %d %s %d", thing.Id, group.PStringData, (int)enabled); Log(t); }
};
static CGameScriptInterfaceBase* g_pGameScriptInterface;
class CTimer { public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
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
class CEntityScriptBase { public:
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
    CEntityScriptBase() : Game(0), Me(9) { }
    virtual ~CEntityScriptBase() { }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CScriptBase { public:
    void SetBanditAllegiences(const CScriptThing& thing) { char t[64]; std::sprintf(t, "Allegiences(%d)", thing.Id); Log(t); }
    void WatchForHeroAttackOnBandit(CScriptThing thing) { char t[64]; std::sprintf(t, "WatchForHeroAttackOnBandit(%d)", thing.Id); Log(t); }
    class CAT_BanditAlly;
};
class CQ_AmbushTradersScript::CAT_BanditAlly : public CEntityScriptBase {
public:
    CQ_AmbushTradersScript* ParentClass;
    CQ_SunnyvaleMasterData* PMasterData;
    bool                    BanditHired;
    bool                    BanditFollowing;
    bool                    BanditTeleported;
    bool                    BanditSwitchedAllegience;
    bool                    BanditRemovedFromBowerstonePosh;
    bool                    ResetInvalidRegionTimer;
    long                    BanditHealthBarID;
    CTimer                  InvalidRegionTimer;
    void Init();
};
}

void NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Init()
{
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    Game->SetThingHasInformation(Me, true, false, false);
    ParentClass->SetBanditAllegiences(Me);
    InvalidRegionTimer = 0;
    ResetInvalidRegionTimer = true;
    BanditHired = false;
    BanditFollowing = false;
    BanditTeleported = false;
    BanditSwitchedAllegience = false;
    BanditRemovedFromBowerstonePosh = false;
    Game->EntitySetStategroupEnabled(Me, "SG_MINION_SIT_OUTSIDE", false);
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("WatchForHeroAttackOnBandit"), 0, ParentClass,
            &CQ_AmbushTradersScript::WatchForHeroAttackOnBandit, CScriptThing(Me, CScriptThing::InlineCopy())),
        CCharString(""));
}

int main()
{
    int ok = 1;
    {
        CGameScriptInterfaceBase game;
        g_pGameScriptInterface = &game;
        NScript::CQ_AmbushTradersScript parent;
        NScript::CQ_AmbushTradersScript::CAT_BanditAlly ally;
        ally.Game = &game; ally.ParentClass = &parent; ally.PMasterData = 0;
        ally.BanditHired = ally.BanditFollowing = ally.BanditTeleported = ally.BanditSwitchedAllegience = true;
        ally.BanditRemovedFromBowerstonePosh = true; ally.ResetInvalidRegionTimer = false;
        ally.BanditHealthBarID = 77; ally.InvalidRegionTimer.TimerIndex = 5;
        ally.Init();
        const char* want = "Ally 9>0;Follow 9 1;Info 9 100;Allegiences(9);SetTimer 5=0;Group 9 SG_MINION_SIT_OUTSIDE 0;"
                           "Spawn ParentClass.WatchForHeroAttackOnBandit id0 '';WatchForHeroAttackOnBandit(9);";
        ok = std::strcmp(gLog, want) == 0;
        ok = ok && ally.ResetInvalidRegionTimer && !ally.BanditHired && !ally.BanditFollowing && !ally.BanditTeleported
                && !ally.BanditSwitchedAllegience && !ally.BanditRemovedFromBowerstonePosh && ally.BanditHealthBarID == 77;
        if (!ok) std::printf("%s\n", gLog);
    }
    ok = ok && gThingsAlive == 0;
    if (!ok) { std::printf("00f05f30_TEST FAIL\n"); return 1; }
    std::printf("00f05f30_TEST PASS\n");
    return 0;
}