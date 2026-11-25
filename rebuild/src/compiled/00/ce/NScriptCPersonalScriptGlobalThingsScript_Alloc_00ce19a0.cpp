// NScript::CPersonalScript_GlobalThingsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScript_GlobalThingsScript(data, game)
// Leaf: ?Alloc@CPersonalScript_GlobalThingsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE19A0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012c30cc (outer CPersonalScript_GlobalThingsScript proven by its Main 00cde420 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd5511 with name "PersonalScript_GlobalThings"; PDB ?Alloc@CPersonalScript_GlobalThingsScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012c30cc; PDB ?Alloc@CPersonalScript_GlobalThingsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c30cc@@YIPAUCVtable_012c30cc@@PAX0@Z` (00/ce/global_AllocCVtable012c30cc_00ce19a0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CPersonalScript_GlobalThingsScript.h (+ add-only opt-in FABLE_RETAIL_CPERSONALSCRIPT_GLOBALTHINGSSCRIPT_ALLOC: CPersonalScript_GlobalThingsScript, Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012C30CC now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CPERSONALSCRIPT_GLOBALTHINGSSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CPersonalScript_GlobalThingsScript.h"
inline NScript::CPersonalScript_GlobalThingsScript::CPersonalScript_GlobalThingsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CPersonalScript_GlobalThingsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScript_GlobalThingsScript(data, game);
}
