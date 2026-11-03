#include <cstdio>
#include <vector>
// Model of CreateRelevantPersonAndUpdateStats: pick the sex whose weighted share is behind, create that creature,
// record it, and when not listing set it up (follow hero, add to village, sexuality from the village people def).
struct Creature { bool male, follows; float sexuality; };
static Creature g_pool[8]; static int g_used;
static Creature* Create(bool male) { Creature* c = &g_pool[g_used++]; c->male = male; c->follows = false; c->sexuality = -1.0f; return c; }
static Creature* CreateRelevant(long noMales, long noFemales, float maleWeight, float femaleWeight, bool listCreatures,
                                std::vector<Creature*>& males, std::vector<Creature*>& females, long& count, float prob)
{
    Creature* creature = 0;
    if (noMales * femaleWeight < noFemales * maleWeight) { creature = Create(true); if (listCreatures) males.push_back(creature); ++count; }
    else { creature = Create(false); if (listCreatures) females.push_back(creature); ++count; }
    if (creature) { creature->follows = true; if (!listCreatures) creature->sexuality = prob; }
    return creature;
}
int main()
{
    std::vector<Creature*> m, f; long count = 0;
    Creature* a = CreateRelevant(0, 2, 1.0f, 1.0f, true, m, f, count, 0.1f);
    Creature* b = CreateRelevant(3, 1, 1.0f, 1.0f, false, m, f, count, 0.25f);
    if (!a || !a->male || m.size() != 1 || f.size() != 0 || a->sexuality != -1.0f) { std::printf("004ff750_TEST FAIL\n"); return 1; }
    if (!b || b->male || !b->follows || b->sexuality != 0.25f || count != 2) { std::printf("004ff750_TEST FAIL\n"); return 1; }
    std::printf("004ff750_TEST PASS\n"); return 0;
}