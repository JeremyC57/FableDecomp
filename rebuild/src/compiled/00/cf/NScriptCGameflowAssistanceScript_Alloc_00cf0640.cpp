// NScript::CGameflowAssistanceScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CGameflowAssistanceScript(data, game)
// Leaf: ?Alloc@CGameflowAssistanceScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF0640, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00cf0640 at 00cd5643 with name "GameflowAssistance"; Alloc news + stores script vtable 012c5de0; PDB ?Alloc@CGameflowAssistanceScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012c5de0; PDB ?Alloc@CGameflowAssistanceScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c5de0@@YIPAUCVtable_012c5de0@@PAX0@Z` (00/cf/global_AllocCVtable012c5de0_00cf0640.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x68 = sizeof, Ego_r.pdb 124 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGameflowAssistanceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CGameflowAssistanceScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CGameflowAssistanceScript();    // ??1CGameflowAssistanceScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x68 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x68)
};
CScriptBase* __fastcall CGameflowAssistanceScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CGameflowAssistanceScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CGameflowAssistanceScript) == 0x68 ? 1 : -1];
