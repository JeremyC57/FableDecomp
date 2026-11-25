// NScript::CV_BanditTollScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_BanditTollScript(data, game)
// Leaf: ?Alloc@CV_BanditTollScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4F550, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e4b7c (outer CV_BanditTollScript proven by its Main 00e479b0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd819f with name "V_BanditToll"; PDB ?Alloc@CV_BanditTollScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e4b7c; PDB ?Alloc@CV_BanditTollScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e4b7c@@YIPAUCVtable_012e4b7c@@PAX0@Z` (00/e4/global_AllocCVtable012e4b7c_00e4f550.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x5C = sizeof, Ego_r.pdb 112 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CV_BanditTollScript.h (+ add-only opt-in FABLE_RETAIL_CV_BANDITTOLLSCRIPT_ALLOC: CV_BanditTollScript, Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012E4B7C now carries the FableWin.pdb overrides (RegisterMain Main Init OnPersist) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CV_BANDITTOLLSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CV_BanditTollScript.h"
inline NScript::CV_BanditTollScript::CV_BanditTollScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CV_BanditTollScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_BanditTollScript(data, game);
}
