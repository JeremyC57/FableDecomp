// NScript::CQ_HangingTreeGoodScript::OnPersist(CPersistContext& context) -- saves/loads QuestStartScreenShown by name (default false)
// Leaf: ?OnPersist@CQ_HangingTreeGoodScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6F480, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012d260c slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d6f4b0 (slot 2) registers 10 entity scripts whose PDB Alloc outer is CQ_HangingTreeGoodScript; PDB ?OnPersist@CQ_HangingTreeGoodScript@NScript@@; refs=1; was _unattributed::PersistTransfer_QuestStartScreenShown_00d6f480.
//   ?OnPersist@CQ_HangingTreeGoodScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d6f480::PersistTransfer_QuestStartScreenShown_00d6f480(CPersistContext* context) -- persists the "QuestStartScreenShown" flag at +0x52 Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6F480, oracle length... (catalog stand-in _unattributed).
// Class: engine/CQ_HangingTreeGoodScript.h (shared header: CQ_HangingTreeGoodScript declares bool QuestStartScreenShown and OnPersist); includes as the landed includer rebuild/src/compiled/00/d7/NScriptCQHangingTreeGoodScript_DoMission_00d74430.cpp. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HangingTreeGoodScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_HangingTreeGoodScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStartScreenShown", QuestStartScreenShown, false);
}
