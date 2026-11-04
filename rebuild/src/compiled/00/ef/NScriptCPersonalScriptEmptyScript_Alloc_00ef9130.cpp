// NScript::CPersonalScript_EmptyScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScript_EmptyScript(data, game)
// Leaf: ?Alloc@CPersonalScript_EmptyScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF9130, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f5d7c (outer CPersonalScript_EmptyScript proven by its Main 00ef0090 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cda9e7 with name "PersonalScript_Empty"; PDB ?Alloc@CPersonalScript_EmptyScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f5d7c; PDB ?Alloc@CPersonalScript_EmptyScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5d7c@@YIPAUCVtable_012f5d7c@@PAX0@Z` (00/ef/global_AllocCVtable012f5d7c_00ef9130.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CEmptyData;
class CPersonalScript_EmptyScript : public CLevelScriptBase<CEmptyData> {
public:
    CPersonalScript_EmptyScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    virtual ~CPersonalScript_EmptyScript();    // ??1CPersonalScript_EmptyScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x4C)
};
CScriptBase* __fastcall CPersonalScript_EmptyScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScript_EmptyScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CPersonalScript_EmptyScript) == 0x4c ? 1 : -1];
