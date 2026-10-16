#include <cstdio>
// Behaviour model of CTCQuestCard::CTCQuestCard (0x007026A0): flags false, FeatIndex -1, def looked up, rewards copied.
struct Def { long renown; long gold; };
struct Card {
    const Def* def; bool finished, replayable; long feat; long gold, renown;
    Card(const Def* d) : def(0), finished(false), replayable(false), feat(-1)
    { def = d; gold = def->gold; renown = def->renown; }
};
int main()
{
    Def d = { 7, 300 };
    Card c(&d);
    if (c.finished || c.replayable || c.feat != -1 || c.gold != 300 || c.renown != 7) { std::printf("007026a0_TEST FAIL\n"); return 1; }
    std::printf("007026a0_TEST PASS\n");
    return 0;
}
