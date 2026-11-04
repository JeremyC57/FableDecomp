// NScript::CV_RandomPopulationSimScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_RandomPopulationSimScript(data, game)
// Leaf: ?Alloc@CV_RandomPopulationSimScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC3BC0, 57 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f12d8 (outer CV_RandomPopulationSimScript proven by its Main 00ec1910 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd938d with name "V_RandomPopulationSim"; PDB ?Alloc@CV_RandomPopulationSimScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f12d8; PDB ?Alloc@CV_RandomPopulationSimScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f12d8@@YIPAUCVtable_012f12d8@@PAX0@Z` (00/ec/global_AllocCVtable012f12d8_00ec3bc0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x84 = sizeof, Ego_r.pdb 152 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_RandomPopulationSimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_RandomPopulationSimScript();    // ??1CV_RandomPopulationSimScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x84 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x84)
};
CScriptBase* __fastcall CV_RandomPopulationSimScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_RandomPopulationSimScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_RandomPopulationSimScript) == 0x84 ? 1 : -1];
