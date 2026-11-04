// NScript::CPersonalScript_GlobalThingsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScript_GlobalThingsScript(data, game)
// Leaf: ?Alloc@CPersonalScript_GlobalThingsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE19A0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012c30cc (outer CPersonalScript_GlobalThingsScript proven by its Main 00cde420 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd5511 with name "PersonalScript_GlobalThings"; PDB ?Alloc@CPersonalScript_GlobalThingsScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012c30cc; PDB ?Alloc@CPersonalScript_GlobalThingsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c30cc@@YIPAUCVtable_012c30cc@@PAX0@Z` (00/ce/global_AllocCVtable012c30cc_00ce19a0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_GlobalThingsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CPersonalScript_GlobalThingsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CPersonalScript_GlobalThingsScript();    // ??1CPersonalScript_GlobalThingsScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CPersonalScript_GlobalThingsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScript_GlobalThingsScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CPersonalScript_GlobalThingsScript) == 0x48 ? 1 : -1];
