// NScript::CV_LostTraderScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_LostTraderScript(data, game)
// Leaf: ?Alloc@CV_LostTraderScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA7680, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012edce8 (outer CV_LostTraderScript proven by its Main 00ea2e80 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd8ec5 with name "V_LostTrader"; PDB ?Alloc@CV_LostTraderScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012edce8; PDB ?Alloc@CV_LostTraderScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012edce8@@YIPAUCVtable_012edce8@@PAX0@Z` (00/ea/global_AllocCVtable012edce8_00ea7680.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_LostTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_LostTraderScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_LostTraderScript();    // ??1CV_LostTraderScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x58 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x58)
};
CScriptBase* __fastcall CV_LostTraderScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_LostTraderScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_LostTraderScript) == 0x58 ? 1 : -1];
