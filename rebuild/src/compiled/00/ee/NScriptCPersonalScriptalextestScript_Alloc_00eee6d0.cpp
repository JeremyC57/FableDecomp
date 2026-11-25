// NScript::CPersonalScript_alextestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScript_alextestScript(data, game)
// Leaf: ?Alloc@CPersonalScript_alextestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEE6D0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f5814 (outer CPersonalScript_alextestScript proven by its Main 00eed360 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cda336 with name "PersonalScript_alextest"; PDB ?Alloc@CPersonalScript_alextestScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f5814; PDB ?Alloc@CPersonalScript_alextestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5814@@YIPAUCVtable_012f5814@@PAX0@Z` (00/ee/global_AllocCVtable012f5814_00eee6d0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CPersonalScript_alextestScript.h (+ add-only opt-in FABLE_RETAIL_CPERSONALSCRIPT_ALEXTESTSCRIPT_ALLOC: CPersonalScript_alextestScript, Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012F5814 now carries the FableWin.pdb overrides (RegisterMain Main Init) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CPERSONALSCRIPT_ALEXTESTSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CPersonalScript_alextestScript.h"
inline NScript::CPersonalScript_alextestScript::CPersonalScript_alextestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CalextestData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CPersonalScript_alextestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScript_alextestScript(data, game);
}
