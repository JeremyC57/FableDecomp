#include "engine/CGameScriptInterface.h"
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).

struct CQuestMgr { void Notify(int questName, char showScreen, int updateLogbook, int showQuestCompleteScreen); void Reload(int questName); };
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())
extern char g_someFlag;
struct CGameScriptInterface_Methods : CGameScriptInterface { void SetQuestProgress(float v); };

void __fastcall CGameScriptInterface_SetQuestAsCompleted(CGameScriptInterface* self, int, int questName, char showScreen, int updateLogbook, int showQuestCompleteScreen)
{
    reinterpret_cast<CQuestMgr*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->Notify(questName, showScreen, updateLogbook, showQuestCompleteScreen);
    if (!showScreen && g_someFlag) reinterpret_cast<CQuestMgr*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->Reload(questName);
    ((CGameScriptInterface_Methods*)self)->SetQuestProgress(1.0f);
}
