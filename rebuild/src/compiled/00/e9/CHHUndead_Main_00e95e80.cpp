// NScript::CV_HauntedHouseScript::CHHUndead::Main() -- haunted-house undead script: register an alive-filter over Me, start a script frame; unless done, when the parent script's flag (+0x50) is set remove Me (unless done by then), then run script frames until done
// Leaf: ?Main@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ
// Identity (naming wave 11): ?Main@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012ECD3C slot 1 is stored by the factory 0x00E95CD0, registered as "HHUndead" (site 0x00E94AD5)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E95E80, length 137 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, the filter registration @ 0x00F35B10,
// the filter's out-of-line destructor @ 0x00E241B0, IsDone @ 0x00F35B30; CGameScriptInterface slots 7 NewScriptFrame and
// 108 RemoveThing(const CScriptThing&, bool, bool) (called with true, true).
// Layout = Ego_r.pdb CHHUndead (32 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14
// (CV_HauntedHouseScript*, the tested byte at +0x50 is unnamed in the PDB), PMasterData +0x18, RaiseUndeadBarAmount +0x1c.
// Shape: as the landed CHTE_Prisoner::Main 0x00D69F50; the early exits are plain returns (the second IsDone test
// jumps straight to the epilogue), the wait loop a `do/while`.
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
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
// The flag at +0x50 is the PDB member CellarDoorOpen (the row-local model called it m_raiseUndead).
void NScript::CV_HauntedHouseScript::CHHUndead::Main()
{
    {
        NScript::CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass.CellarDoorOpen) {
        if (IsActiveThreadTerminating())
            return;
        Game.RemoveThing(Me, true, true);
    }
    do {
        Game.NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
