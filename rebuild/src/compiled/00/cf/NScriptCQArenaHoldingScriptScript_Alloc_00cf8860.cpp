// NScript::CQ_ArenaHoldingScriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_ArenaHoldingScriptScript(data, game)
// Leaf: ?Alloc@CQ_ArenaHoldingScriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF8860, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012c60a0 (outer CQ_ArenaHoldingScriptScript proven by its Main 00cf0790 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd56dc with name "Q_ArenaHoldingScript"; PDB ?Alloc@CQ_ArenaHoldingScriptScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012c60a0; PDB ?Alloc@CQ_ArenaHoldingScriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c60a0@@YIPAUCVtable_012c60a0@@PAX0@Z` (00/cf/global_AllocCVtable012c60a0_00cf8860.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x50 = sizeof, Ego_r.pdb 100 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQ_ArenaHoldingScriptScript.h (+ add-only opt-in FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ALLOC: CQ_ArenaHoldingScriptScript, Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012C60A0 now carries the FableWin.pdb overrides (RegisterMain Main Init) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CQ_ArenaHoldingScriptScript.h"
inline NScript::CQ_ArenaHoldingScriptScript::CQ_ArenaHoldingScriptScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CQ_ArenaHoldingScriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_ArenaHoldingScriptScript(data, game);
}
