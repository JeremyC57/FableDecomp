#include "engine/CGameScriptInterface.h"

struct CQuestMgr { void Notify(int questName, char showScreen, int updateLogbook, int showQuestCompleteScreen); void Reload(int questName); };
extern CQuestMgr* g_questMgr;
extern char g_someFlag;
struct CGameScriptInterface_Methods : CGameScriptInterface { void SetQuestProgress(float v); };

void __fastcall CGameScriptInterface_SetQuestAsCompleted(CGameScriptInterface* self, int, int questName, char showScreen, int updateLogbook, int showQuestCompleteScreen)
{
    g_questMgr->Notify(questName, showScreen, updateLogbook, showQuestCompleteScreen);
    if (!showScreen && g_someFlag) g_questMgr->Reload(questName);
    ((CGameScriptInterface_Methods*)self)->SetQuestProgress(1.0f);
}
