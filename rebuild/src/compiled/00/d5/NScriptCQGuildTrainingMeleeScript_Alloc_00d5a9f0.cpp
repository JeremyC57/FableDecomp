// NScript::CQ_GuildTrainingMeleeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_GuildTrainingMeleeScript(data, game)
// Leaf: ?Alloc@CQ_GuildTrainingMeleeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D5A9F0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012d0274 (outer CQ_GuildTrainingMeleeScript proven by its Main 00d55e90 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd6261 with name "Q_GuildTrainingMelee"; PDB ?Alloc@CQ_GuildTrainingMeleeScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012d0274; PDB ?Alloc@CQ_GuildTrainingMeleeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012d0274@@YIPAUCVtable_012d0274@@PAX0@Z` (00/d5/global_AllocCVtable012d0274_00d5a9f0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQ_GuildTrainingMeleeScript.h (+ add-only opt-in FABLE_RETAIL_CQ_GUILDTRAININGMELEESCRIPT_ALLOC: CQ_GuildTrainingMeleeScript, Alloc, RegisterMain, Main, Init), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012D0274 now carries the FableWin.pdb overrides (RegisterMain Main Init) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_GUILDTRAININGMELEESCRIPT_ALLOC
#include "rebuild_abi.h"
#include "engine/CQ_GuildTrainingMeleeScript.h"
inline NScript::CQ_GuildTrainingMeleeScript::CQ_GuildTrainingMeleeScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CQ_GuildTrainingMeleeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingMeleeScript(data, game);
}
