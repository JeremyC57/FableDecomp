// NScript::CV_DemonDoorsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_DemonDoorsScript(data, game)
// Leaf: ?Alloc@CV_DemonDoorsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E75590, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e8b10 (outer CV_DemonDoorsScript proven by its Main 00e6e380 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd8799 with name "V_DemonDoors"; PDB ?Alloc@CV_DemonDoorsScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e8b10; PDB ?Alloc@CV_DemonDoorsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e8b10@@YIPAUCVtable_012e8b10@@PAX0@Z` (00/e7/global_AllocCVtable012e8b10_00e75590.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CV_DemonDoorsScript.h (+ add-only opt-in FABLE_RETAIL_CV_DEMONDOORSSCRIPT_ALLOC: CV_DemonDoorsScript, Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012E8B10 now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CV_DEMONDOORSSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CV_DemonDoorsScript.h"
inline NScript::CV_DemonDoorsScript::CV_DemonDoorsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CV_DemonDoorsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_DemonDoorsScript(data, game);
}
