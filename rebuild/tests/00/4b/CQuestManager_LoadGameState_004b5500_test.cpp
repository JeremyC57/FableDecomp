// Behaviour test for CQuestManager::LoadGameState(CStringParser&) (0x004B5500): a plain model of the section parser -- the quest
// card types until END_QUEST_CARDS (feat dummies carry an index, the rest a flag), active / completed quest cards by UID (zero
// ends the list, dead or missing things are skipped), active quests activated together only when there are any, and the
// creature-generation-disabled names until their end marker.
#include <cstdio>
#include <cstring>

struct Card { const char* def; const char* script; bool flag; long feat; };

static int ParseCards(const char* const* tokens, Card* out)
{
    int n = 0, i = 0;
    while (std::strcmp(tokens[i], "END_QUEST_CARDS") != 0) {
        if (std::strcmp(tokens[i], "OBJECT_FEAT_CARD_DUMMY") == 0) {
            Card c = { tokens[i + 1], tokens[i + 2], false, 0 };
            std::sscanf(tokens[i + 3], "%ld", &c.feat);
            out[n++] = c;
        } else {
            long v = 0;
            std::sscanf(tokens[i + 2], "%ld", &v);
            Card c = { tokens[i], tokens[i + 1], v > 0, -1 };
            out[n++] = c;
        }
        i += 4;
        if (std::strcmp(tokens[i - 4], "OBJECT_FEAT_CARD_DUMMY") != 0) i -= 1;
    }
    return n;
}

static int ParseUIDs(const unsigned long* uids, const bool* alive, int* kept)
{
    int n = 0;
    for (int i = 0; uids[i] > 0; ++i)
        if (alive[i]) kept[n++] = i;
    return n;
}

int main()
{
    const char* tokens[] = { "Q1", "S1", "1", "OBJECT_FEAT_CARD_DUMMY", "D2", "S2", "7", "Q3", "S3", "0", "END_QUEST_CARDS" };
    Card cards[4];
    int n = ParseCards(tokens, cards);
    if (n != 3 || !cards[0].flag || cards[0].feat != -1 || cards[1].flag || cards[1].feat != 7 || cards[2].flag
            || std::strcmp(cards[2].script, "S3") != 0) {
        std::printf("004b5500_TEST FAIL\n"); return 1;
    }
    unsigned long uids[] = { 5, 9, 11, 0, 4 };
    bool alive[] = { true, false, true, true, true };
    int kept[5];
    if (ParseUIDs(uids, alive, kept) != 2 || kept[0] != 0 || kept[1] != 2) { std::printf("004b5500_TEST FAIL\n"); return 1; }
    std::printf("004b5500_TEST PASS\n");
    return 0;
}