// NScript::CV_AmbushScamScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_AmbushScamScript(data, game)
// Leaf: ?Alloc@CV_AmbushScamScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E322C0, 40 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e2428 (outer CV_AmbushScamScript proven by its Main 00e2b090 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd7fd4 with name "V_AmbushScam"; PDB ?Alloc@CV_AmbushScamScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Create_CClass_00e2ab60_00e322c0; PDB ?Alloc@CV_AmbushScamScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Create_CClass_00e2ab60_00e322c0@@YIPAUCClass_00e2ab60@@PAX0@Z` (00/e3/global_CreateCClass00e2ab6000e322c0_00e322c0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x94 = sizeof), the out-of-line constructor 0x00E2AB60 (??0CV_AmbushScamScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, Ego_r.pdb / FableWin.pdb).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AmbushScamScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_AmbushScamScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);    // 0x00E2AB60
    virtual ~CV_AmbushScamScript();    // ??1CV_AmbushScamScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x94 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x94)
};
CScriptBase* __fastcall CV_AmbushScamScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_AmbushScamScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_AmbushScamScript) == 0x94 ? 1 : -1];
