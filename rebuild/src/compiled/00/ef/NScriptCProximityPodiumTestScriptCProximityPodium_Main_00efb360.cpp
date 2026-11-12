// NScript::CProximityPodiumTestScript::CProximityPodium::Main() -- entity script: register an alive predicate over the podium, then each time the hero comes within 2 units switch the podium's activation trigger on for ten seconds and off again, until the script is done
// Leaf: ?Main@CProximityPodium@CProximityPodiumTestScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFB360, length 256 bytes, plain `ret` (entity-script vtable slot at
// 0x012F6B24).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's member), CEntityScriptBase::SetPredicate
// 0x00F35B10 and IsActiveThreadTerminating 0x00F35B30 (both on `this`; manifest labels propagated), NScript::CIsThingConscious::~CIsThingConscious
// 0x00EF7AB0 (out of line), IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float)
// (`__fastcall`, 0x00CBE2FF), CScriptThing::SetActivationTriggerStatus (virtual slot 69, +0x114); through the entity's
// game script interface (+4): NewScriptFrame (slot 7, +0x1c), GetHero (slot 70, +0x118), Pause(float) (slot 376, +0x5e0).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}.
// Shape: the landed CHTE_Prisoner::Main model (0x00D69F50): the predicate is a scoped frame object; `if (done) return;`
// then `while (!done) { while (!near) { yield; if (done) return; } if (done) return; ...; yield; }` -- both loops
// rotated, the inner test duplicated.
// Naming wave 35: identity reg 00efb227 "ProximityPodium" in 00efb1e0; Alloc 00efb280; vtable 012f6b20 slot 1; PDB ?Main@CProximityPodium@CProximityPodiumTestScript@NScript@@; refs=1; was under-qualified CProximityPodium::Main (wave3/labels_alloc.tsv); PDB ?Main@CProximityPodium@CProximityPodiumTestScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CProximityPodium@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NScript { class CPredicateBase; }
// NScript::CIsThingConscious: the retail model (vtable 0x012C2FE8, Thing +0x04, 0x10); its dtor 0x00EF7AB0 is called out of line
//   (agentNF25b: was the address-namespaced local N_00ef7ab0::CIsThingAlive; Ego_r + FableWin ??1CIsThingConscious@NScript@@UAE@XZ).
#include "engine/CIsThingConscious.h"
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
using NScript::CIsThingConscious;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08;
//   SetPredicate(const NScript::CPredicateBase&) 0x00F35B10, IsActiveThreadTerminating() const 0x00F35B30) (agentNF25b: was a local model
//   whose derived class declared its own SetPredicate(CIsThingAlive&)).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CProximityPodiumTestScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CProximityPodium;
};
}
using NScript::CProximityPodiumTestScript;
namespace NScript {
class CProximityPodiumTestScript::CProximityPodium : public CEntityScriptBase {
public:
private:
    virtual void Main();                          // PDB ?Main@CProximityPodium@CProximityPodiumTestScript@NScript@@EAEXXZ
};
}

void NScript::CProximityPodiumTestScript::CProximityPodium::Main()
{
    {
        CIsThingConscious predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Me.SetActivationTriggerStatus(true);
        Game->Pause(10.0f);
        Me.SetActivationTriggerStatus(false);
        Game->NewScriptFrame();
    }
}