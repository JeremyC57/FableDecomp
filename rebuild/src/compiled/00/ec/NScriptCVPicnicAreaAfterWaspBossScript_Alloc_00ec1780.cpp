// NScript::CV_PicnicAreaAfterWaspBossScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_PicnicAreaAfterWaspBossScript(data, game)
// Leaf: ?Alloc@CV_PicnicAreaAfterWaspBossScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC1780, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f1230 (outer CV_PicnicAreaAfterWaspBossScript proven by its Main 00ec12e0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd92f4 with name "V_PicnicAreaAfterWaspBoss"; PDB ?Alloc@CV_PicnicAreaAfterWaspBossScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f1230; PDB ?Alloc@CV_PicnicAreaAfterWaspBossScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f1230@@YIPAUCVtable_012f1230@@PAX0@Z` (00/ec/global_AllocCVtable012f1230_00ec1780.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_PicnicAreaAfterWaspBossScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_PicnicAreaAfterWaspBossScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_PicnicAreaAfterWaspBossScript();    // ??1CV_PicnicAreaAfterWaspBossScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CV_PicnicAreaAfterWaspBossScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_PicnicAreaAfterWaspBossScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_PicnicAreaAfterWaspBossScript) == 0x48 ? 1 : -1];
