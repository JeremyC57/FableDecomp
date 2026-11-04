// NScript::CTestNewNav3Script::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CTestNewNav3Script(data, game)
// Leaf: ?Alloc@CTestNewNav3Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEFC80, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eefc80 at 00cdabbe with name "TestNewNav3"; Alloc news + stores script vtable 012f5cec; PDB ?Alloc@CTestNewNav3Script@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f5cec; PDB ?Alloc@CTestNewNav3Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5cec@@YIPAUCVtable_012f5cec@@PAX0@Z` (00/ee/global_AllocCVtable012f5cec_00eefc80.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: the quest-script default, as the 34 shared script headers); layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CTestNewNav3Script : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CTestNewNav3Script(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CTestNewNav3Script();    // ??1CTestNewNav3Script@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CTestNewNav3Script::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CTestNewNav3Script(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CTestNewNav3Script) == 0x48 ? 1 : -1];
