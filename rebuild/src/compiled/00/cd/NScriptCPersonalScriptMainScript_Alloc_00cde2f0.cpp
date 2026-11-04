// NScript::CPersonalScriptMainScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScriptMainScript(data, game)
// Leaf: ?Alloc@CPersonalScriptMainScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDE2F0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00cde2f0 at 00cd5478 with name "PersonalScriptMain"; Alloc news + stores script vtable 012c3000; PDB ?Alloc@CPersonalScriptMainScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012c3000; PDB ?Alloc@CPersonalScriptMainScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c3000@@YIPAUCVtable_012c3000@@PAX0@Z` (00/cd/global_AllocCVtable012c3000_00cde2f0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: the quest-script default, as the 34 shared script headers); layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScriptMainScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CPersonalScriptMainScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CPersonalScriptMainScript();    // ??1CPersonalScriptMainScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CPersonalScriptMainScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScriptMainScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CPersonalScriptMainScript) == 0x48 ? 1 : -1];
