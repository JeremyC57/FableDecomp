// NScript::CQ_BanditCampScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_BanditCampScript(data, game)
// Leaf: ?Alloc@CQ_BanditCampScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D03580, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012c8300 (outer CQ_BanditCampScript proven by its Main 00d00af0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd5a03 with name "Q_BanditCamp"; PDB ?Alloc@CQ_BanditCampScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012c8300; PDB ?Alloc@CQ_BanditCampScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c8300@@YIPAUCVtable_012c8300@@PAX0@Z` (00/d0/global_AllocCVtable012c8300_00d03580.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x50 = sizeof, Ego_r.pdb 100 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_BanditCampScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_BanditCampScript();    // ??1CQ_BanditCampScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x50 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x50)
};
CScriptBase* __fastcall CQ_BanditCampScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_BanditCampScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_BanditCampScript) == 0x50 ? 1 : -1];
