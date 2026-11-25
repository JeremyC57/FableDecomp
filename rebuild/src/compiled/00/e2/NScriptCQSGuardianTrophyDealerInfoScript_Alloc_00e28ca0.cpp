// NScript::CQS_GuardianTrophyDealerInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQS_GuardianTrophyDealerInfoScript(data, game)
// Leaf: ?Alloc@CQS_GuardianTrophyDealerInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E28CA0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e2018 (outer CQS_GuardianTrophyDealerInfoScript proven by its Main 00e27920 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd7ddf with name "QS_GuardianTrophyDealerInfo"; PDB ?Alloc@CQS_GuardianTrophyDealerInfoScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e2018; PDB ?Alloc@CQS_GuardianTrophyDealerInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e2018@@YIPAUCVtable_012e2018@@PAX0@Z` (00/e2/global_AllocCVtable012e2018_00e28ca0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQS_GuardianTrophyDealerInfoScript.h (+ add-only opt-in FABLE_RETAIL_CQS_GUARDIANTROPHYDEALERINFOSCRIPT_ALLOC: Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012E2018 now carries the FableWin.pdb overrides (RegisterMain Main Init OnPersist) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQS_GUARDIANTROPHYDEALERINFOSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CQS_GuardianTrophyDealerInfoScript.h"
inline NScript::CQS_GuardianTrophyDealerInfoScript::CQS_GuardianTrophyDealerInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CQS_GuardianTrophyDealerInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQS_GuardianTrophyDealerInfoScript(data, game);
}
