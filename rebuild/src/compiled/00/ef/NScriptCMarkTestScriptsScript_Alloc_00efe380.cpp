// NScript::CMarkTestScriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CMarkTestScriptsScript(data, game)
// Leaf: ?Alloc@CMarkTestScriptsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFE380, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f6d00 (outer CMarkTestScriptsScript proven by its Main 00efca00 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cdaf6c with name "MarkTestScripts"; PDB ?Alloc@CMarkTestScriptsScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f6d00; PDB ?Alloc@CMarkTestScriptsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f6d00@@YIPAUCVtable_012f6d00@@PAX0@Z` (00/ef/global_AllocCVtable012f6d00_00efe380.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CMarkTestScriptsScript.h (+ add-only opt-in FABLE_RETAIL_CMARKTESTSCRIPTSSCRIPT_ALLOC: Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012F6D00 now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CMARKTESTSCRIPTSSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CMarkTestScriptsScript.h"
inline NScript::CMarkTestScriptsScript::CMarkTestScriptsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CMarkTestScriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CMarkTestScriptsScript(data, game);
}
