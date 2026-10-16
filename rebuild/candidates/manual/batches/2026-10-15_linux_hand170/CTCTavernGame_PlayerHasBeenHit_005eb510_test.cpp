#include <cstdio>
// Behaviour model of CTCTavernGame::PlayerHasBeenHit (0x005EB510): the hero must exist, be alive and carry the entity-events
// component, and that component must hold a hit-by event.
struct Events { bool hit; };
struct Hero { bool dead; Events* events; };
static bool HasBeenHit(const Hero* h)
{
    if (h && !h->dead && h->events) { if (h->events->hit) return true; }
    return false;
}
int main()
{
    Events yes = { true }, no = { false };
    Hero a = { false, &yes }, b = { false, &no }, c = { true, &yes }, d = { false, 0 };
    if (!HasBeenHit(&a) || HasBeenHit(&b) || HasBeenHit(&c) || HasBeenHit(&d) || HasBeenHit(0)) { std::printf("005eb510_TEST FAIL\n"); return 1; }
    std::printf("005eb510_TEST PASS\n");
    return 0;
}
