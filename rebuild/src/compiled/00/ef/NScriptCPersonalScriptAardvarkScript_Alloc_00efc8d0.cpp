// NScript::CPersonalScript_AardvarkScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScript_AardvarkScript(data, game)
// Leaf: ?Alloc@CPersonalScript_AardvarkScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFC8D0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00efc8d0 at 00cda60e with name "PersonalScript_Aardvark"; Alloc news + stores script vtable 012f6ca8; PDB ?Alloc@CPersonalScript_AardvarkScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f6ca8; PDB ?Alloc@CPersonalScript_AardvarkScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f6ca8@@YIPAUCVtable_012f6ca8@@PAX0@Z` (00/ef/global_AllocCVtable012f6ca8_00efc8d0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: the quest-script default, as the 34 shared script headers); layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_AardvarkScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CPersonalScript_AardvarkScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CPersonalScript_AardvarkScript();    // ??1CPersonalScript_AardvarkScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x4C)
};
CScriptBase* __fastcall CPersonalScript_AardvarkScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScript_AardvarkScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CPersonalScript_AardvarkScript) == 0x4c ? 1 : -1];
