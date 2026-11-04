// NScript::CQR_EscortTrader_ManagerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQR_EscortTrader_ManagerScript(data, game)
// Leaf: ?Alloc@CQR_EscortTrader_ManagerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25970, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00e25970 at 00cd7c0d with name "QR_EscortTrader_Manager"; Alloc news + stores script vtable 012e1d48; PDB ?Alloc@CQR_EscortTrader_ManagerScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012e1d48; PDB ?Alloc@CQR_EscortTrader_ManagerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e1d48@@YIPAUCVtable_012e1d48@@PAX0@Z` (00/e2/global_AllocCVtable012e1d48_00e25970.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTrader_ManagerScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQR_EscortTrader_ManagerScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQR_EscortTrader_ManagerScript();    // ??1CQR_EscortTrader_ManagerScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x58 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x58)
};
CScriptBase* __fastcall CQR_EscortTrader_ManagerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQR_EscortTrader_ManagerScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQR_EscortTrader_ManagerScript) == 0x58 ? 1 : -1];
