// NScript::CV_MurderTwistFinalScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_MurderTwistFinalScript(data, game)
// Leaf: ?Alloc@CV_MurderTwistFinalScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6A60, 40 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012ef978 (outer CV_MurderTwistFinalScript proven by its Main 00eaea20 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd9090 with name "V_MurderTwistFinal"; PDB ?Alloc@CV_MurderTwistFinalScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Create_CClass_00eae910_00eb6a60; PDB ?Alloc@CV_MurderTwistFinalScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Create_CClass_00eae910_00eb6a60@@YIPAUCClass_00eae910@@PAX0@Z` (00/eb/global_CreateCClass00eae91000eb6a60_00eb6a60.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x88 = sizeof), the out-of-line constructor 0x00EAE910 (??0CV_MurderTwistFinalScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, Ego_r.pdb / FableWin.pdb).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_MurderTwistFinalScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);    // 0x00EAE910
    virtual ~CV_MurderTwistFinalScript();    // ??1CV_MurderTwistFinalScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x88 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x88)
};
CScriptBase* __fastcall CV_MurderTwistFinalScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_MurderTwistFinalScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_MurderTwistFinalScript) == 0x88 ? 1 : -1];
