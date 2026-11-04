// NScript::CBenTestScriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CBenTestScriptsScript(data, game)
// Leaf: ?Alloc@CBenTestScriptsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFF000, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f6ec0 (outer CBenTestScriptsScript proven by its Main 00efe4b0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cdb005 with name "BenTestScripts"; PDB ?Alloc@CBenTestScriptsScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f6ec0; PDB ?Alloc@CBenTestScriptsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f6ec0@@YIPAUCVtable_012f6ec0@@PAX0@Z` (00/ef/global_AllocCVtable012f6ec0_00eff000.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CBenTestScriptsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CBenTestScriptsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CBenTestScriptsScript();    // ??1CBenTestScriptsScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CBenTestScriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CBenTestScriptsScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CBenTestScriptsScript) == 0x48 ? 1 : -1];
