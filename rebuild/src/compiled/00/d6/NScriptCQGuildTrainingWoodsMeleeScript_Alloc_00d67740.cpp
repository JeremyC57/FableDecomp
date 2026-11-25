// NScript::CQ_GuildTrainingWoodsMeleeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_GuildTrainingWoodsMeleeScript(data, game)
// Leaf: ?Alloc@CQ_GuildTrainingWoodsMeleeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D67740, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012d1d1c (outer CQ_GuildTrainingWoodsMeleeScript proven by its Main 00d66620 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd64c5 with name "Q_GuildTrainingWoodsMelee"; PDB ?Alloc@CQ_GuildTrainingWoodsMeleeScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012d1d1c; PDB ?Alloc@CQ_GuildTrainingWoodsMeleeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012d1d1c@@YIPAUCVtable_012d1d1c@@PAX0@Z` (00/d6/global_AllocCVtable012d1d1c_00d67740.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQ_GuildTrainingWoodsMeleeScript.h (NEW header: Ego_r.pdb layout, FableWin.pdb methods), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is inline in the header. The vftable 0x012D1D1C now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "rebuild_abi.h"
#include "engine/CQ_GuildTrainingWoodsMeleeScript.h"
CScriptBase* FABLE_FASTCALL NScript::CQ_GuildTrainingWoodsMeleeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingWoodsMeleeScript(data, game);
}
