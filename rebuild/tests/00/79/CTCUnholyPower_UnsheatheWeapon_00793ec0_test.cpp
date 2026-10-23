// Behaviour test for CTCUnholyPower::UnsheatheWeapon (0x0078E9C0): a dead or missing removed item does nothing; otherwise
// the primary slot is cleared (weapon dropped, 0x1a reaction when no longer carried), the item goes back into its def's
// carry slot, the weapon sheathes refresh, the flourish is cancelled, the mode restarts and the player turns aggressive.
#include <cstdio>
#include <cstring>

struct CThing {
    bool dead, isWeapon, has1a, hasWeapons, hasCombat;
    long carrySlot;
    int notCarriedCalls;
};
struct Log { int drops, removes, reacts, clearPrimary, put, refresh, flourishOff, restart, aggressive; long putSlot; CThing* putThing; };
static Log g;
struct CTCCarrying {
    CThing* primary;
    bool pending;
    CThing* carried;
    bool HasPendingActions() { return pending; }
    CThing* GetThingInPrimarySlot() { return primary; }
    void RemoveThing(CThing* t) { ++g.removes; if (carried == t) carried = 0; if (primary == t) primary = 0; }
    bool IsCarryingThing(CThing* t) { return carried == t || primary == t; }
    CThing* RemoveThingFromPrimarySlot(bool) { ++g.clearPrimary; CThing* p = primary; primary = 0; return p; }
    void PutThingInCarrySlot(CThing* t, long slot, bool) { ++g.put; g.putSlot = slot; g.putThing = t; carried = t; }
};
static void DropWeapon(CThing* weapon, CTCCarrying* carrying, CThing*) { ++g.drops; carrying->primary = 0; (void)weapon; }

struct CTCUnholyPower {
    CThing* parent;
    CThing* itemRemoved;
    CTCCarrying* carrying;
    void UnsheatheWeapon()
    {
        if (!itemRemoved || itemRemoved->dead)
            return;
        CTCCarrying* c = carrying;
        if (!c->HasPendingActions()) {
            CThing* held = c->GetThingInPrimarySlot();
            if (held->isWeapon) DropWeapon(held, c, parent);
            else c->RemoveThing(held);
            if (held->has1a) {
                if (!c->IsCarryingThing(held)) { ++g.reacts; ++held->notCarriedCalls; }
            }
            if (!c->HasPendingActions())
                c->RemoveThingFromPrimarySlot(false);
        }
        if (c->IsCarryingThing(itemRemoved))
            c->RemoveThing(itemRemoved);
        c->PutThingInCarrySlot(itemRemoved, itemRemoved->carrySlot, true);
        if (parent->hasWeapons) ++g.refresh;
        if (parent->hasCombat) ++g.flourishOff;
        ++g.restart;
        ++g.aggressive;
    }
};

int main()
{
    CThing hero; std::memset(&hero, 0, sizeof(hero)); hero.hasWeapons = true; hero.hasCombat = true;
    CThing sword; std::memset(&sword, 0, sizeof(sword)); sword.isWeapon = true; sword.has1a = true;
    CThing item; std::memset(&item, 0, sizeof(item)); item.carrySlot = 3;
    CTCCarrying carrying; carrying.primary = &sword; carrying.pending = false; carrying.carried = 0;
    CTCUnholyPower tc; tc.parent = &hero; tc.itemRemoved = &item; tc.carrying = &carrying;
    std::memset(&g, 0, sizeof(g));
    item.dead = true;
    tc.UnsheatheWeapon();
    if (g.put != 0) { std::printf("00793ec0_TEST FAIL\n"); return 1; }
    item.dead = false;
    tc.UnsheatheWeapon();
    if (g.drops != 1 || g.reacts != 1 || g.clearPrimary != 1 || g.put != 1 || g.putSlot != 3 || g.putThing != &item
        || g.refresh != 1 || g.flourishOff != 1 || g.restart != 1 || g.aggressive != 1) { std::printf("00793ec0_TEST FAIL\n"); return 1; }
    std::printf("00793ec0_TEST PASS\n");
    return 0;
}