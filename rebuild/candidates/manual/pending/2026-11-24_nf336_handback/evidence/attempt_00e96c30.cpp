// NScript::CV_HauntedHouseScript::SendBodyguardsToTheRightPlace(CCharString marker1, CCharString marker2) -- collect the things named "BodyGuard"; with one bodyguard teleport it to marker1, with two teleport the first to marker1 and the second to marker2
// Leaf: ?SendBodyguardsToTheRightPlace@CV_HauntedHouseScript@NScript@@QAEXVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E96C30, 413 bytes, thiscall, `ret 8` (two by-value CCharString arguments, destroyed by
//   the callee at the end).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?SendBodyguardsToTheRightPlace@CV_HauntedHouseScript@NScript@@QAEXVCCharString@@0@Z
//   is the only CV_HauntedHouseScript member taking two CCharStrings by value; the row's only caller is
//   CV_HauntedHouseScript::CM_HeroInsideHouse::Main 0x00E96660 (site 0x00E96888, `this` = ParentClass +0x14, two CCharString
//   argument copies built in the argument area); the body fetches "BodyGuard" things. Catalog label DoMission_BodyGuardSimple
//   (descriptive, wrong). FableWin.pdb also has a CQ_BreakSiegeScript twin of the method (another body).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, ~CScriptThing 0x004AA840, std::vector<CScriptThing>::~vector 0x008AC970 (out of line on the terminating
//   exit only), free 0x00BFEA14; CGameScriptInterface slots 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 472
//   EntityTeleportToThing (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "BodyGuard" 0x012CB3F0.
// Layout: CScriptBase retail 0x40, Game +0x40; STLport 4.6 vector<CScriptThing> (12 bytes, 12-byte elements: the
//   `imul 0x2AAAAAAB ; sar 1` divide by 12).
// Shape: the vector local is the STLport 4.6 template (allocator-proxy base, inline ~vector = _Destroy loop through the
//   element's virtual destructor + null-tested free); VC7.1 inlines the destructor on the fall-through exit and calls the
//   COMDAT copy 0x008AC970 on the shared IsActiveThreadTerminating exit.
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is the same in every CV_HauntedHouseScript object-mate row, so the mates compose in one TU.
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
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
void NScript::CV_HauntedHouseScript::SendBodyguardsToTheRightPlace(CCharString marker1, CCharString marker2)
{
    std::vector<CScriptThing> bodyguards;
    Game.GetAllThingsWithScriptName("BodyGuard", bodyguards);
    if (bodyguards.size() == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game.EntityTeleportToThing(bodyguards[0], Game.GetThingWithScriptName(marker1), false);
    }
    if (bodyguards.size() == 2) {
        if (IsActiveThreadTerminating())
            return;
        Game.EntityTeleportToThing(bodyguards[0], Game.GetThingWithScriptName(marker1), false);
        Game.EntityTeleportToThing(bodyguards[1], Game.GetThingWithScriptName(marker2), false);
    }
}
