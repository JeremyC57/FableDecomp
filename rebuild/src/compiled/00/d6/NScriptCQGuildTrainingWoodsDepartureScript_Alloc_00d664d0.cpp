// NScript::CQ_GuildTrainingWoodsDepartureScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_GuildTrainingWoodsDepartureScript(data, game)
// Leaf: ?Alloc@CQ_GuildTrainingWoodsDepartureScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D664D0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012d15fc (outer CQ_GuildTrainingWoodsDepartureScript proven by its Main 00d61d20 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd642c with name "Q_GuildTrainingWoodsDeparture"; PDB ?Alloc@CQ_GuildTrainingWoodsDepartureScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012d15fc; PDB ?Alloc@CQ_GuildTrainingWoodsDepartureScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012d15fc@@YIPAUCVtable_012d15fc@@PAX0@Z` (00/d6/global_AllocCVtable012d15fc_00d664d0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x50 = sizeof, Ego_r.pdb 100 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingWoodsDepartureScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_GuildTrainingWoodsDepartureScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_GuildTrainingWoodsDepartureScript();    // ??1CQ_GuildTrainingWoodsDepartureScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x50 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x50)
};
CScriptBase* __fastcall CQ_GuildTrainingWoodsDepartureScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_GuildTrainingWoodsDepartureScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_GuildTrainingWoodsDepartureScript) == 0x50 ? 1 : -1];
