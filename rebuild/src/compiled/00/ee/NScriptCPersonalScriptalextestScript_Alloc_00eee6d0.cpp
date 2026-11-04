// NScript::CPersonalScript_alextestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CPersonalScript_alextestScript(data, game)
// Leaf: ?Alloc@CPersonalScript_alextestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEE6D0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f5814 (outer CPersonalScript_alextestScript proven by its Main 00eed360 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cda336 with name "PersonalScript_alextest"; PDB ?Alloc@CPersonalScript_alextestScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f5814; PDB ?Alloc@CPersonalScript_alextestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5814@@YIPAUCVtable_012f5814@@PAX0@Z` (00/ee/global_AllocCVtable012f5814_00eee6d0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CalextestData;
class CPersonalScript_alextestScript : public CLevelScriptBase<CalextestData> {
public:
    CPersonalScript_alextestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CalextestData>(data, game) { }
    virtual ~CPersonalScript_alextestScript();    // ??1CPersonalScript_alextestScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CPersonalScript_alextestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CPersonalScript_alextestScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CPersonalScript_alextestScript) == 0x48 ? 1 : -1];
