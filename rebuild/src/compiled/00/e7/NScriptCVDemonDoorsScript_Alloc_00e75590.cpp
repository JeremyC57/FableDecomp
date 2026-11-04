// NScript::CV_DemonDoorsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_DemonDoorsScript(data, game)
// Leaf: ?Alloc@CV_DemonDoorsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E75590, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012e8b10 (outer CV_DemonDoorsScript proven by its Main 00e6e380 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd8799 with name "V_DemonDoors"; PDB ?Alloc@CV_DemonDoorsScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012e8b10; PDB ?Alloc@CV_DemonDoorsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e8b10@@YIPAUCVtable_012e8b10@@PAX0@Z` (00/e7/global_AllocCVtable012e8b10_00e75590.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_DemonDoorsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_DemonDoorsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_DemonDoorsScript();    // ??1CV_DemonDoorsScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CV_DemonDoorsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_DemonDoorsScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_DemonDoorsScript) == 0x48 ? 1 : -1];
