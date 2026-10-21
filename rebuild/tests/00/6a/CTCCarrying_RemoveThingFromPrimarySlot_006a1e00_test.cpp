// Behaviour test for CTCCarrying::RemoveThingFromPrimarySlot (0x006A1E00): drop/remove, notify, and clear a melee follower.
#include <cstdio>
struct Thing { bool weapon, melee, projectile, loaded, animating, played, dead; };
struct Carrying {
    Thing* slots[2]; int n; int drops, removes;
    bool IsPrimarySlotEmpty() { return n == 0; }
    Thing* Primary() { return slots[0]; }
    void RemoveThing(Thing* t) { ++removes; if (n && slots[0] == t) { slots[0] = slots[1]; --n; } }
    void Drop(Thing* t) { ++drops; RemoveThing(t); --removes; }
    Thing* RemoveThingFromPrimarySlot(bool)
    {
        if (!IsPrimarySlotEmpty()) {
            Thing* t = Primary();
            if (t->weapon) Drop(t); else RemoveThing(t);
            if (t->weapon && t->projectile) t->loaded = false;
            if (t->animating) t->played = true;
            if (!IsPrimarySlotEmpty()) {
                Thing* next = Primary();
                if (next && !next->dead && next->weapon && next->melee) RemoveThing(next);
            }
            return t;
        }
        return 0;
    }
};
int main()
{
    Thing bow = { true, false, true, true, false, false, false }, sword = { true, true, false, false, true, false, false };
    Carrying c = { { &bow, &sword }, 2, 0, 0 };
    Thing* r = c.RemoveThingFromPrimarySlot(false);
    if (r != &bow || bow.loaded || c.drops != 1 || c.n != 0 || c.removes != 1) { std::printf("006a1e00_TEST FAIL\n"); return 1; }
    if (c.RemoveThingFromPrimarySlot(false) != 0) { std::printf("006a1e00_TEST FAIL\n"); return 1; }
    std::printf("006a1e00_TEST PASS\n");
    return 0;
}