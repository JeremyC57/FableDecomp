// NScript::CHeroBoastsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CHeroBoastsScript(data, game)
// Leaf: ?Alloc@CHeroBoastsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE6C40, 62 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ce6c40 at 00cd53df with name "HeroBoasts"; Alloc news + stores script vtable 012c3688; PDB ?Alloc@CHeroBoastsScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012c3688; PDB ?Alloc@CHeroBoastsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c3688@@YIPAUCVtable_012c3688@@PAX0@Z` (00/ce/global_AllocCVtable012c3688_00ce6c40.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x50 = sizeof, Ego_r.pdb 100 - 0x14), CScriptBase::CScriptBase 0x00CB8110, CCharString::CCharString() 0x0099E4B0; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "fable_string.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CHeroBoastsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CHeroBoastsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CHeroBoastsScript();    // ??1CHeroBoastsScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    CCharString CurrentQuest;                        // +0x48 (Ego_r.pdb +0x5c; default ctor 0x0099E4B0)
    unsigned char _members4c[0x50 - 0x4c];    // +0x4c (the script's own members; retail sizeof 0x50)
};
CScriptBase* __fastcall CHeroBoastsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CHeroBoastsScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CHeroBoastsScript) == 0x50 ? 1 : -1];
