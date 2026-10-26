// Behaviour test for CTCInventoryQuests::ActivateSelectedTestQuest 0x0061AB30 (model copy with stub quest manager/console/world)
#include <cstdio>
#include <cstring>

static int g_deactivated = 0, g_cleared = 0, g_hsp = 0, g_card = 0, g_plain = 0, g_executed = 0;
class CCharString { public:
    char text[64];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
    bool operator!=(const char* t) const { return std::strcmp(text, t) != 0; }
};
CCharString operator+(const char* left, const CCharString& right) { CCharString r(left); std::strcat(r.text, right.text); return r; }
static bool PathExists(const CCharString&) { return true; }
class CTestQuest { public: CCharString QuestName, HSPName, IniFile, QuestCardDefName; };
static CTestQuest g_quests[2];
struct CTestQuestVector { CTestQuest* _M_start; CTestQuest& operator[](unsigned long n) { return *(_M_start + n); } };
class CQuestManager { public:
    void DeactivateQuest(const CCharString&, unsigned long reason) { if (reason == 0) ++g_deactivated; }
    void ClearQuestPersistScript(const CCharString&) { ++g_cleared; }
    bool ActivateTestQuestCard(const CCharString&, const CCharString&) { ++g_card; return true; }
    void ActivateQuest(const CCharString&, bool a, bool b) { if (a && b) ++g_plain; }
};
static CQuestManager g_qm; static CQuestManager* g_pQuestManager = &g_qm;
class CConsole { public:
    static CConsole* Get() { static CConsole c; return &c; }
    void ExecuteConfigFile(const CCharString& path) { if (std::strcmp(path.text, "Data\\Levels\\Ini\\a.ini") == 0) ++g_executed; }
};
class CWorld { public: void LoadQuestHSP(const CCharString& hsp) { if (hsp != "") ++g_hsp; } };
static CWorld g_world;
class CTCInventoryQuests { public:
    bool ActiveTestQuestMode;
    long TestQuestSelectedIndex;
    CWorld* GetWorld() { return &g_world; }
    void GetTestQuestsToDisplay(CTestQuestVector* quests) { quests->_M_start = g_quests; }
    void ActivateSelectedTestQuest();
};

void CTCInventoryQuests::ActivateSelectedTestQuest()
{
    if (!ActiveTestQuestMode)
        return;
    CTestQuestVector quests;
    GetTestQuestsToDisplay(&quests);
    CTestQuest& quest = quests[TestQuestSelectedIndex];
    if (quest.IniFile != "") {
        CCharString path = "Data\\Levels\\Ini\\" + quest.IniFile;
        if (PathExists(path))
            CConsole::Get()->ExecuteConfigFile(path);
    }
    g_pQuestManager->DeactivateQuest(quest.QuestName, 0);
    g_pQuestManager->ClearQuestPersistScript(quest.QuestName);
    GetWorld()->LoadQuestHSP(quest.HSPName);
    if (quest.QuestCardDefName != "")
        g_pQuestManager->ActivateTestQuestCard(quest.QuestName, quest.QuestCardDefName);
    else
        g_pQuestManager->ActivateQuest(quest.QuestName, true, true);
}

int main()
{
    g_quests[0].QuestName = CCharString("Q0"); g_quests[0].HSPName = CCharString("h0"); g_quests[0].IniFile = CCharString("a.ini");
    g_quests[1].QuestName = CCharString("Q1"); g_quests[1].HSPName = CCharString("h1"); g_quests[1].QuestCardDefName = CCharString("CARD");
    CTCInventoryQuests q; q.ActiveTestQuestMode = true; q.TestQuestSelectedIndex = 0;
    q.ActivateSelectedTestQuest();
    q.TestQuestSelectedIndex = 1; q.ActivateSelectedTestQuest();
    q.ActiveTestQuestMode = false; q.ActivateSelectedTestQuest();
    if (g_deactivated != 2 || g_cleared != 2 || g_hsp != 2 || g_card != 1 || g_plain != 1 || g_executed != 1) { std::printf("0061ab30_TEST FAIL\n"); return 1; }
    std::printf("0061ab30_TEST PASS\n");
    return 0;
}