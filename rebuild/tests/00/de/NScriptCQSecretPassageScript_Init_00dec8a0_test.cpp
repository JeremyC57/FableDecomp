// Behaviour test for 0x00DEC8A0 NScript::CQ_SecretPassageScript::Init: the five Secret Passage regions are registered with
// AddQuestRegion under the quest name Q_SecretPassage, in retail's order.
#include <cstdio>
#include <cstring>

static char g_log[512];
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
class CGameScriptInterface { public:
    void AddQuestRegion(const CCharString& quest, const CCharString& region) const
    {
        std::strcat(g_log, quest.Text); std::strcat(g_log, ":"); std::strcat(g_log, region.Text); std::strcat(g_log, ";");
    }
};
struct CQ_SecretPassageScript {
    CGameScriptInterface* Game;
    void Init()
    {
        Game->AddQuestRegion("Q_SecretPassage", "GraveyardSecretPassage_1");
        Game->AddQuestRegion("Q_SecretPassage", "KrakenChamber");
        Game->AddQuestRegion("Q_SecretPassage", "PrisonSecretPassage");
        Game->AddQuestRegion("Q_SecretPassage", "PrisonSecretPassageExterior");
        Game->AddQuestRegion("Q_SecretPassage", "PrisonTortureChamber");
    }
};

int main()
{
    CGameScriptInterface game;
    CQ_SecretPassageScript s; s.Game = &game;
    s.Init();
    if (std::strcmp(g_log, "Q_SecretPassage:GraveyardSecretPassage_1;Q_SecretPassage:KrakenChamber;Q_SecretPassage:PrisonSecretPassage;"
                           "Q_SecretPassage:PrisonSecretPassageExterior;Q_SecretPassage:PrisonTortureChamber;")) {
        std::printf("00dec8a0_TEST FAIL\n"); return 1;
    }
    std::printf("00dec8a0_TEST PASS\n");
    return 0;
}