// Behaviour test for NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main (0x00D647F0): three rounds (melee, bow,
// lightning) that share ONE quest-info counter variable (re-created each round). In each round a hit with the round's weapon counts
// (the seventh ends the round and heals him); a hit with another weapon heals him and, when no conversation is running, starts a
// scolding conversation whose id becomes the current conversation. In the bow round a lightning hit always heals, with the line only
// when no conversation is active; a melee hit there heals only together with the line. A log model of those rules.
#include <cstdio>
#include <cstring>
enum { NONE, MELEE, BOW, LIGHTNING };
struct Maze {
    long counter, conversation, nextConversation, nextCounter;
    int beenHit, scolds, heals;
    bool active;
    long NewConversation() { active = true; return ++nextConversation; }
    void Scold() { long line = NewConversation(); ++scolds; conversation = line; }
    // one frame of the bow round (round 2)
    void BowFrame(int hit)
    {
        if (hit == MELEE && !active) { Scold(); ++heals; }
        else if (hit == BOW) { if (++beenHit == 7) ++heals; }
        else if (hit == LIGHTNING) {
            if (!active) Scold();
            ++heals;
        }
    }
    int Round(int round, const int* hits, int n)
    {
        counter = ++nextCounter;            // the same variable, a new HUD element per round
        beenHit = 0;
        for (int i = 0; i < n && beenHit < 7; ++i) {
            if (round == 2) { BowFrame(hits[i]); continue; }
            int good = round == 1 ? MELEE : LIGHTNING;
            if (hits[i] == good) { if (++beenHit == 7) ++heals; }
            else if (hits[i] != NONE && !active) { Scold(); ++heals; }
        }
        return beenHit;
    }
};
int main()
{
    Maze m; std::memset(&m, 0, sizeof m);
    int h1[] = { MELEE, BOW, MELEE, MELEE, LIGHTNING, MELEE, MELEE, MELEE, MELEE, BOW };
    bool ok = m.Round(1, h1, 10) == 7 && m.counter == 1 && m.scolds == 1 && m.heals == 2 && m.conversation == 1;
    m.active = false;
    int h2[] = { LIGHTNING, LIGHTNING, MELEE, BOW, BOW };
    ok = ok && m.Round(2, h2, 5) == 2 && m.counter == 2 && m.scolds == 2 && m.heals == 4 && m.conversation == 2;
    m.active = false;
    int h3[] = { MELEE, LIGHTNING, BOW };
    ok = ok && m.Round(3, h3, 3) == 1 && m.counter == 3 && m.scolds == 3 && m.heals == 5 && m.conversation == 3;
    std::printf(ok ? "00d647f0_TEST PASS\n" : "00d647f0_TEST FAIL\n");
    return ok ? 0 : 1;
}