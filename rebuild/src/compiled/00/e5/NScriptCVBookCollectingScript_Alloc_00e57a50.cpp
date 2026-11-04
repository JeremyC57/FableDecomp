// NScript::CV_BookCollectingScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_BookCollectingScript(data, game)
// Leaf: ?Alloc@CV_BookCollectingScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E57A50, 40 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e5efc (outer CV_BookCollectingScript proven by its Main 00e54450 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd849c with name "V_BookCollecting"; PDB ?Alloc@CV_BookCollectingScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Create_CClass_00e54310_00e57a50; PDB ?Alloc@CV_BookCollectingScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Create_CClass_00e54310_00e57a50@@YIPAUCClass_00e54310@@PAX0@Z` (00/e5/global_CreateCClass00e5431000e57a50_00e57a50.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0xB8 = sizeof), the out-of-line constructor 0x00E54310 (??0CV_BookCollectingScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, Ego_r.pdb / FableWin.pdb).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BookCollectingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);    // 0x00E54310
    virtual ~CV_BookCollectingScript();    // ??1CV_BookCollectingScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0xb8 - 0x48];    // +0x48 (the script's own members; retail sizeof 0xB8)
};
CScriptBase* __fastcall CV_BookCollectingScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_BookCollectingScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_BookCollectingScript) == 0xb8 ? 1 : -1];
