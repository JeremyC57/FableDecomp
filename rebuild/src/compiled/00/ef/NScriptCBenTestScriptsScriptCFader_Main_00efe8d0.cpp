// NScript::CBenTestScriptsScript::CFader::Main() -- register an alive filter over Me, give it hero information, wait a script frame at a time until the hero talks to or uses it, then (unless the script is ending) fade it out through the parent test script
// Leaf: ?Main@CFader@CBenTestScriptsScript@NScript@@EAEXXZ
// agentNF25b (2026-11-12): on the shared engine/CEntityScriptBase.h, CGameScriptInterface.h and CIsThingConscious.h: the filter is
//   NScript::CIsThingConscious (vtable 0x012C2FE8, dtor 0x00EF7AB0; was N_00ef7ab0::CIsThingAlive), its registration
//   CEntityScriptBase::SetPredicate (was a descriptive RegisterFilter on the entity), and Main the private virtual override
//   (the PDB EAE spelling; was a public non-virtual QAE).
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Naming wave 25: ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30 (Ego_r.pdb / FableWin.pdb: global class, const)
//   is declared on the global CEntityScriptBase the entity derives from (the landed SetPredicate rows' empty-base model; the vptr /
//   Game / Me fields stay on the entity: empty-base layout, bytes unchanged); was CBenTestScriptsScript::CFader::IsDone (non-const; the label DB's IsDone).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFE8D0, length 277 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?Main@CFader@CBenTestScriptsScript@NScript@@EAEXXZ; the parent call is
//   ?FadeEntityOut@CBenTestScriptsScript@NScript@@QAEXABVCScriptThing@@_N@Z @ 0x00EFE9F0 (the next function).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member, filter vtable 0x012C2FE8), the filter
//   registration @ 0x00F35B10 and IsDone @ 0x00F35B30 (on `this`; manifest labels propagated, as in the landed
//   CSummonerToKill::Main 0x00D83DC0), this module's out-of-line ~CIsThingAlive @ 0x00EF7AB0; CCharString(const char*, long)
//   @ 0x0099EBF0 ("SCRIPT_NAME_HERO" @ 0x0125D1C8) and ~CCharString @ 0x0099EAE0; game script interface (+4) slots 7
//   NewScriptFrame and 360 SetThingHasInformation(const CScriptThing&, bool, bool, bool) (cgsi_slots_aligned.tsv); Me's
//   CGameScriptThing slots 27 MsgIsTalkedToBy and 20 MsgIsUsedBy (rtti-vtables 0x012852CC).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass (CBenTestScriptsScript*) +0x14.
// Shape: the landed WatchForFailure 0x00E1F2D0 wait loop: `bool waiting = !(a || b);` over two implicit CCharString
//   conversion temporaries keeps the destruction flags in ebx (`or ebx,1/2`, zeroed with the first string's frame word
//   at entry) and the condition in a frame byte; the filter is a block-scoped polymorphic object (vtable stored before the
//   member copy) whose out-of-line destructor runs at the end of its block.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
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

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08;
//   SetPredicate(const NScript::CPredicateBase&) 0x00F35B10, IsActiveThreadTerminating() const 0x00F35B30) (agentNF25b: was an empty
//   local base with the vptr / Game / Me and a descriptive RegisterFilter(CIsThingAlive&) on the entity).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CBenTestScriptsScript { public:
    void FadeEntityOut(const CScriptThing& thing, bool fadeOut);   // 0x00EFE9F0
    class CFader;
};
class CBenTestScriptsScript::CFader : public CEntityScriptBase {
public:
    CBenTestScriptsScript*      ParentClass;            // +0x14
    void*                       PMasterData;            // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
};

void CBenTestScriptsScript::CFader::Main()
{
    {
        CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingHasInformation(Me, false, true, false);
    for (;;) {
        bool waiting = !(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || Me.MsgIsUsedBy("SCRIPT_NAME_HERO"));
        if (!waiting)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->FadeEntityOut(Me, true);
}
}