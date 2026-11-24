// NScript::CV_HauntedHouseScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_HauntedHouseScript(data, game)
// Leaf: ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E96DD0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00e96dd0 at 00cd8b2f with name "V_HauntedHouse"; Alloc news + stores script vtable 012ecb94; PDB ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012ecb94; PDB ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012ecb94@@YIPAUCVtable_012ecb94@@PAX0@Z` (00/e9/global_AllocCVtable012ecb94_00e96dd0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x5C = sizeof, Ego_r.pdb 112 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
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
// The Alloc expands the constructor (retail stores the vtable 0x012ECB94 in place): its body is visible here, inline.
inline NScript::CV_HauntedHouseScript::CV_HauntedHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CV_HauntedHouseScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_HauntedHouseScript(data, game);
}
