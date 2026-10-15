// Behaviour test for CQ_WizardBattleScript::Init() @ 0x00E1A870
#include <cstdio>
#include <string.h>

class CCharString {
public:
    char m_text[32];
    CCharString(const char* text, long length = -1) { (void)length; strncpy(m_text, text, 31); m_text[31] = 0; }
};

struct CGameScriptInterface_Calls {
    char m_quest[32];
    char m_region[32];
    int m_calls;
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region)
    {
        strcpy(m_quest, quest.m_text); strcpy(m_region, region.m_text); ++m_calls;
    }
};

struct CQ_WizardBattleScript {
    CGameScriptInterface_Calls* Game;
    long MazeConversation;
    void Init()
    {
        Game->AddQuestRegion("Q_WizardBattle", "HookCoast");
        MazeConversation = 0;
    }
};

int main()
{
    CGameScriptInterface_Calls game; game.m_calls = 0;
    CQ_WizardBattleScript s; s.Game = &game; s.MazeConversation = 99;
    s.Init();
    if (game.m_calls != 1 || strcmp(game.m_quest, "Q_WizardBattle") != 0 || strcmp(game.m_region, "HookCoast") != 0 || s.MazeConversation != 0) { std::printf("00e1a870_TEST FAIL\n"); return 1; }
    std::printf("00e1a870_TEST PASS\n");
    return 0;
}