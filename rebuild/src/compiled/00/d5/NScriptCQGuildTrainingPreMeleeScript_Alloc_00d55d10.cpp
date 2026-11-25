// NScript::CQ_GuildTrainingPreMeleeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_GuildTrainingPreMeleeScript(data, game)
// Leaf: ?Alloc@CQ_GuildTrainingPreMeleeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D55D10, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012cf788 (outer CQ_GuildTrainingPreMeleeScript proven by its Main 00d51520 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd61c8 with name "Q_GuildTrainingPreMelee"; PDB ?Alloc@CQ_GuildTrainingPreMeleeScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012cf788; PDB ?Alloc@CQ_GuildTrainingPreMeleeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012cf788@@YIPAUCVtable_012cf788@@PAX0@Z` (00/d5/global_AllocCVtable012cf788_00d55d10.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQ_GuildTrainingPreMeleeScript.h (+ add-only opt-in FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT_ALLOC: CQ_GuildTrainingPreMeleeScript, Alloc), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012CF788 now carries the FableWin.pdb overrides (RegisterMain Main Init) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT
#define FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CQ_GuildTrainingPreMeleeScript.h"
inline NScript::CQ_GuildTrainingPreMeleeScript::CQ_GuildTrainingPreMeleeScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CQ_GuildTrainingPreMeleeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingPreMeleeScript(data, game);
}
