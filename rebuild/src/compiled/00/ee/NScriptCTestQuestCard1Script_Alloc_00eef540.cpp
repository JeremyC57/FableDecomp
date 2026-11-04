// NScript::CTestQuestCard1Script::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CTestQuestCard1Script(data, game)
// Leaf: ?Alloc@CTestQuestCard1Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEF540, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eef540 at 00cdad95 with name "TestQuestCard1"; Alloc news + stores script vtable 012f5a78; PDB ?Alloc@CTestQuestCard1Script@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f5a78; PDB ?Alloc@CTestQuestCard1Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5a78@@YIPAUCVtable_012f5a78@@PAX0@Z` (00/ee/global_AllocCVtable012f5a78_00eef540.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CEmptyData;
class CTestQuestCard1Script : public CLevelScriptBase<CEmptyData> {
public:
    CTestQuestCard1Script(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    virtual ~CTestQuestCard1Script();    // ??1CTestQuestCard1Script@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x4C)
};
CScriptBase* __fastcall CTestQuestCard1Script::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CTestQuestCard1Script(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CTestQuestCard1Script) == 0x4c ? 1 : -1];
