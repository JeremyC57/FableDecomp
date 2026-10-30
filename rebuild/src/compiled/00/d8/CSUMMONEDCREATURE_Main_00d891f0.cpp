// CSUMMONED_CREATURE::Main() -- Hero Souls (Nostro) summoned creature: watch itself with an alive predicate, set the hero as its best enemy target, wait until the master data records Nostro's soul as collected, then remove itself
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D891F0, length 175 bytes (`ret`).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the predicate's member; CIsThingAlive vtable 0x012C32F0
// kept in ebx and stored again by its inline destructor), CEntityScriptBase::SetPredicate @ 0x00F35B10,
// ~CScriptThing @ 0x004AA840, CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30; CGameScriptInterface slots
// 7 NewScriptFrame, 70 GetHero, 451 GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&), 108 RemoveThing.
// Layout = Ego_r.pdb CSUMMONED_CREATURE of the CQ_HeroSoulsNostroScript family (28 bytes): CEntityScriptBase {vptr,
// Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18 (CQ_SunnyvaleMasterData, CollectedSoulFromNostro +0x68).
// Shape: the landed CSummonerToKill::Main 0x00D88540 model: the predicate is a scoped temporary object, one frame and a
// done check, the GiveThingBestEnemyTarget call with GetHero evaluated first (the interface vtable read twice), then
// the top-tested wait loop on the master-data flag (16-aligned head) and the final removal.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    virtual ~CScriptThing();                               // 0x004AA840
};
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
class CIsThingAlive : public NScript::CPredicateBase { public:                             // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing Thing;                            // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CQ_SunnyvaleMasterData {
    unsigned char _pad00[0x68];
    bool          CollectedSoulFromNostro;         // +0x68
};
struct CQ_HeroSoulsNostroScript;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CSUMMONED_CREATURE : public CEntityScriptBase {
public:
    void*                     __vftable;           // +0x00
    CGameScriptInterface*     Game;                // +0x04
    CScriptThing              Me;                  // +0x08
    CQ_HeroSoulsNostroScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
    void Main();
};

void CSUMMONED_CREATURE::Main()
{
    {
        CIsThingAlive predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!PMasterData->CollectedSoulFromNostro) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}