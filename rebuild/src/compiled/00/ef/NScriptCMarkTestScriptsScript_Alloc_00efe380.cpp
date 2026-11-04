// NScript::CMarkTestScriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CMarkTestScriptsScript(data, game)
// Leaf: ?Alloc@CMarkTestScriptsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFE380, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f6d00 (outer CMarkTestScriptsScript proven by its Main 00efca00 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cdaf6c with name "MarkTestScripts"; PDB ?Alloc@CMarkTestScriptsScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f6d00; PDB ?Alloc@CMarkTestScriptsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f6d00@@YIPAUCVtable_012f6d00@@PAX0@Z` (00/ef/global_AllocCVtable012f6d00_00efe380.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CMarkTestScriptsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CMarkTestScriptsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CMarkTestScriptsScript();    // ??1CMarkTestScriptsScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CMarkTestScriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CMarkTestScriptsScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CMarkTestScriptsScript) == 0x48 ? 1 : -1];
