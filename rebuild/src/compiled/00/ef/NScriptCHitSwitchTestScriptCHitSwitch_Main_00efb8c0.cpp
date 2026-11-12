// NScript::CHitSwitchTestScript::CHitSwitch::Main() -- register an alive filter over Me, then until the script ends: wait a script frame at a time until the hero hits the switch, then (unless the script is ending) set its activation trigger, pause ten seconds, clear it again and wait a frame
// Leaf: ?Main@CHitSwitch@CHitSwitchTestScript@NScript@@EAEXXZ
// agentNF25b (2026-11-12): on the shared engine/CEntityScriptBase.h, CGameScriptInterface.h and CIsThingConscious.h: the filter is
//   NScript::CIsThingConscious (vtable 0x012C2FE8, dtor 0x00EF7AB0; was N_00ef7ab0::CIsThingAlive), its registration
//   CEntityScriptBase::SetPredicate (was a descriptive RegisterFilter on the entity), and Main the private virtual override
//   (the PDB EAE spelling; was a public non-virtual QAE).
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Naming wave 25: ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30 (Ego_r.pdb / FableWin.pdb: global class, const)
//   is declared on the global CEntityScriptBase the entity derives from (the landed SetPredicate rows' empty-base model; the vptr /
//   Game / Me fields stay on the entity: empty-base layout, bytes unchanged); was CHitSwitchTestScript::CHitSwitch::IsDone (non-const; the label DB's IsDone).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFB8C0, length 243 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?Main@CHitSwitch@CHitSwitchTestScript@NScript@@EAEXXZ (catalog row "CHitSwitch::Main").
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member, filter vtable 0x012C2FE8), the filter
//   registration @ 0x00F35B10 and IsDone @ 0x00F35B30 (on `this`, as in the landed CFader::Main 0x00EFE8D0), the
//   out-of-line ~CIsThingAlive @ 0x00EF7AB0, the __fastcall helper FoundMessageIsThingHitBy(CScriptThing*, CScriptThing*)
//   @ 0x00CBE0C7 (engine_api.tsv; body: thing->MsgIsHitBy(hitter->GetName())); game script interface (+4) slots 7
//   NewScriptFrame, 70 GetHero, 376 Pause(float) (cgsi_slots_aligned.tsv); Me's CGameScriptThing slot 69
//   SetActivationTriggerStatus(bool) (vtable 0x012852CC).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}.
// Shape: block-scoped filter (CFader form); `if (IsActiveThreadTerminating()) return;` then a `while (!IsActiveThreadTerminating())` outer loop whose entry test
//   is the second IsDone call; the inner hit wait is a plain rotated while.
class CCharString { public:
    void* PStringData;
};

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots)
//   (agentNF25b: replaces the local CGameScriptInterface_Calls slot model).
#include "engine/CGameScriptInterface.h"

namespace NScript { class CPredicateBase; }
// NScript::CIsThingConscious: the retail model (vtable 0x012C2FE8, Thing +0x04, 0x10); its dtor 0x00EF7AB0 is called out of line
//   (agentNF25b: was the address-namespaced local N_00ef7ab0::CIsThingAlive; Ego_r + FableWin ??1CIsThingConscious@NScript@@UAE@XZ).
#include "engine/CIsThingConscious.h"
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
using NScript::CIsThingConscious;

bool __fastcall FoundMessageIsThingHitBy(CScriptThing* thing, CScriptThing* hitter);   // 0x00CBE0C7

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08;
//   SetPredicate(const NScript::CPredicateBase&) 0x00F35B10, IsActiveThreadTerminating() const 0x00F35B30) (agentNF25b: was an empty
//   local base with the vptr / Game / Me and a descriptive RegisterFilter(CIsThingAlive&) on the entity).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CHitSwitchTestScript { public:
    class CHitSwitch;
};
class CHitSwitchTestScript::CHitSwitch : public CEntityScriptBase {
public:
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
};

void CHitSwitchTestScript::CHitSwitch::Main()
{
    {
        CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!FoundMessageIsThingHitBy(&Me, &Game->GetHero())) {
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
}