// NScript::CV_OracleScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_OracleScript(data, game)
// Leaf: ?Alloc@CV_OracleScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBEE30, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f0c84 (outer CV_OracleScript proven by its Main 00ebcd70 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd91c2 with name "V_Oracle"; PDB ?Alloc@CV_OracleScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f0c84; PDB ?Alloc@CV_OracleScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f0c84@@YIPAUCVtable_012f0c84@@PAX0@Z` (00/eb/global_AllocCVtable012f0c84_00ebee30.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CV_OracleScript.h, replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012F0C84 now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "rebuild_abi.h"
#include "engine/CV_OracleScript.h"
inline NScript::CV_OracleScript::CV_OracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CV_OracleScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_OracleScript(data, game);
}
