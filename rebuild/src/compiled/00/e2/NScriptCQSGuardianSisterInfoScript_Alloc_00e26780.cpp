// NScript::CQS_GuardianSisterInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQS_GuardianSisterInfoScript(data, game)
// Leaf: ?Alloc@CQS_GuardianSisterInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E26780, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e1dac (outer CQS_GuardianSisterInfoScript proven by its Main 00e25ab0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd7cad with name "QS_GuardianSisterInfo"; PDB ?Alloc@CQS_GuardianSisterInfoScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e1dac; PDB ?Alloc@CQS_GuardianSisterInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e1dac@@YIPAUCVtable_012e1dac@@PAX0@Z` (00/e2/global_AllocCVtable012e1dac_00e26780.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQS_GuardianSisterInfoScript.h (+ add-only opt-in FABLE_RETAIL_CQS_GUARDIANSISTERINFOSCRIPT_ALLOC: CQS_GuardianSisterInfoScript), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012E1DAC now carries the FableWin.pdb overrides (RegisterMain Main Init OnPersist) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQS_GUARDIANSISTERINFOSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CQS_GuardianSisterInfoScript.h"
inline NScript::CQS_GuardianSisterInfoScript::CQS_GuardianSisterInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CQS_GuardianSisterInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQS_GuardianSisterInfoScript(data, game);
}
