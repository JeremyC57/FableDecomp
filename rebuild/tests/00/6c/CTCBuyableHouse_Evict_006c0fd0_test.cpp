// Behaviour test for CTCBuyableHouse::Evict (0x006C0FD0): unlink every villager whose home or work is the house.
#include <cstdio>
struct Thing { int id; };
struct Creature { Thing* home; Thing* work; bool persisted; bool shopkeeper; bool detachedShop; bool detachedVillage; };
struct House {
    Thing* self; bool hasVillage; Creature* residents[4]; int n; int bagsDestroyed;
    void Evict()
    {
        ++bagsDestroyed;
        if (!hasVillage) return;
        Creature* picked[4]; int m = 0;
        for (int i = 0; i < n; ++i)
            if (residents[i] && (residents[i]->home == self || residents[i]->work == self)) picked[m++] = residents[i];
        for (int i = 0; i < m; ++i) {
            Creature* c = picked[i];
            if (c->home == self) { c->home = 0; c->persisted = false; }
            if (c->work == self) { c->work = 0; c->persisted = false; }
            if (c->shopkeeper) c->detachedShop = true;
            c->detachedVillage = true;
        }
    }
};
int main()
{
    Thing h = { 1 }, other = { 2 };
    Creature a = { &h, &other, true, false, false, false }, b = { &other, &h, true, true, false, false }, c = { &other, &other, true, false, false, false };
    House house = { &h, true, { &a, &b, &c, 0 }, 4, 0 };
    house.Evict();
    if (a.home != 0 || a.work != &other || a.persisted || !a.detachedVillage || b.work != 0 || !b.detachedShop
        || c.detachedVillage || !c.persisted || house.bagsDestroyed != 1) { std::printf("006c0fd0_TEST FAIL\n"); return 1; }
    std::printf("006c0fd0_TEST PASS\n");
    return 0;
}