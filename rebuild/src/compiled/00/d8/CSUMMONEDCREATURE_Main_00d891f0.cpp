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
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
struct CQ_SunnyvaleMasterData {
    unsigned char _pad00[0x68];
    bool          CollectedSoulFromNostro;         // +0x68
};
struct CQ_HeroSoulsNostroScript;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CSUMMONED_CREATURE : public CEntityScriptBase {
public:
        CQ_HeroSoulsNostroScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
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