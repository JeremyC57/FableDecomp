// NScript::CV_ChapelOfEvilScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_ChapelOfEvilScript(data, game)
// Leaf: ?Alloc@CV_ChapelOfEvilScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6E230, 68 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e8080 (outer CV_ChapelOfEvilScript proven by its Main 00e693f0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd85ce with name "V_ChapelOfEvil"; PDB ?Alloc@CV_ChapelOfEvilScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e8080; PDB ?Alloc@CV_ChapelOfEvilScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e8080@@YIPAUCVtable_012e8080@@PAX0@Z` (00/e6/global_AllocCVtable012e8080_00e6e230.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x90 = sizeof, Ego_r.pdb 164 - 0x14), CScriptBase::CScriptBase 0x00CB8110, CCharString::CCharString() 0x0099E4B0; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "fable_string.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChapelOfEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_ChapelOfEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_ChapelOfEvilScript();    // ??1CV_ChapelOfEvilScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x88 - 0x48];        // +0x48 (the script's own members before TortureSound)
    CCharString TortureSound;                        // +0x88 (Ego_r.pdb +0x9c; default ctor 0x0099E4B0)
    unsigned char _members8c[0x90 - 0x8c];    // +0x8c (the script's own members; retail sizeof 0x90)
};
CScriptBase* __fastcall CV_ChapelOfEvilScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_ChapelOfEvilScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_ChapelOfEvilScript) == 0x90 ? 1 : -1];
