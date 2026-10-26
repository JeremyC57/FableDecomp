#include <cstdio>

static int g_notifyCount = 0;
static int g_reloadCount = 0;
static float g_lastProgress = 0.0f;
static int g_lastNotifyA0 = 0;

struct CQuestMgr {
    void Notify(int questName, char showScreen, int updateLogbook, int showQuestCompleteScreen);
    void Reload(int questName);
};

void CQuestMgr::Notify(int questName, char showScreen, int updateLogbook, int showQuestCompleteScreen) {
    (void)showScreen; (void)updateLogbook; (void)showQuestCompleteScreen;
    g_notifyCount++;
    g_lastNotifyA0 = questName;
}
void CQuestMgr::Reload(int questName) {
    (void)questName;
    g_reloadCount++;
}

CQuestMgr g_mgrObj;
CQuestMgr* g_questMgr = &g_mgrObj;
char g_someFlag = 1;

class CGameScriptInterface { public:
    void SetQuestProgress(float v);
};
void CGameScriptInterface::SetQuestProgress(float v) {
    g_lastProgress = v;
}

void __fastcall CGameScriptInterface_SetQuestAsCompleted(CGameScriptInterface* self, int edx_ignore, int questName, char showScreen, int updateLogbook, int showQuestCompleteScreen)
{
    g_questMgr->Notify(questName, showScreen, updateLogbook, showQuestCompleteScreen);
    if (!showScreen && g_someFlag) {
        g_questMgr->Reload(questName);
    }
    self->SetQuestProgress(1.0f);
}

int main() {
    CGameScriptInterface obj;

    // Case 1: a1 != 0 -> Notify only, no Reload
    g_notifyCount = g_reloadCount = 0; g_lastProgress = 0.0f;
    CGameScriptInterface_SetQuestAsCompleted(&obj, 0, 111, 1, 222, 333);
    if (g_notifyCount != 1) { std::printf("FAIL notify1\n"); return 1; }
    if (g_lastNotifyA0 != 111) { std::printf("FAIL a0_1\n"); return 1; }
    if (g_reloadCount != 0) { std::printf("FAIL reload1\n"); return 1; }
    if (g_lastProgress != 1.0f) { std::printf("FAIL prog1\n"); return 1; }

    // Case 2: a1 == 0, flag set -> Notify + Reload
    g_notifyCount = g_reloadCount = 0; g_lastProgress = 0.0f;
    CGameScriptInterface_SetQuestAsCompleted(&obj, 0, 444, 0, 555, 666);
    if (g_notifyCount != 1) { std::printf("FAIL notify2\n"); return 1; }
    if (g_reloadCount != 1) { std::printf("FAIL reload2\n"); return 1; }
    if (g_lastNotifyA0 != 444) { std::printf("FAIL a0_2\n"); return 1; }
    if (g_lastProgress != 1.0f) { std::printf("FAIL prog2\n"); return 1; }

    // Case 3: a1 == 0, flag clear -> Notify only, no Reload
    g_someFlag = 0;
    g_notifyCount = g_reloadCount = 0; g_lastProgress = 0.0f;
    CGameScriptInterface_SetQuestAsCompleted(&obj, 0, 777, 0, 0, 0);
    if (g_reloadCount != 0) { std::printf("FAIL reload3\n"); return 1; }
    if (g_lastProgress != 1.0f) { std::printf("FAIL prog3\n"); return 1; }

    std::printf("CGameScriptInterface_00892f80_TEST PASS\n");
    return 0;
}