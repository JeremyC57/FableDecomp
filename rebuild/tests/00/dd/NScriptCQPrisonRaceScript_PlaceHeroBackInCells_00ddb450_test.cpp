// Behaviour test for CQ_PrisonRaceScript::PlaceHeroBackInCells() @ 0x00DDB450
#include <cstdio>
#include <string.h>

class CCharString {
public:
    char m_text[32];
    CCharString(const char* text, long length = -1) { (void)length; strncpy(m_text, text, 31); m_text[31] = 0; }
};

struct CGameScriptInterface_Calls {
    char m_quest[32];
    unsigned long m_delay;
    int m_calls;
    virtual void DeactivateQuestLater(const CCharString& quest, unsigned long delay)
    {
        strcpy(m_quest, quest.m_text); m_delay = delay; ++m_calls;
    }
};

struct CQ_PrisonMasterData { long m_counter; };

struct CQ_PrisonRaceScript {
    CGameScriptInterface_Calls* Game;
    CQ_PrisonMasterData*        MasterData;
    void PlaceHeroBackInCells()
    {
        ++MasterData->m_counter;
        Game->DeactivateQuestLater("Q_PrisonRace", 1);
    }
};

int main()
{
    CGameScriptInterface_Calls game; game.m_calls = 0; game.m_quest[0] = 0; game.m_delay = 0;
    CQ_PrisonMasterData master; master.m_counter = 4;
    CQ_PrisonRaceScript s; s.Game = &game; s.MasterData = &master;
    s.PlaceHeroBackInCells();
    if (master.m_counter != 5 || game.m_calls != 1 || strcmp(game.m_quest, "Q_PrisonRace") != 0 || game.m_delay != 1) { std::printf("00ddb450_TEST FAIL\n"); return 1; }
    std::printf("00ddb450_TEST PASS\n");
    return 0;
}