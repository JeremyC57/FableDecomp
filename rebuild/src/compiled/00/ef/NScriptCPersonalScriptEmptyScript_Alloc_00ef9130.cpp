// NScript::CPersonalScript_EmptyScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScript_EmptyScript(data, game)
// Leaf: ?Alloc@CPersonalScript_EmptyScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF9130, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f5d7c (outer CPersonalScript_EmptyScript proven by its Main 00ef0090 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cda9e7 with name "PersonalScript_Empty"; PDB ?Alloc@CPersonalScript_EmptyScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f5d7c; PDB ?Alloc@CPersonalScript_EmptyScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5d7c@@YIPAUCVtable_012f5d7c@@PAX0@Z` (00/ef/global_AllocCVtable012f5d7c_00ef9130.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CPersonalScript_EmptyScript.h, replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012F5D7C now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CPersonalScript_EmptyScript.h"
inline NScript::CPersonalScript_EmptyScript::CPersonalScript_EmptyScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CPersonalScript_EmptyScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScript_EmptyScript(data, game);
}
