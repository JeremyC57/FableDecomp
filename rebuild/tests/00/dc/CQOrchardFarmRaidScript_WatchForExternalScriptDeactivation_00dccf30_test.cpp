// Behaviour test for CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation (0x00DCCF30): waits frame by frame
// while either farm-raid quest is active, then adds both quest cards and deactivates the parent quest.
#include <cstdio>
#include <cstring>
struct CCharString {
    char Text[48];
    CCharString(const char* text) { std::strncpy(Text, text, 47); Text[47] = 0; }
};
struct CGameScriptInterface {
    int frames, goodUntil, evilUntil, cards, deactivated; char log[256];
    void NewScriptFrame() { ++frames; }
    bool IsQuestActive(const CCharString& q) {
        if (std::strcmp(q.Text, "Q_OrchardFarmRaidGood") == 0) return frames < goodUntil;
        if (std::strcmp(q.Text, "Q_OrchardFarmRaidEvil") == 0) return frames < evilUntil;
        return false;
    }
    void AddQuestCard(const CCharString& card, const CCharString& quest, bool f1, bool f2) {
        if (!f1 && !f2) { ++cards; std::strcat(log, card.Text); std::strcat(log, "/"); std::strcat(log, quest.Text); std::strcat(log, ";"); }
    }
    void DeactivateQuestLater(const CCharString& quest, unsigned long delay) {
        if (delay == 0 && std::strcmp(quest.Text, "Q_OrchardFarmRaid") == 0) ++deactivated;
    }
};
struct CScript {
    CGameScriptInterface* Game; int terminateAt;
    bool IsActiveThreadTerminating() { return Game->frames >= terminateAt; }
    void WatchForExternalScriptDeactivation();
};
void CScript::WatchForExternalScriptDeactivation()
{
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        bool deactivated = !(Game->IsQuestActive("Q_OrchardFarmRaidGood") || Game->IsQuestActive("Q_OrchardFarmRaidEvil"));
        if (deactivated)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false);
    Game->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false);
    Game->DeactivateQuestLater("Q_OrchardFarmRaid", 0);
}
int main()
{
    CGameScriptInterface g; std::memset(&g, 0, sizeof(g)); g.goodUntil = 3; g.evilUntil = 5;
    CScript s; s.Game = &g; s.terminateAt = 100;
    s.WatchForExternalScriptDeactivation();
    bool ok = g.frames == 5 && g.cards == 2 && g.deactivated == 1 && std::strcmp(g.log,
        "OBJECT_QUEST_CARD_PROTECT_FARM/Q_OrchardFarmRaidGood;OBJECT_QUEST_CARD_PROTECT_FARM_EVIL/Q_OrchardFarmRaidEvil;") == 0;
    CGameScriptInterface h; std::memset(&h, 0, sizeof(h)); h.goodUntil = 10; h.evilUntil = 0;
    CScript t; t.Game = &h; t.terminateAt = 4;
    t.WatchForExternalScriptDeactivation();
    ok = ok && h.frames == 4 && h.cards == 0 && h.deactivated == 0;
    if (!ok) { std::printf("00dccf30_TEST FAIL\n"); return 1; }
    std::printf("00dccf30_TEST PASS\n");
    return 0;
}