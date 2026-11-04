// NScript::CQ_HerosOldHouseScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_HerosOldHouseScript(data, game)
// Leaf: ?Alloc@CQ_HerosOldHouseScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8D5A0, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012d5018 (outer CQ_HerosOldHouseScript proven by its Main 00d89e60 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd6b58 with name "Q_HerosOldHouse"; PDB ?Alloc@CQ_HerosOldHouseScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012d5018; PDB ?Alloc@CQ_HerosOldHouseScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012d5018@@YIPAUCVtable_012d5018@@PAX0@Z` (00/d8/global_AllocCVtable012d5018_00d8d5a0.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x50 = sizeof, Ego_r.pdb 100 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HerosOldHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HerosOldHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_HerosOldHouseScript();    // ??1CQ_HerosOldHouseScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x50 - 0x48];    // +0x48 (the script's own members; retail sizeof 0x50)
};
CScriptBase* __fastcall CQ_HerosOldHouseScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_HerosOldHouseScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_HerosOldHouseScript) == 0x50 ? 1 : -1];
