// Behaviour test for 0x00E7A5C0 (CV_ExposeMayorScript::PhaseTwo): persistent villager, the quest card once.
#include <cstdio>
#include <cstring>
#include <string>
// Host model: logs 'P' villager made persistent, 'C' card given (Expose Mayor card, the active quest, not added),
// 'O' first objective set (active quest, objective 01, BarrowFields, ""); terminating is a countdown.
struct Game {
    std::string log;
    std::string GetActiveQuestName() { return "Q_ExposeMayor"; }
    std::string GetThingWithScriptName(const char* n) { return n; }
    void SetThingPersistent(const std::string& t, bool p) { log += (t == "EM_SuspiciousVillager" && p) ? "P" : "?"; }
    void GiveHeroQuestCardDirectly(const std::string& card, const std::string& quest, bool add)
    { log += (card == "OBJECT_QUEST_CARD_EXPOSE_MAYOR" && quest == "Q_ExposeMayor" && !add) ? "C" : "?"; }
    void SetQuestCardObjective(const std::string& q, const std::string& t, const std::string& r1, const std::string& r2)
    { log += (q == "Q_ExposeMayor" && t == "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_01" && r1 == "BarrowFields" && r2.empty()) ? "O" : "?"; }
};
struct Script {
    Game* GSI; bool QuestCardGiven; int termAfter; int checks;
    Script(Game* g, bool given, int after) : GSI(g), QuestCardGiven(given), termAfter(after), checks(0) {}
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void PhaseTwo()
    {
        std::string villager = GSI->GetThingWithScriptName("EM_SuspiciousVillager");
        GSI->SetThingPersistent(villager, true);
        if (!QuestCardGiven) {
            if (IsActiveThreadTerminating()) return;
            GSI->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_EXPOSE_MAYOR", GSI->GetActiveQuestName(), false);
            GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_01", "BarrowFields", "");
            QuestCardGiven = true;
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Script s(&g, false, 5); s.PhaseTwo(); CHECK(g.log == "PCO" && s.QuestCardGiven); s.PhaseTwo(); CHECK(g.log == "PCOP"); }
    { Game g; Script s(&g, false, 0); s.PhaseTwo(); CHECK(g.log == "P" && !s.QuestCardGiven); }
    { Game g; Script s(&g, true, 0); s.PhaseTwo(); CHECK(g.log == "P" && s.checks == 0); }
    if (g_fail) { std::printf("00e7a5c0_TEST FAIL\n"); return 1; }
    std::printf("00e7a5c0_TEST PASS\n");
    return 0;
}
