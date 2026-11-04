// NScript::CV_GuildMasterScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_GuildMasterScript(data, game)
// Leaf: ?Alloc@CV_GuildMasterScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E92900, 62 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: level-script Alloc: operator new + stores script vtable 012ec138 (outer CV_GuildMasterScript proven by its Main 00e90830 entity registrations); CScriptInfoManager::RegisterScripts 00cd52d0 stores it at 00cd8a96 with name "V_GuildMaster"; PDB ?Alloc@CV_GuildMasterScript@NScript@@SIPAVCScriptBase@@...; refs=1 (RegisterScripts only); was _global::Alloc_CVtable_012ec138; PDB ?Alloc@CV_GuildMasterScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012ec138@@YIPAUCVtable_012ec138@@PAX0@Z` (00/e9/global_AllocCVtable012ec138_00e92900.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x54 = sizeof, Ego_r.pdb 104 - 0x14), CScriptBase::CScriptBase 0x00CB8110, CCharString::CCharString() 0x0099E4B0; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "fable_string.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_GuildMasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_GuildMasterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_GuildMasterScript();    // ??1CV_GuildMasterScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members48[0x50 - 0x48];        // +0x48 (the script's own members before LastDialogueSaid)
    CCharString LastDialogueSaid;                    // +0x50 (Ego_r.pdb +0x64; default ctor 0x0099E4B0)
};
CScriptBase* __fastcall CV_GuildMasterScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_GuildMasterScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_GuildMasterScript) == 0x54 ? 1 : -1];
