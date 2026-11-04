// NScript::CTestTheInventoryScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CTestTheInventoryScript(data, game)
// Leaf: ?Alloc@CTestTheInventoryScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEEE40, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eeee40 at 00cda94a with name "TestTheInventory"; Alloc news + stores script vtable 012f5998; PDB ?Alloc@CTestTheInventoryScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f5998; PDB ?Alloc@CTestTheInventoryScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5998@@YIPAUCVtable_012f5998@@PAX0@Z` (00/ee/global_AllocCVtable012f5998_00eeee40.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CEmptyData;
class CTestTheInventoryScript : public CLevelScriptBase<CEmptyData> {
public:
    CTestTheInventoryScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    virtual ~CTestTheInventoryScript();    // ??1CTestTheInventoryScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x4C)
};
CScriptBase* __fastcall CTestTheInventoryScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CTestTheInventoryScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CTestTheInventoryScript) == 0x4c ? 1 : -1];
