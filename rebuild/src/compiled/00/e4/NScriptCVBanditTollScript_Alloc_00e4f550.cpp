// NScript::CV_BanditTollScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_BanditTollScript(data, game)
// Leaf: ?Alloc@CV_BanditTollScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4F550, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e4b7c (outer CV_BanditTollScript proven by its Main 00e479b0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd819f with name "V_BanditToll"; PDB ?Alloc@CV_BanditTollScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e4b7c; PDB ?Alloc@CV_BanditTollScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e4b7c@@YIPAUCVtable_012e4b7c@@PAX0@Z` (00/e4/global_AllocCVtable012e4b7c_00e4f550.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x5C = sizeof, Ego_r.pdb 112 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditTollScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BanditTollScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_BanditTollScript();    // ??1CV_BanditTollScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x5c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x5C)
};
CScriptBase* __fastcall CV_BanditTollScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_BanditTollScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_BanditTollScript) == 0x5c ? 1 : -1];
