// NScript::CQ_GuildTrainingDepartureScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_GuildTrainingDepartureScript(data, game)
// Leaf: ?Alloc@CQ_GuildTrainingDepartureScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D513C0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012cf670 (outer CQ_GuildTrainingDepartureScript proven by its Main 00d50800 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd612f with name "Q_GuildTrainingDeparture"; PDB ?Alloc@CQ_GuildTrainingDepartureScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012cf670; PDB ?Alloc@CQ_GuildTrainingDepartureScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012cf670@@YIPAUCVtable_012cf670@@PAX0@Z` (00/d5/global_AllocCVtable012cf670_00d513c0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingDepartureScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_GuildTrainingDepartureScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_GuildTrainingDepartureScript();    // ??1CQ_GuildTrainingDepartureScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x58 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x58)
};
CScriptBase* __fastcall CQ_GuildTrainingDepartureScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingDepartureScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_GuildTrainingDepartureScript) == 0x58 ? 1 : -1];
