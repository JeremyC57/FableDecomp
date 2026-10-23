// Behaviour test for CTonyTestScriptsScript::AddQuestsToGuild (0x00EFF1C0): seven quest cards, card first, quest second.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* p;
    CCharString(const char* t, long = -1) : p(t) {}
};
static const char* g_card[8];
static const char* g_quest[8];
static int g_n = 0;
struct CGameScriptInterface {
    void AddQuestCard(const CCharString& card, const CCharString& quest, bool a, bool b)
    {
        if (a || b) g_n = 100;
        if (g_n < 8) { g_card[g_n] = card.p; g_quest[g_n] = quest.p; }
        ++g_n;
    }
};
struct CTonyTestScriptsScript {
    CGameScriptInterface* GSI;
    void AddQuestsToGuild()
    {
        GSI->AddQuestCard("OBJECT_QUEST_CARD_ARENA", "Q_Arena", false, false);
        GSI->AddQuestCard("OBJECT_QUEST_CARD_WASP_MENACE", "Q_WaspBoss", false, false);
        GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_ProtectFarm", false, false);
        GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_ProtectFarmEvil", false, false);
        GSI->AddQuestCard("OBJECT_QUEST_CARD_GRANNY_SCRIPT", "Q_HobbeCave", false, false);
        GSI->AddQuestCard("OBJECT_QUEST_CARD_DARKWOOD_TRADER_ESCORT", "Q_TraderEscort", false, false);
        GSI->AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CONTEST", "Q_HobbeContest", false, false);
    }
};
int main()
{
    CGameScriptInterface gsi;
    CTonyTestScriptsScript s;
    s.GSI = &gsi;
    s.AddQuestsToGuild();
    if (g_n != 7 || std::strcmp(g_card[0], "OBJECT_QUEST_CARD_ARENA") != 0 || std::strcmp(g_quest[0], "Q_Arena") != 0
        || std::strcmp(g_quest[3], "Q_ProtectFarmEvil") != 0 || std::strcmp(g_card[6], "OBJECT_QUEST_CARD_HOBBE_CONTEST") != 0) {
        std::printf("00eff1c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00eff1c0_TEST PASS\n");
    return 0;
}
