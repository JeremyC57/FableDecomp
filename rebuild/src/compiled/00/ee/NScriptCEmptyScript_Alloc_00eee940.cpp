// NScript::CEmptyScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CEmptyScript(data, game)
// Leaf: ?Alloc@CEmptyScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEE940, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eee940 at 00cda8d1 with name "Empty"; Alloc news + stores script vtable 012f5954; PDB ?Alloc@CEmptyScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f5954; PDB ?Alloc@CEmptyScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5954@@YIPAUCVtable_012f5954@@PAX0@Z` (00/ee/global_AllocCVtable012f5954_00eee940.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CEmptyData;
class CEmptyScript : public CLevelScriptBase<CEmptyData> {
public:
    CEmptyScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    virtual ~CEmptyScript();    // ??1CEmptyScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CEmptyScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CEmptyScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CEmptyScript) == 0x48 ? 1 : -1];
