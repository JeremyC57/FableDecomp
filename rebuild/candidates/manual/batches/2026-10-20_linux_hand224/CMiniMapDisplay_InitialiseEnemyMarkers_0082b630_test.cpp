#include <cstdio>
#include <cstring>
struct Thing { long mapIndex; bool dead; int team; const char* def; bool underground; unsigned char flags; };
static int g_markers = 0;
static long Region(long m) { return m / 10; }
static bool AreThingsEnemies(const Thing* a, const Thing* b) { return a->team != b->team; }
static void Initialise(Thing* hero, Thing** things, int n, long current, bool loaded)
{
    if (!loaded) return;
    if (!hero || hero->dead) return;
    for (int i = 0; i < n; ++i) {
        Thing* t = things[i];
        if (current == Region(t->mapIndex) && AreThingsEnemies(hero, t) && std::strcmp(t->def, "CREATURE_BIRD_GUILD_SPARROW") != 0
            && !t->underground && !(t->flags & 0x80))
            ++g_markers;
    }
}
int main()
{
    Thing hero = { 11, false, 0, "HERO", false, 0 };
    Thing a = { 12, false, 1, "CREATURE_BANDIT", false, 0 }, b = { 13, false, 0, "CREATURE_GUARD", false, 0 },
          c = { 14, false, 1, "CREATURE_BIRD_GUILD_SPARROW", false, 0 }, d = { 15, false, 1, "CREATURE_HOBBE", true, 0 },
          e = { 16, false, 1, "CREATURE_WASP", false, 0x80 }, f = { 25, false, 1, "CREATURE_BANDIT", false, 0 };
    Thing* list[] = { &a, &b, &c, &d, &e, &f };
    Initialise(&hero, list, 6, 1, true);
    hero.dead = true; Initialise(&hero, list, 6, 1, true);
    if (g_markers != 1) { std::printf("0082b630_TEST FAIL\n"); return 1; }
    std::printf("0082b630_TEST PASS\n");
    return 0;
}
