// NScript::CPersonalScript_AardvarkScript::OnPersist(CPersistContext& context) -- saves/loads ScriptVar by name (default 0L)
// Leaf: ?OnPersist@CPersonalScript_AardvarkScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFC8A0, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00efc8d0 at 00cda60e with name "PersonalScript_Aardvark"; Alloc news + stores script vtable 012f6ca8; PDB ?Alloc@CPersonalScript_AardvarkScript@NScript@@SIPAVCScriptBase@@; script vtable slot 4 (slot 1 pushes "Main" 0x012C2F94); PDB OnPersist; refs=1; was _unattributed::PersistTransfer_ScriptVar_00efc8a0.
//   ?OnPersist@CPersonalScript_AardvarkScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00efc8a0::PersistTransfer_ScriptVar_00efc8a0(CPersistContext* context) -- persists the ScriptVar dword at +0x48 with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFC8A0, oracle length 35 b... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00EFC4C0 (rebuild/src/compiled/00/ef/NScriptCPersonalScriptAardvarkScript_CPersonalScriptAardvarkScript_00efc4c0.cpp); the PDB OnPersist declaration added; its long ScriptVar; members ScriptVar +0x48 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CDanyalTestScriptsData; }
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CPersonalScript_AardvarkScript : public CLevelScriptBase<NScript::CDanyalTestScriptsData> {
public:
    CPersonalScript_AardvarkScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
public:
    long ScriptVar;   // +0x48 (persisted under this name; offset from the landed byte-matching OnPersist)
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CPersonalScript_AardvarkScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CPersonalScript_AardvarkScript::CPersonalScript_AardvarkScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CDanyalTestScriptsData>(data, game)
{
}
}

void NScript::CPersonalScript_AardvarkScript::OnPersist(CPersistContext& context)
{
    context.Transfer("ScriptVar", ScriptVar, 0L);
}
