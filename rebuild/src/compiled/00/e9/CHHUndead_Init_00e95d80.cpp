// NScript::CV_HauntedHouseScript::CHHUndead::Init() -- give the undead the hero as its best enemy target, then remember its health as the raise-undead bar amount (+0x1c)
// Leaf: ?Init@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E95D80, length 64 bytes.
// Callees: the script interface's slots 70 GetHero (CScriptThing&), 451 GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) and 264 GetHealth(const CScriptThing&) (float).
// Ego_r.pdb CHHUndead (32 bytes): PQuest +4, the CScriptThing +8, ParentClass +0x14, PMasterData +0x18, RaiseUndeadBarAmount +0x1c. Shape: the interface pointer re-read for each
// call but its vtable kept in edi across the one expression (`GiveThingBestEnemyTarget(Thing, GetHero())`), the thing's address in ebx, `fstp [esi+1Ch]`. Twin bodies at 0x00D292E0 / 0x00E95D80.
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
// Self respelled by the move: ?Init@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ (Ego_r.pdb + FableWin.pdb; slot 2 of the retail
//   vtable 0x012ECD3C holds 0x00E95D80); the row-local model spelled it ?Init@CHHUndead@@QAEXXZ. The interface is CEntityScriptBase's
//   Game reference +0x04 (was PQuest), the thing its Me +0x08 (was Thing).
void NScript::CV_HauntedHouseScript::CHHUndead::Init()
{
    Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
    RaiseUndeadBarAmount = Game.GetHealth(Me);
}
