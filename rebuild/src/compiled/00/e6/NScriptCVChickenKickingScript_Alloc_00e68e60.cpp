// NScript::CV_ChickenKickingScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_ChickenKickingScript(data, game)
// Leaf: ?Alloc@CV_ChickenKickingScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E68E60, 40 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e7318 (outer CV_ChickenKickingScript proven by its Main 00e62a00 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd8700 with name "V_ChickenKicking"; PDB ?Alloc@CV_ChickenKickingScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Create_CClass_00e62830_00e68e60; PDB ?Alloc@CV_ChickenKickingScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Create_CClass_00e62830_00e68e60@@YIPAUCClass_00e62830@@PAX0@Z` (00/e6/global_CreateCClass00e6283000e68e60_00e68e60.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x90 = sizeof), the out-of-line constructor 0x00E62830 (??0CV_ChickenKickingScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, Ego_r.pdb / FableWin.pdb).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChickenKickingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_ChickenKickingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);    // 0x00E62830
    virtual ~CV_ChickenKickingScript();    // ??1CV_ChickenKickingScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x90 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x90)
};
CScriptBase* __fastcall CV_ChickenKickingScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_ChickenKickingScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_ChickenKickingScript) == 0x90 ? 1 : -1];
