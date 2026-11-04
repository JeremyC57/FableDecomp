// NScript::CHeroTurningDebuggerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CHeroTurningDebuggerScript(data, game)
// Leaf: ?Alloc@CHeroTurningDebuggerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFC460, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00efc460 at 00cda6ab with name "HeroTurningDebugger"; Alloc news + stores script vtable 012f6c58; PDB ?Alloc@CHeroTurningDebuggerScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f6c58; PDB ?Alloc@CHeroTurningDebuggerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f6c58@@YIPAUCVtable_012f6c58@@PAX0@Z` (00/ef/global_AllocCVtable012f6c58_00efc460.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model: engine/CLevelScriptBase.h; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this
//   body does not touch are one pad block.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CDanyalTestScriptsData;
class CHeroTurningDebuggerScript : public CLevelScriptBase<CDanyalTestScriptsData> {
public:
    CHeroTurningDebuggerScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CDanyalTestScriptsData>(data, game) { }
    virtual ~CHeroTurningDebuggerScript();    // ??1CHeroTurningDebuggerScript@NScript@@UAE@XZ
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
CScriptBase* __fastcall CHeroTurningDebuggerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CHeroTurningDebuggerScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CHeroTurningDebuggerScript) == 0x48 ? 1 : -1];
