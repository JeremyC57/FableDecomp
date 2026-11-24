// NScript::CV_HauntedHouseScript::Init() -- reset the haunted house quest: bar, counters and flags, four undead at most
// Leaf: ?Init@CV_HauntedHouseScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93D80, 31 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012ECB94, which the landed NScript::CV_HauntedHouseScript::CV_HauntedHouseScript 0x00E93D50 (and the landed Alloc 0x00E96DD0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_HauntedHouseScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_HauntedHouseScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Shape: the float zero is an integer store of eax.
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
void NScript::CV_HauntedHouseScript::Init()
{
    UndeadBar = 0.0f;
    GuiCounter = 0;
    CellarDoorOpen = false;
    HeroBeenToChest = false;
    QuestCardGiven = false;
    InHouse = false;
    UndeadAlive = 0;
    MaxUndead = 4;
}
