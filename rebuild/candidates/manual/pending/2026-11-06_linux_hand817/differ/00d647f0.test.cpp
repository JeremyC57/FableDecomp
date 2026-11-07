// Behaviour test for NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main (0x00D647F0): three rounds (melee, bow,
// lightning); in each round a hit with the round's weapon counts (seven hits beat him), a hit with another weapon heals him and,
// when no conversation is running, plays the round's scolding line. A log model of one round.
#include <cstdio>
#include <cstring>
enum { MELEE, BOW, LIGHTNING };
static int Round(int good, const int* hits, int n, int* scolds, int* heals)
{
    int counter = 0; *scolds = 0; *heals = 0;
    for (int i = 0; i < n && counter < 7; ++i) {
        if (hits[i] == good) ++counter; else { ++*scolds; ++*heals; }
    }
    return counter;
}
int main()
{
    int h1[] = { MELEE, BOW, MELEE, MELEE, LIGHTNING, MELEE, MELEE, MELEE, MELEE, BOW };
    int s, hl;
    bool ok = Round(MELEE, h1, 10, &s, &hl) == 7 && s == 2 && hl == 2;
    int h2[] = { BOW, BOW, MELEE };
    ok = ok && Round(BOW, h2, 3, &s, &hl) == 2 && s == 1;
    std::printf(ok ? "00d647f0_TEST PASS\n" : "00d647f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
