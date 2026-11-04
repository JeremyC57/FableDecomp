// NScript::Cbandit_level_cutsceneScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new Cbandit_level_cutsceneScript(data, game)
// Leaf: ?Alloc@Cbandit_level_cutsceneScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFAF30, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f69e8 (outer Cbandit_level_cutsceneScript proven by its Main 00efa4f0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cda849 with name "bandit_level_cutscene"; PDB ?Alloc@Cbandit_level_cutsceneScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f69e8; PDB ?Alloc@Cbandit_level_cutsceneScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f69e8@@YIPAUCVtable_012f69e8@@PAX0@Z` (00/ef/global_AllocCVtable012f69e8_00efaf30.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class Cjoss_test_scriptsData;
class Cbandit_level_cutsceneScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    Cbandit_level_cutsceneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<Cjoss_test_scriptsData>(data, game) { }
    virtual ~Cbandit_level_cutsceneScript();    // ??1Cbandit_level_cutsceneScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall Cbandit_level_cutsceneScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new Cbandit_level_cutsceneScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::Cbandit_level_cutsceneScript) == 0x48 ? 1 : -1];
