// NScript::CQ_BanditCampHoldingScriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_BanditCampHoldingScriptScript(data, game)
// Leaf: ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D12C50, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00d12c50 at 00cd5b35 with name "Q_BanditCampHoldingScript"; Alloc news + stores script vtable 012c9e14; PDB ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012c9e14; PDB ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c9e14@@YIPAUCVtable_012c9e14@@PAX0@Z` (00/d1/global_AllocCVtable012c9e14_00d12c50.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: the quest-script default, as the 34 shared script headers); layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampHoldingScriptScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_BanditCampHoldingScriptScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_BanditCampHoldingScriptScript();    // ??1CQ_BanditCampHoldingScriptScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x4C)
};
CScriptBase* __fastcall CQ_BanditCampHoldingScriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_BanditCampHoldingScriptScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_BanditCampHoldingScriptScript) == 0x4c ? 1 : -1];
