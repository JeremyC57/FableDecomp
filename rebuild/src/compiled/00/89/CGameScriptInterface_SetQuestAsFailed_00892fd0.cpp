// CGameScriptInterface::SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const -- fail the quest through the quest manager singleton; when no fail screen is shown and the flag at 0x01375741 is set, report the quest's finish screen as displayed; then the script-interface call at 0x006E71F0 with 1.0f
// Leaf: ?SetQuestAsFailed@CGameScriptInterface@@UBEXABVCCharString@@_NABVCWideString@@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00892FD0, 79 bytes, `ret 10h` (vtable slot 289 of CGameScriptInterface, engine/CGameScriptInterface.h;
//   Ego_r.pdb 0x008D0160 / FableWin.pdb 0x02AAC9C0 ?SetQuestAsFailed@CGameScriptInterface@@UBEXABVCCharString@@_NABVCWideString@@1@Z).
// Callees: CQuestManager::SetQuestAsFailed 0x004B2280 (Ego_r.pdb / FableWin.pdb ?SetQuestAsFailed@CQuestManager@@QAE_NABVCCharString@@_NABVCWideString@@1@Z,
//   the landed self Leaf) and CQuestManager::OnQuestFinishScreenDisplayed 0x004B0160 (Ego_r.pdb / FableWin.pdb, engine/CQuestManager.h
//   FINISHSCREEN), both on CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x013B89FC (Name fix agentNF7 2026-11-10).
// Kept from the landed row (identities not proven here, see NOTES): the byte at 0x01375741 (g_someFlag) and the 0x006E71F0 call on `this`
//   (applyFloat on the HEAD local CGameScriptInterface_Methods).
// NF166 (round 10, q5001): moved onto engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT) and engine/CQuestManager.h (FAILQUEST,
//   FINISHSCREEN). Was the free __fastcall stand-in CGameScriptInterface_SetQuestAsFailed with int / char parameters calling
//   CQuestMgr::doFailQuest / CQuestMgr::notify (stand-ins).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_FAILQUEST
#define FABLE_RETAIL_CQUESTMANAGER_FINISHSCREEN
#include "engine/CGameScriptInterface.h"
#include "engine/CQuestManager.h"
extern char g_someFlag;
struct CGameScriptInterface_Methods : CGameScriptInterface { void applyFloat(float v); };

void CGameScriptInterface::SetQuestAsFailed(const CCharString& quest_name, bool show_screen, const CWideString& reason, bool silent) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().SetQuestAsFailed(quest_name, show_screen, reason, silent);
    if (!show_screen && g_someFlag)
        CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().OnQuestFinishScreenDisplayed(quest_name);
    ((CGameScriptInterface_Methods*)this)->applyFloat(1.0f);
}
