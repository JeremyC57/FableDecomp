// NScript::CCS_PlayCutsceneScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CCS_PlayCutsceneScript(data, game)
// Leaf: ?Alloc@CCS_PlayCutsceneScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F01760, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00f01760 at 00cdb302 with name "CS_PlayCutscene"; Alloc news + stores script vtable 012f72d0; PDB ?Alloc@CCS_PlayCutsceneScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f72d0; PDB ?Alloc@CCS_PlayCutsceneScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f72d0@@YIPAUCVtable_012f72d0@@PAX0@Z` (00/f0/global_AllocCVtable012f72d0_00f01760.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: the quest-script default, as the 34 shared script headers); layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CCS_PlayCutsceneScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CCS_PlayCutsceneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CCS_PlayCutsceneScript();    // ??1CCS_PlayCutsceneScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CCS_PlayCutsceneScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CCS_PlayCutsceneScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CCS_PlayCutsceneScript) == 0x48 ? 1 : -1];
