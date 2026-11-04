// NScript::CGuildSealTriggerTestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CGuildSealTriggerTestScript(data, game)
// Leaf: ?Alloc@CGuildSealTriggerTestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFC130, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012f6bd4 (outer CGuildSealTriggerTestScript proven by its Main 00efbca0 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cda571 with name "GuildSealTriggerTest"; PDB ?Alloc@CGuildSealTriggerTestScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012f6bd4; PDB ?Alloc@CGuildSealTriggerTestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f6bd4@@YIPAUCVtable_012f6bd4@@PAX0@Z` (00/ef/global_AllocCVtable012f6bd4_00efc130.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CDanyalTestScriptsData;
class CGuildSealTriggerTestScript : public CLevelScriptBase<CDanyalTestScriptsData> {
public:
    CGuildSealTriggerTestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CDanyalTestScriptsData>(data, game) { }
    virtual ~CGuildSealTriggerTestScript();    // ??1CGuildSealTriggerTestScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CGuildSealTriggerTestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CGuildSealTriggerTestScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CGuildSealTriggerTestScript) == 0x48 ? 1 : -1];
