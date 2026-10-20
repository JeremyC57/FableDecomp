// Behaviour test for CTCInventoryQuests::DeactivateSelectedTestQuest 0x0061AC60 (model copy with stub quest manager/console)
#include <cstdio>
#include <cstring>

static int g_completed = 0, g_finishShown = 0, g_deactivated = 0, g_executed = 0;
static char g_lastPath[128];
struct CCharString {
    char text[64];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
    bool operator!=(const char* t) const { return std::strcmp(text, t) != 0; }
};
CCharString operator+(const char* left, const CCharString& right)
{
    CCharString r(left); std::strcat(r.text, right.text); return r;
}
static bool PathExists(const CCharString& path) { return std::strstr(path.text, "missing") == 0; }
struct CTestQuest { CCharString QuestName, IniFile, OutiFile; };
static CTestQuest g_quests[2];
struct CTestQuestVector { CTestQuest* _M_start; CTestQuest& operator[](unsigned long n) { return *(_M_start + n); } };
struct CQuestManager {
    bool SetQuestAsCompleted(const CCharString&, bool a, bool b, bool c) { if (!a && !b && !c) ++g_completed; return true; }
    bool OnQuestFinishScreenDisplayed(const CCharString&) { ++g_finishShown; return true; }
    void DeactivateQuest(const CCharString&, unsigned long reason) { if (reason == 0) ++g_deactivated; }
};
static CQuestManager g_qm; static CQuestManager* g_pQuestManager = &g_qm;
static bool g_bQuestFinishScreensEnabled = true;
struct CConsole {
    static CConsole* Get() { static CConsole c; return &c; }
    void ExecuteConfigFile(const CCharString& path) { ++g_executed; std::strcpy(g_lastPath, path.text); }
};
struct CTCInventoryQuests {
    bool ActiveTestQuestMode;
    long TestQuestSelectedIndex;
    void GetTestQuestsToDisplay(CTestQuestVector* quests) { quests->_M_start = g_quests; }
    void DeactivateSelectedTestQuest();
};

void CTCInventoryQuests::DeactivateSelectedTestQuest()
{
    if (!ActiveTestQuestMode)
        return;
    CTestQuestVector quests;
    GetTestQuestsToDisplay(&quests);
    CTestQuest& quest = quests[TestQuestSelectedIndex];
    g_pQuestManager->SetQuestAsCompleted(quest.QuestName, false, false, false);
    if (g_bQuestFinishScreensEnabled)
        g_pQuestManager->OnQuestFinishScreenDisplayed(quest.QuestName);
    g_pQuestManager->DeactivateQuest(quest.QuestName, 0);
    if (quest.OutiFile != "") {
        CCharString path = "Data\\Levels\\Ini\\" + quest.OutiFile;
        if (PathExists(path))
            CConsole::Get()->ExecuteConfigFile(path);
    }
}

int main()
{
    g_quests[1].QuestName = CCharString("Q_TEST"); g_quests[1].OutiFile = CCharString("q_test_out.ini");
    CTCInventoryQuests q; q.ActiveTestQuestMode = true; q.TestQuestSelectedIndex = 1;
    q.DeactivateSelectedTestQuest();
    if (g_completed != 1 || g_finishShown != 1 || g_deactivated != 1 || g_executed != 1 ||
        std::strcmp(g_lastPath, "Data\\Levels\\Ini\\q_test_out.ini") != 0) { std::printf("0061ac60_TEST FAIL\n"); return 1; }
    q.TestQuestSelectedIndex = 0; g_bQuestFinishScreensEnabled = false; q.DeactivateSelectedTestQuest();   // no out ini
    q.ActiveTestQuestMode = false; q.DeactivateSelectedTestQuest();
    if (g_completed != 2 || g_finishShown != 1 || g_deactivated != 2 || g_executed != 1) { std::printf("0061ac60_TEST FAIL\n"); return 1; }
    std::printf("0061ac60_TEST PASS\n");
    return 0;
}