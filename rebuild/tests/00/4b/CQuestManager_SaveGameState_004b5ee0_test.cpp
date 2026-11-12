// Behaviour test for 0x004B5EE0 CQuestManager::SaveGameState: a standalone model of the text it writes (quest-card lines,
// persisted active cards, persistent active quests, completed cards, the two name lists and the pending-screen count).
#include <cstdio>
#include <string>
#include <vector>

struct Card { std::string def, script; bool flag; long feat; };
struct Quest { std::string name; bool persistent; };
struct Thing { unsigned long uid; bool dead, persisted; };

static std::string Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }

static std::string Save(const std::vector<Card>& cards, const std::vector<Thing>& active, const std::vector<Quest>& quests,
                        const std::vector<Thing>& completed, const std::vector<std::string>& cg, long pending)
{
    std::string t = "\r\nSTART_QUEST_CARDS\r\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        if (cards[i].feat > -1)
            t += "OBJECT_FEAT_CARD_DUMMY, \"" + cards[i].def + "\", \"" + cards[i].script + "\", " + Num(cards[i].feat) + "\r\n";
        else
            t += cards[i].def + ", " + cards[i].script + ", " + Num(cards[i].flag ? 1 : 0) + "\r\n";
    }
    t += "END_QUEST_CARDS\r\n\r\nSTART_ACTIVE_QUEST_CARDS\r\n";
    for (size_t i = 0; i < active.size(); ++i)
        if (!active[i].dead && active[i].persisted) t += Num(active[i].uid) + "\r\n";
    t += "END_ACTIVE_QUEST_CARDS\r\n\r\nSTART_ACTIVE_QUESTS\r\n";
    for (size_t i = 0; i < quests.size(); ++i)
        if (quests[i].persistent) t += quests[i].name + "\r\n";
    t += "END_ACTIVE_QUESTS\r\n\r\nSTART_COMPLETED_QUESTS\r\n";
    for (size_t i = 0; i < completed.size(); ++i)
        if (!completed[i].dead) t += Num(completed[i].uid) + "\r\n";
    t += "END_COMPLETED_QUESTS\r\n\r\nSTART_CG_DISABLED_QUESTS\r\n";
    for (size_t i = 0; i < cg.size(); ++i) t += cg[i] + "\r\n";
    t += "END_CG_DISABLED_QUESTS\r\n\r\n";
    t += "NumberOfQuestsBetweenStartAndFinishScreens " + Num(pending) + "\r\n";
    return t;
}

int main()
{
    std::vector<Card> cards;
    Card a = { "OBJECT_CARD", "Q_A", true, -1 }; Card b = { "OBJECT_FEAT", "Q_B", false, 4 };
    cards.push_back(a); cards.push_back(b);
    std::vector<Thing> active; Thing t1 = { 7, false, true }; Thing t2 = { 8, false, false }; active.push_back(t1); active.push_back(t2);
    std::vector<Quest> quests; Quest q1 = { "Q_A", true }; Quest q2 = { "Q_C", false }; quests.push_back(q1); quests.push_back(q2);
    std::vector<Thing> completed; Thing t3 = { 9, true, true }; completed.push_back(t3);
    std::vector<std::string> cg(1, "Q_D");
    std::string s = Save(cards, active, quests, completed, cg, 2);
    if (s.find("OBJECT_CARD, Q_A, 1\r\n") == std::string::npos || s.find("OBJECT_FEAT_CARD_DUMMY, \"OBJECT_FEAT\", \"Q_B\", 4\r\n") == std::string::npos ||
        s.find("START_ACTIVE_QUEST_CARDS\r\n7\r\nEND") == std::string::npos || s.find("Q_C") != std::string::npos ||
        s.find("START_COMPLETED_QUESTS\r\nEND") == std::string::npos || s.find("Screens 2\r\n") == std::string::npos) {
        std::printf("004b5ee0_TEST FAIL\n"); return 1;
    }
    std::printf("004b5ee0_TEST PASS\n");
    return 0;
}