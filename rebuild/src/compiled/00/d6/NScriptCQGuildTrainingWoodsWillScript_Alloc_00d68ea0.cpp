// NScript::CQ_GuildTrainingWoodsWillScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_GuildTrainingWoodsWillScript(data, game)
// Leaf: ?Alloc@CQ_GuildTrainingWoodsWillScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D68EA0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012d1e08 (outer CQ_GuildTrainingWoodsWillScript proven by its Main 00d67890 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd655e with name "Q_GuildTrainingWoodsWill"; PDB ?Alloc@CQ_GuildTrainingWoodsWillScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012d1e08; PDB ?Alloc@CQ_GuildTrainingWoodsWillScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012d1e08@@YIPAUCVtable_012d1e08@@PAX0@Z` (00/d6/global_AllocCVtable012d1e08_00d68ea0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x50 = sizeof, Ego_r.pdb 100 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQ_GuildTrainingWoodsWillScript.h (NEW header: Ego_r.pdb layout, FableWin.pdb methods), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is inline in the header. The vftable 0x012D1E08 now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "rebuild_abi.h"
#include "engine/CQ_GuildTrainingWoodsWillScript.h"
CScriptBase* FABLE_FASTCALL NScript::CQ_GuildTrainingWoodsWillScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingWoodsWillScript(data, game);
}
