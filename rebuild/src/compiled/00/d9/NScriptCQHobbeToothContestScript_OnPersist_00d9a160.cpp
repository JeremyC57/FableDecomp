// NScript::CQ_HobbeToothContestScript::OnPersist(CPersistContext& context) -- saves/loads QuestStartScreenShown by name (default false)
// Leaf: ?OnPersist@CQ_HobbeToothContestScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9A160, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012d65cc slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d9a190 (slot 2) registers 7 entity scripts whose PDB Alloc outer is CQ_HobbeToothContestScript; PDB ?OnPersist@CQ_HobbeToothContestScript@NScript@@; refs=1; was _unattributed::PersistTransfer_QuestStartScreenShown_00d9a160.
//   ?OnPersist@CQ_HobbeToothContestScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d9a160::PersistTransfer_QuestStartScreenShown_00d9a160(CPersistContext* context) -- persists the "QuestStartScreenShown" flag at +0x64 Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9A160, oracle length... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00D99E40 (rebuild/src/compiled/00/d9/NScriptCQHobbeToothContestScript_CQHobbeToothContestScript_00d99e40.cpp); the PDB OnPersist declaration added; its bool QuestStartScreenShown; members QuestStartScreenShown +0x64 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"


namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeToothContestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HobbeToothContestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_HobbeToothContestScript();
    unsigned char _members[0x60 - 0x48];           // POD members up to the timer
    CTimer        WhisperCommentTimer;                            // +0x60
    bool QuestStartScreenShown;   // +0x64 (persisted under this name; offset from the landed byte-matching OnPersist)
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQ_HobbeToothContestScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};

CQ_HobbeToothContestScript::CQ_HobbeToothContestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CQ_HobbeToothContestScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStartScreenShown", QuestStartScreenShown, false);
}
