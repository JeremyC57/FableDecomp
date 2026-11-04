// NScript::CQ_BreakSiegeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_BreakSiegeScript(data, game)
// Leaf: ?Alloc@CQ_BreakSiegeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D23C80, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012cab18 (outer CQ_BreakSiegeScript proven by its Main 00d1d100 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd5c67 with name "Q_BreakSiege"; PDB ?Alloc@CQ_BreakSiegeScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012cab18; PDB ?Alloc@CQ_BreakSiegeScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012cab18@@YIPAUCVtable_012cab18@@PAX0@Z` (00/d2/global_AllocCVtable012cab18_00d23c80.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x5C = sizeof, Ego_r.pdb 112 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BreakSiegeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_BreakSiegeScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_BreakSiegeScript();    // ??1CQ_BreakSiegeScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x5c - 0x48];    // +0x48 (the script's own members; retail sizeof 0x5C)
};
CScriptBase* __fastcall CQ_BreakSiegeScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_BreakSiegeScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_BreakSiegeScript) == 0x5c ? 1 : -1];
