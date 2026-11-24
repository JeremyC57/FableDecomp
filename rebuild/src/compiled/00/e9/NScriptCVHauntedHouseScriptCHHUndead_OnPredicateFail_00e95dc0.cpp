// NScript::CV_HauntedHouseScript::CHHUndead::OnPredicateFail() -- an undead is gone: when the hero killed it, raise the house's undead bar by four, three, two or one times its amount (bar below 50 / 100 / 200 / above) and count one undead fewer
// Leaf: ?OnPredicateFail@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95DC0, 147 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012ECD3C, which the landed
//   NScript::CV_HauntedHouseScript::CHHUndead::Alloc 0x00E95CD0 stores (site 0x00E95D06); Ego_r.pdb has ?OnPredicateFail@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CHHUndead (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slot 18 MsgIsKilledBy ("" 0x0122D70E temporary); CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0. Float pool: 50.0f 0x0124E7F8, 4.0f 0x0123088C, 100.0f 0x01230A08, 3.0f 0x0122DED4, 200.0f
//   0x0125A168 (the doubling is `fadd st(0),st(0)`).
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
void NScript::CV_HauntedHouseScript::CHHUndead::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        float amount;
        if (ParentClass.UndeadBar < 50.0f)
            amount = RaiseUndeadBarAmount * 4.0f;
        else if (ParentClass.UndeadBar < 100.0f)
            amount = RaiseUndeadBarAmount * 3.0f;
        else if (ParentClass.UndeadBar < 200.0f)
            amount = RaiseUndeadBarAmount * 2.0f;
        else
            amount = RaiseUndeadBarAmount;
        ParentClass.UndeadBar += amount;
        --ParentClass.UndeadAlive;
    }
}
