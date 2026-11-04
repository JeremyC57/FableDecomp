// NScript::CQ_HangingTreeEvilScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_HangingTreeEvilScript(data, game)
// Leaf: ?Alloc@CQ_HangingTreeEvilScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6ED10, 40 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012d1f98 (outer CQ_HangingTreeEvilScript proven by its Main 00d69480 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd6690 with name "Q_HangingTreeEvil"; PDB ?Alloc@CQ_HangingTreeEvilScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Create_CQ_HangingTreeEvilScript_00d6ed10; PDB ?Alloc@CQ_HangingTreeEvilScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Create_CQ_HangingTreeEvilScript_00d6ed10@@YIPAUCQ_HangingTreeEvilScript@@PAX0@Z` (00/d6/global_CreateCQHangingTreeEvilScript00d6ed10_00d6ed10.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0xD0 = sizeof), the out-of-line constructor 0x00D68F00 (??0CQ_HangingTreeEvilScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, Ego_r.pdb / FableWin.pdb).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HangingTreeEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);    // 0x00D68F00
    virtual ~CQ_HangingTreeEvilScript();    // ??1CQ_HangingTreeEvilScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0xd0 - 0x48];    // +0x48 (the script's own members; retail sizeof 0xD0)
};
CScriptBase* __fastcall CQ_HangingTreeEvilScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_HangingTreeEvilScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_HangingTreeEvilScript) == 0xd0 ? 1 : -1];
