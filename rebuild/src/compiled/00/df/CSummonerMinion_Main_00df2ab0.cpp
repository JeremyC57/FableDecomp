// CSummonerMinion::Main() -- Summoning-the-ship minion script: hide in limbo until the summoner attacks reach this minion's wave, then (wave 1 targets the hero) appear, fade in, wait for every summoner to die and remove itself
// Leaf: Main
// Retail 0x00DF2AB0, 368 bytes, `ret`.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, the filter registration 0x00F35B10,
// CScriptThing::~CScriptThing 0x004AA840, IsDone 0x00F35B30, FadeInThing 0x00CBE7AA (__fastcall CScriptThing*, float 2.0);
// CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 108 RemoveThing, 376 Pause(1.0f), 451 GiveThingBestEnemyTarget,
// 453 EntitySetInLimbo, 510 EntitySetAlpha (names from rtti-vtables.tsv + manifest).
// Layout = Ego_r.pdb CSummonerMinion (32 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18, WaveID +0x1C; CQ_SummoningTheShipScript retail = PDB - 0x14 (SummonersAlive +0x48,
// CurrentAttackWave +0x4C, SummonerAttacksStarted +0x51).
// Shape: as the landed CHeroSoulsEnemy::Main 0x00D7F110: every wait is `while (!cond) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`
// followed by an `if (IsActiveThreadTerminating()) return;`; the target call is the nested GiveThingBestEnemyTarget(Me, Game->GetHero()).
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
void __fastcall FadeInThing(const CScriptThing& thing, float time);   // 0x00CBE7AA
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CQ_SummoningTheShipScript {
    unsigned char _pad00[0x48];
    long          SummonersAlive;                       // +0x48
    long          CurrentAttackWave;                    // +0x4c
    bool          LighthouseStarted;                    // +0x50
    bool          SummonerAttacksStarted;               // +0x51
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CSummonerMinion : public CEntityScriptBase {
public:
    void*                      __vftable;               // +0x00
    CGameScriptInterface*      Game;                    // +0x04
    CScriptThing               Me;                      // +0x08
    CQ_SummoningTheShipScript* ParentClass;             // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;             // +0x18
    long                       WaveID;                  // +0x1c
    void Main();
};

void CSummonerMinion::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAlpha(Me, 0.0f, true);
    Game->EntitySetInLimbo(Me, true, true);
    while (!(ParentClass->SummonerAttacksStarted && ParentClass->CurrentAttackWave == WaveID)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (WaveID == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    }
    Game->EntitySetInLimbo(Me, false, true);
    FadeInThing(Me, 2.0f);
    while (ParentClass->SummonersAlive) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    Game->RemoveThing(Me, true, true);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}