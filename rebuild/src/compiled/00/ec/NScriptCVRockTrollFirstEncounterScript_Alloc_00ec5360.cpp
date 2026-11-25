// NScript::CV_RockTrollFirstEncounterScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_RockTrollFirstEncounterScript(data, game)
// Leaf: ?Alloc@CV_RockTrollFirstEncounterScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC5360, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f15c4 (outer CV_RockTrollFirstEncounterScript proven by its Main 00ec3d40 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd9426 with name "V_RockTrollFirstEncounter"; PDB ?Alloc@CV_RockTrollFirstEncounterScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f15c4; PDB ?Alloc@CV_RockTrollFirstEncounterScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f15c4@@YIPAUCVtable_012f15c4@@PAX0@Z` (00/ec/global_AllocCVtable012f15c4_00ec5360.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CV_RockTrollFirstEncounterScript.h (+ add-only opt-in FABLE_RETAIL_CV_ROCKTROLLFIRSTENCOUNTERSCRIPT_ALLOC: Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012F15C4 now carries the FableWin.pdb overrides (RegisterMain Main Init OnPersist) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CV_ROCKTROLLFIRSTENCOUNTERSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CV_RockTrollFirstEncounterScript.h"
inline NScript::CV_RockTrollFirstEncounterScript::CV_RockTrollFirstEncounterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CV_RockTrollFirstEncounterScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_RockTrollFirstEncounterScript(data, game);
}
