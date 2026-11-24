// NScript::CV_HauntedHouseScript::OnPersist(CPersistContext& context) -- saves/loads CellarDoorOpen and HeroBeenToChest by name (default false)
// Leaf: ?OnPersist@CV_HauntedHouseScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93E60, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00e96dd0 at 00cd8b2f with name "V_HauntedHouse"; Alloc news + stores script vtable 012ecb94; PDB ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@; script vtable slot 4 (slot 1 pushes "Main" 0x012C2F94); PDB OnPersist; refs=1; was _unattributed::PersistTransfer2_00e93e60.
//   ?OnPersist@CV_HauntedHouseScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e93e60::PersistTransfer2_00e93e60(CPersistContext* context) -- persists the "CellarDoorOpen" and "HeroBeenToChest" byte flags at +0x50/+0x51 Retail: Fable.exe (TLC, VC7.1) function @ 0x00E93E60, oracle ... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00E93D50 (rebuild/src/compiled/00/e9/NScriptCVHauntedHouseScript_CVHauntedHouseScript_00e93d50.cpp); the PDB OnPersist declaration added; its bool CellarDoorOpen, bool HeroBeenToChest; members CellarDoorOpen +0x50, HeroBeenToChest +0x51 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
#include "engine/CPersistContext.h"
// The HEAD row also defined the constructor (the landed 0x00E93D50 body) in this TU; kept, so the object's COMDAT set is unchanged.
NScript::CV_HauntedHouseScript::CV_HauntedHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}

void NScript::CV_HauntedHouseScript::OnPersist(CPersistContext& context)
{
    context.Transfer("CellarDoorOpen", CellarDoorOpen, false);
    context.Transfer("HeroBeenToChest", HeroBeenToChest, false);
}
