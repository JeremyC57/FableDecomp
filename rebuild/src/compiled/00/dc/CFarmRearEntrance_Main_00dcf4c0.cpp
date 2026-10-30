// NScript::CQ_OrchardFarmRaidScript::CFarmRearEntrance::Main() -- Opening Graveyard entity script: register an alive-filter over Me, let the interface update once, and if the script is not done and the parent quest has started the undead cut-scene, remove Me (fading) through the interface
// Leaf: Main
// Identity (naming wave 11): ?Main@CFarmRearEntrance@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012DB4B8 slot 1 is stored by the factory 0x00DCF3D0 registered as "FarmRearEntrance"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCF4C0, length 119 bytes (padded to 128).
// Label correction: the catalog's "CQ_OrchardFarmRaidScript::FadeObjectUp" is not a quest-script member:
// this body is slot 1 (Main, entity-script contract [dtor, Main, Init, ...]) of vtable 0x012DB4B8, stored by the factory
// 0x00DCF3D0 (`new` of 0x1c bytes: Game +4, Me +8 copied from the argument, ParentClass +0x14, PMasterData +0x18).
// No RTTI locator and no registered name in the factory, so the class is named by its vtable (the landed convention).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member at +4; filter vtable
// 0x012C32F0), the filter registration @ 0x00F35B10 (on `this`), CScriptThing::~CScriptThing @ 0x004AA840, IsDone
// @ 0x00F35B30 (x2), the game interface's virtual slots 7 (+0x1c) and 108 (+0x1b0, RemoveThing(thing, true, true)).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14 (the parent's byte at +0x50 is
// CQ_OrchardFarmRaidScript::UndeadCutsceneStarted in the retail layout), PMasterData +0x18.
// Shape (landed CHTE_Prisoner::Main 0x00D69F50): the filter is a scoped 0x10-byte frame object, the address of Me stays
// in edi; the three-term condition is one `&&` chain re-testing IsDone after the parent flag.
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
namespace NScript {
class CQ_OrchardFarmRaidScript {
public:
    unsigned char _pad[0x50];
    bool          UndeadCutsceneStarted;           // +0x50
public:
    class CFarmRearEntrance;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_OrchardFarmRaidScript;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_OrchardFarmRaidScript::CFarmRearEntrance : public CEntityScriptBase {
public:
        CQ_OrchardFarmRaidScript* ParentClass;   // +0x14
    void*                 PMasterData;             // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}
void NScript::CQ_OrchardFarmRaidScript::CFarmRearEntrance::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (!IsActiveThreadTerminating() && ParentClass->UndeadCutsceneStarted && !IsActiveThreadTerminating())
        Game->RemoveThing(Me, true, true);
}