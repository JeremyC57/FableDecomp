// NScript::CQS_GuardianSisterInfoScript::OnPersist(CPersistContext& context) -- saves/loads GuardianSpokeToHero by name (default false)
// Leaf: ?OnPersist@CQS_GuardianSisterInfoScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25F40, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012e1dac slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e25ab0 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CQS_GuardianSisterInfoScript; PDB ?OnPersist@CQS_GuardianSisterInfoScript@NScript@@; refs=1; was _unattributed::PersistTransfer_GuardianSpokeToHero_00e25f40.
//   ?OnPersist@CQS_GuardianSisterInfoScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e25f40::PersistTransfer_GuardianSpokeToHero_00e25f40(CPersistContext* context) -- persists the "GuardianSpokeToHero" flag at +0x48 Retail: Fable.exe (TLC, VC7.1) function @ 0x00E25F40, oracle length 33 ... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00E259D0 (rebuild/src/compiled/00/e2/NScriptCQSGuardianSisterInfoScript_CQSGuardianSisterInfoScript_00e259d0.cpp); the PDB OnPersist declaration added; its bool GuardianSpokeToHero; members GuardianSpokeToHero +0x48 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CQ_SunnyvaleMasterData; }
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQS_GuardianSisterInfoScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CQS_GuardianSisterInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
public:
    bool GuardianSpokeToHero;   // +0x48 (persisted under this name; offset from the landed byte-matching OnPersist)
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQS_GuardianSisterInfoScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CQS_GuardianSisterInfoScript::CQS_GuardianSisterInfoScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CQS_GuardianSisterInfoScript::OnPersist(CPersistContext& context)
{
    context.Transfer("GuardianSpokeToHero", GuardianSpokeToHero, false);
}
