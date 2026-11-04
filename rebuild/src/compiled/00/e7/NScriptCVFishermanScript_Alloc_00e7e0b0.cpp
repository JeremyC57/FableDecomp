// NScript::CV_FishermanScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_FishermanScript(data, game)
// Leaf: ?Alloc@CV_FishermanScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7E0B0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012ea024 (outer CV_FishermanScript proven by its Main 00e7b4e0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd8832 with name "V_Fisherman"; PDB ?Alloc@CV_FishermanScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012ea024; PDB ?Alloc@CV_FishermanScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012ea024@@YIPAUCVtable_012ea024@@PAX0@Z` (00/e7/global_AllocCVtable012ea024_00e7e0b0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_FishermanScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_FishermanScript();    // ??1CV_FishermanScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x58 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x58)
};
CScriptBase* __fastcall CV_FishermanScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_FishermanScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_FishermanScript) == 0x58 ? 1 : -1];
