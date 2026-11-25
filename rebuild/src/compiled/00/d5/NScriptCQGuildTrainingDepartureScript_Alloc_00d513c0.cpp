// NScript::CQ_GuildTrainingDepartureScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_GuildTrainingDepartureScript(data, game)
// Leaf: ?Alloc@CQ_GuildTrainingDepartureScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D513C0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012cf670 (outer CQ_GuildTrainingDepartureScript proven by its Main 00d50800 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd612f with name "Q_GuildTrainingDeparture"; PDB ?Alloc@CQ_GuildTrainingDepartureScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012cf670; PDB ?Alloc@CQ_GuildTrainingDepartureScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012cf670@@YIPAUCVtable_012cf670@@PAX0@Z` (00/d5/global_AllocCVtable012cf670_00d513c0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQ_GuildTrainingDepartureScript.h (NEW header: Ego_r.pdb layout, FableWin.pdb methods), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is inline in the header. The vftable 0x012CF670 now carries the FableWin.pdb overrides (RegisterMain Main Init) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "rebuild_abi.h"
#include "engine/CQ_GuildTrainingDepartureScript.h"
CScriptBase* FABLE_FASTCALL NScript::CQ_GuildTrainingDepartureScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingDepartureScript(data, game);
}
