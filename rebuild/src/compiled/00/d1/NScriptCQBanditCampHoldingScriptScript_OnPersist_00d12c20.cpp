// NScript::CQ_BanditCampHoldingScriptScript::OnPersist(CPersistContext& context) -- saves/loads PathReached by name (default false)
// Leaf: ?OnPersist@CQ_BanditCampHoldingScriptScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D12C20, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00d12c50 at 00cd5b35 with name "Q_BanditCampHoldingScript"; Alloc news + stores script vtable 012c9e14; PDB ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@; script vtable slot 4 (slot 1 pushes "Main" 0x012C2F94); PDB OnPersist; refs=1; was _unattributed::PersistTransfer_PathReached_00d12c20.
//   ?OnPersist@CQ_BanditCampHoldingScriptScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d12c20::PersistTransfer_PathReached_00d12c20(CPersistContext* context) -- persists the "PathReached" flag at +0x48 Retail: Fable.exe (TLC, VC7.1) function @ 0x00D12C20, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00D12980 (rebuild/src/compiled/00/d1/NScriptCQBanditCampHoldingScriptScript_CQBanditCampHoldingScriptScript_00d12980.cpp); the PDB OnPersist declaration added; its bool PathReached; members PathReached +0x48 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
class CQ_BanditCampHoldingScriptScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CQ_BanditCampHoldingScriptScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
public:
    bool PathReached;   // +0x48 (persisted under this name; offset from the landed byte-matching OnPersist)
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQ_BanditCampHoldingScriptScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CQ_BanditCampHoldingScriptScript::CQ_BanditCampHoldingScriptScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CQ_BanditCampHoldingScriptScript::OnPersist(CPersistContext& context)
{
    context.Transfer("PathReached", PathReached, false);
}
