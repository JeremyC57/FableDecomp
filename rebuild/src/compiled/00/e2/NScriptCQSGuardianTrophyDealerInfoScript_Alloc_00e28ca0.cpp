// NScript::CQS_GuardianTrophyDealerInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQS_GuardianTrophyDealerInfoScript(data, game)
// Leaf: ?Alloc@CQS_GuardianTrophyDealerInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E28CA0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e2018 (outer CQS_GuardianTrophyDealerInfoScript proven by its Main 00e27920 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd7ddf with name "QS_GuardianTrophyDealerInfo"; PDB ?Alloc@CQS_GuardianTrophyDealerInfoScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e2018; PDB ?Alloc@CQS_GuardianTrophyDealerInfoScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e2018@@YIPAUCVtable_012e2018@@PAX0@Z` (00/e2/global_AllocCVtable012e2018_00e28ca0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianTrophyDealerInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQS_GuardianTrophyDealerInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQS_GuardianTrophyDealerInfoScript();    // ??1CQS_GuardianTrophyDealerInfoScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x4C)
};
CScriptBase* __fastcall CQS_GuardianTrophyDealerInfoScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQS_GuardianTrophyDealerInfoScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQS_GuardianTrophyDealerInfoScript) == 0x4c ? 1 : -1];
