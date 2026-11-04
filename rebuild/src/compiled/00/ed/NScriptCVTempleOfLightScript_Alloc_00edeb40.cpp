// NScript::CV_TempleOfLightScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_TempleOfLightScript(data, game)
// Leaf: ?Alloc@CV_TempleOfLightScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDEB40, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f3c68 (outer CV_TempleOfLightScript proven by its Main 00edb190 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd9855 with name "V_TempleOfLight"; PDB ?Alloc@CV_TempleOfLightScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f3c68; PDB ?Alloc@CV_TempleOfLightScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f3c68@@YIPAUCVtable_012f3c68@@PAX0@Z` (00/ed/global_AllocCVtable012f3c68_00edeb40.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x74 = sizeof, Ego_r.pdb 136 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TempleOfLightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_TempleOfLightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_TempleOfLightScript();    // ??1CV_TempleOfLightScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x74 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x74)
};
CScriptBase* __fastcall CV_TempleOfLightScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_TempleOfLightScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_TempleOfLightScript) == 0x74 ? 1 : -1];
