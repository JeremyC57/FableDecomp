// Behaviour test for CGuildSealWarning::Main @ 0x00E4B010 (copy of the model with a recording stub interface).
#include <cstdio>
#include <cstring>
static int g_msgs, g_cards, g_objectives, g_removed, g_pauses;
static bool g_done;
class CScriptThing { public: int id; };
class CGameScriptInterface { public:
    void HeroReceiveMessageFromGuildMaster(const char* m, const char* e, bool a, bool b) const { if (!std::strcmp(m, "TEXT_QST_011_GUILDSEAL_MESSAGE") && !*e && a && b) ++g_msgs; }
    void Pause(float s) const { if (s == 2.0f) ++g_pauses; }
    const char* GetActiveQuestName() const { return "Q"; }
    void GiveHeroQuestCardDirectly(const char* c, const char* q, bool f) const { if (!std::strcmp(c, "OBJECT_QUEST_CARD_BANDIT_TOLL") && !std::strcmp(q, "Q") && !f) ++g_cards; }
    void SetQuestCardObjective(const char* q, const char* t, const char* r1, const char* r2) const
    { if (!std::strcmp(q, "Q") && !std::strcmp(t, "TEXT_QUEST_BANDIT_TOLL_OBJECTIVE_01") && !std::strcmp(r1, "GreatwoodBanditToll") && !*r2) ++g_objectives; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (t.id == 3 && !a && b) ++g_removed; }
};
struct CQ_BanditTollOwner { bool QuestCardGiven; };
struct CGuildSealWarning {
    CGameScriptInterface* Game; CScriptThing Me; CQ_BanditTollOwner* ParentClass;
    bool IsDone() { return g_done; }
    void Main()
    {
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_011_GUILDSEAL_MESSAGE", "", true, true);
        Game->Pause(2.0f);
        if (!ParentClass->QuestCardGiven) {
            if (IsDone()) return;
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BANDIT_TOLL", Game->GetActiveQuestName(), false);
            ParentClass->QuestCardGiven = true;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BANDIT_TOLL_OBJECTIVE_01", "GreatwoodBanditToll", "");
        }
        Game->RemoveThing(Me, false, true);
    }
};
int main()
{
    int bad = 0;
    CGameScriptInterface game; CQ_BanditTollOwner quest; quest.QuestCardGiven = false;
    CGuildSealWarning s; s.Game = &game; s.Me.id = 3; s.ParentClass = &quest;
    g_done = false; s.Main();
    if (g_msgs != 1 || g_pauses != 1 || g_cards != 1 || g_objectives != 1 || g_removed != 1 || !quest.QuestCardGiven) bad++;
    s.Main(); if (g_msgs != 2 || g_cards != 1 || g_removed != 2) bad++;
    quest.QuestCardGiven = false; g_done = true; s.Main(); if (g_msgs != 3 || g_cards != 1 || g_removed != 2 || quest.QuestCardGiven) bad++;
    if (bad) { std::printf("00e4b010_TEST FAIL\n"); return 1; }
    std::printf("00e4b010_TEST PASS\n");
    return 0;
}