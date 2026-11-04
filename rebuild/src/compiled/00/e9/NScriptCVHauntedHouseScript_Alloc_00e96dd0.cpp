// NScript::CV_HauntedHouseScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_HauntedHouseScript(data, game)
// Leaf: ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E96DD0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00e96dd0 at 00cd8b2f with name "V_HauntedHouse"; Alloc news + stores script vtable 012ecb94; PDB ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012ecb94; PDB ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012ecb94@@YIPAUCVtable_012ecb94@@PAX0@Z` (00/e9/global_AllocCVtable012ecb94_00e96dd0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x5C = sizeof, Ego_r.pdb 112 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HauntedHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_HauntedHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_HauntedHouseScript();    // ??1CV_HauntedHouseScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x5c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x5C)
};
CScriptBase* __fastcall CV_HauntedHouseScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_HauntedHouseScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_HauntedHouseScript) == 0x5c ? 1 : -1];
