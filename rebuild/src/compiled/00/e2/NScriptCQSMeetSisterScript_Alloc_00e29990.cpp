// NScript::CQS_MeetSisterScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQS_MeetSisterScript(data, game)
// Leaf: ?Alloc@CQS_MeetSisterScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E29990, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e2194 (outer CQS_MeetSisterScript proven by its Main 00e28de0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd7e6e with name "QS_MeetSister"; PDB ?Alloc@CQS_MeetSisterScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e2194; PDB ?Alloc@CQS_MeetSisterScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e2194@@YIPAUCVtable_012e2194@@PAX0@Z` (00/e2/global_AllocCVtable012e2194_00e29990.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_MeetSisterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQS_MeetSisterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQS_MeetSisterScript();    // ??1CQS_MeetSisterScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CQS_MeetSisterScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQS_MeetSisterScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQS_MeetSisterScript) == 0x48 ? 1 : -1];
