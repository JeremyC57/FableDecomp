// CGameScriptInterface::FailAllActiveQuests() const -- the script call forwards to the quest manager singleton: CQuestManager::FailAllActiveQuests
// Leaf: ?FailAllActiveQuests@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00891520, 11 bytes: `mov ecx,[0x013B89FC] ; jmp 0x004B2AD0` (vtable slot 300 of CGameScriptInterface,
//   engine/CGameScriptInterface.h; Ego_r.pdb 0x008CE2A0 / FableWin.pdb 0x02A72160 ?FailAllActiveQuests@CGameScriptInterface@@UBEXXZ).
// Callee: CQuestManager::FailAllActiveQuests 0x004B2AD0 (Ego_r.pdb ?FailAllActiveQuests@CQuestManager@@QAEXXZ, the landed self Leaf) on
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x013B89FC (Name fix agentNF7 2026-11-10, reached through the public inline Get()).
// NF166 (round 10, q5000): moved onto the shared headers (overlay opt-in FABLE_RETAIL_CQUESTMANAGER_FAILALLACTIVEQUESTS). Was the free
//   __fastcall stand-in CGameScriptInterface_FailAllActiveQuests calling QuestMgr_FailAll(CQuestMgr*) (both stand-in names).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_FAILALLACTIVEQUESTS
#include "engine/CGameScriptInterface.h"
#include "engine/CQuestManager.h"

void CGameScriptInterface::FailAllActiveQuests() const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().FailAllActiveQuests();
}
