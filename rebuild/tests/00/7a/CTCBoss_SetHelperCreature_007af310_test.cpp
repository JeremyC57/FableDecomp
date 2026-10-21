// Behaviour test for CTCBoss::SetHelperCreature (0x007AF310): keep, reuse a dead slot, or append.
#include <cstdio>
struct Thing { bool dead; };
struct Boss {
    Thing* slots[8]; unsigned n;
    static bool Alive(Thing* t) { return t && !t->dead; }
    void SetHelperCreature(Thing* creature)
    {
        unsigned i;
        for (i = 0; i < n; ++i) if (Alive(slots[i]) && creature == slots[i]) return;
        for (i = 0; i < n; ++i) if (!Alive(slots[i])) { slots[i] = creature; return; }
        slots[n++] = creature;
    }
};
int main()
{
    Thing a = { false }, b = { false }, c = { false }, dead = { true };
    Boss boss = { { &a, &dead, 0 }, 2 };
    boss.SetHelperCreature(&a);                     // already held
    if (boss.n != 2 || boss.slots[0] != &a) { std::printf("007af310_TEST FAIL\n"); return 1; }
    boss.SetHelperCreature(&b);                     // reuses the dead slot
    if (boss.n != 2 || boss.slots[1] != &b) { std::printf("007af310_TEST FAIL\n"); return 1; }
    boss.SetHelperCreature(&c);                     // appended
    if (boss.n != 3 || boss.slots[2] != &c) { std::printf("007af310_TEST FAIL\n"); return 1; }
    std::printf("007af310_TEST PASS\n");
    return 0;
}