// NScript::CQS_GuardianSisterInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQS_GuardianSisterInfoScript(data, game)
// Leaf: ?Alloc@CQS_GuardianSisterInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E26780, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e1dac (outer CQS_GuardianSisterInfoScript proven by its Main 00e25ab0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd7cad with name "QS_GuardianSisterInfo"; PDB ?Alloc@CQS_GuardianSisterInfoScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e1dac; PDB ?Alloc@CQS_GuardianSisterInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e1dac@@YIPAUCVtable_012e1dac@@PAX0@Z` (00/e2/global_AllocCVtable012e1dac_00e26780.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianSisterInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQS_GuardianSisterInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQS_GuardianSisterInfoScript();    // ??1CQS_GuardianSisterInfoScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x4C)
};
CScriptBase* __fastcall CQS_GuardianSisterInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQS_GuardianSisterInfoScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQS_GuardianSisterInfoScript) == 0x4c ? 1 : -1];
