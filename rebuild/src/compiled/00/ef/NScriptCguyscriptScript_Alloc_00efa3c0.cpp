// NScript::Cguy_scriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new Cguy_scriptScript(data, game)
// Leaf: ?Alloc@Cguy_scriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFA3C0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f696c (outer Cguy_scriptScript proven by its Main 00efa010 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cda7ac with name "guy_script"; PDB ?Alloc@Cguy_scriptScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f696c; PDB ?Alloc@Cguy_scriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f696c@@YIPAUCVtable_012f696c@@PAX0@Z` (00/ef/global_AllocCVtable012f696c_00efa3c0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class Cjoss_test_scriptsData;
class Cguy_scriptScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    Cguy_scriptScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<Cjoss_test_scriptsData>(data, game) { }
    virtual ~Cguy_scriptScript();    // ??1Cguy_scriptScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall Cguy_scriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new Cguy_scriptScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::Cguy_scriptScript) == 0x48 ? 1 : -1];
