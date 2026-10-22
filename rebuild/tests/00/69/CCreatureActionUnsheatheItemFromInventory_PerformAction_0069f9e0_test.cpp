// Behaviour test for CCreatureAction_UnsheatheItemFromInventory::PerformAction (0x0069F9E0): when the carrier is idle the
// held thing is dropped (weapon) or removed and the slot cleared; a live item is put into its def's carry slot with the
// sheathes refreshed; the main player goes aggressive.
#include <cstdio>

struct Thing { bool isWeapon; bool dead; long carrySlot; };
struct Carrying { bool busy; Thing* primary; Thing* slotThing; long slot; int removed; int dropped; };
static bool g_aggressive, g_mainPlayer = true;
static int g_refreshed;

static void PerformAction(Carrying* c, Thing* item)
{
    if (!c->busy) {
        Thing* held = c->primary;
        if (held->isWeapon) ++c->dropped; else ++c->removed;
        if (!c->busy) c->primary = 0;
    }
    if (item && !item->dead) {
        c->slotThing = item;
        c->slot = item->carrySlot;
        ++g_refreshed;
    }
    if (g_mainPlayer) g_aggressive = true;
}

int main()
{
    Thing sword = { true, false, 0 };
    Thing bow = { false, false, 3 };
    Carrying c = { false, &sword, 0, -1, 0, 0 };
    PerformAction(&c, &bow);
    if (c.dropped != 1 || c.primary != 0 || c.slotThing != &bow || c.slot != 3 || g_refreshed != 1 || !g_aggressive) {
        std::printf("0069f9e0_TEST FAIL\n"); return 1;
    }
    Thing dead = { false, true, 5 };
    c.busy = true; c.primary = &sword;
    PerformAction(&c, &dead);
    if (c.primary != &sword || c.slot != 3 || g_refreshed != 1) { std::printf("0069f9e0_TEST FAIL\n"); return 1; }
    std::printf("0069f9e0_TEST PASS\n");
    return 0;
}