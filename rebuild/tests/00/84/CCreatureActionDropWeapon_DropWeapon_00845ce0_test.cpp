// Behaviour test for CCreatureAction_DropWeapon::DropWeapon (0x00845CE0): an inventory-item weapon is removed from the
// carrying component and, unless its weapon def names a sheathe object or it is dead, put back in its passive carry slot
// (a living occupant of that slot is removed and sent to limbo when it is an inventory item, killed otherwise) and
// remembered as the creature's previously wielded melee or ranged weapon; any other weapon is removed, turned upright to
// face its current XY angle (a fraction of a turn: forward = (sin a, cos a, 0)), and made not-solid to the creature
// (adding the solid-to-thing component when missing). Mode 4 is removed from the creature on every path.
#include <cstdio>
#include <cmath>

struct Thing {
    int id;
    bool inventoryItem, dead, limbo, killed, hasWeaponTC, melee, hasSolid;
    int passiveSlot, sheatheObject, defIndex;
    bool hasWeaponDef;
    float facing;                       // fraction of a turn
    float up[3], forward[3];
    int notSolidTo;
    int removedModes;
    int prevMelee, prevRanged;
    bool hasInventoryWeapons;
};

struct Carrying {
    Thing* slots[4];
    int removed;
    void RemoveThing(Thing* t)
    {
        ++removed;
        for (int i = 0; i < 4; ++i)
            if (slots[i] == t) slots[i] = 0;
    }
    void Put(Thing* t, int slot) { slots[slot] = t; }
};

static const double kTwoPi = 6.2831854820251465;

static void DropWeapon(Thing& weapon, Carrying& carrying, Thing& creature)
{
    if (weapon.inventoryItem) {
        carrying.RemoveThing(&weapon);
        int slot = weapon.passiveSlot;
        Thing* old = carrying.slots[slot];
        if (old && !old->dead) {
            carrying.RemoveThing(old);
            if (old->inventoryItem) old->limbo = true;
            else old->killed = true;
        }
        if ((!weapon.hasWeaponDef || weapon.sheatheObject < 0) && !weapon.dead) {
            if (creature.hasInventoryWeapons && weapon.hasWeaponTC) {
                if (weapon.melee) creature.prevMelee = weapon.defIndex;
                else creature.prevRanged = weapon.defIndex;
            }
            carrying.Put(&weapon, slot);
        }
    }
    else {
        float cosPitch = (float)cos(0.0);
        weapon.up[0] = 0; weapon.up[1] = 0; weapon.up[2] = 1;
        weapon.forward[0] = (float)sin(weapon.facing * kTwoPi) * cosPitch;
        weapon.forward[1] = (float)cos(weapon.facing * kTwoPi) * cosPitch;
        weapon.forward[2] = (float)sin(0.0);
        carrying.RemoveThing(&weapon);
        if (!weapon.hasSolid) weapon.hasSolid = true;
        weapon.notSolidTo = creature.id;
    }
    ++creature.removedModes;
}

static Thing Make(int id)
{
    Thing t = { id, false, false, false, false, false, false, false, 0, -1, 0, false, 0.0f,
                { 0, 0, 0 }, { 0, 0, 0 }, 0, 0, -1, -1, false };
    return t;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    int fails = 0;

    // 1. inventory melee weapon, no sheathe object: back to its passive slot, the occupant goes to limbo
    {
        Thing creature = Make(1); creature.hasInventoryWeapons = true;
        Thing sword = Make(2); sword.inventoryItem = true; sword.passiveSlot = 2; sword.hasWeaponDef = true;
        sword.sheatheObject = -1; sword.hasWeaponTC = true; sword.melee = true; sword.defIndex = 77;
        Thing shield = Make(3); shield.inventoryItem = true;
        Carrying c = { { &sword, 0, &shield, 0 }, 0 };
        DropWeapon(sword, c, creature);
        if (c.slots[0] != 0 || c.slots[2] != &sword || !shield.limbo || shield.killed) ++fails;
        if (creature.prevMelee != 77 || creature.prevRanged != -1 || creature.removedModes != 1) ++fails;
    }
    // 2. ranged weapon whose def names a sheathe object: not re-slotted, non-item occupant killed
    {
        Thing creature = Make(1); creature.hasInventoryWeapons = true;
        Thing bow = Make(2); bow.inventoryItem = true; bow.passiveSlot = 1; bow.hasWeaponDef = true;
        bow.sheatheObject = 5; bow.hasWeaponTC = true; bow.defIndex = 12;
        Thing junk = Make(3);
        Carrying c = { { &bow, &junk, 0, 0 }, 0 };
        DropWeapon(bow, c, creature);
        if (c.slots[0] != 0 || c.slots[1] != 0 || !junk.killed || junk.limbo) ++fails;
        if (creature.prevRanged != -1 || creature.prevMelee != -1 || creature.removedModes != 1) ++fails;
    }
    // 3. ranged weapon without a weapon def: re-slotted and remembered as ranged; a dead occupant is left alone
    {
        Thing creature = Make(1); creature.hasInventoryWeapons = true;
        Thing bow = Make(2); bow.inventoryItem = true; bow.passiveSlot = 3; bow.hasWeaponTC = true; bow.defIndex = 9;
        Thing ghost = Make(3); ghost.dead = true;
        Carrying c = { { &bow, 0, 0, &ghost }, 0 };
        DropWeapon(bow, c, creature);
        if (c.slots[3] != &bow || ghost.killed || ghost.limbo || c.removed != 1) ++fails;
        if (creature.prevRanged != 9) ++fails;
    }
    // 4. a non-inventory thing: dropped upright facing a quarter turn, made not-solid to the creature
    {
        Thing creature = Make(1);
        Thing club = Make(2); club.facing = 0.25f;
        Carrying c = { { &club, 0, 0, 0 }, 0 };
        DropWeapon(club, c, creature);
        if (c.slots[0] != 0 || !club.hasSolid || club.notSolidTo != 1) ++fails;
        if (!Near(club.up[2], 1.0f) || !Near(club.forward[0], 1.0f) || !Near(club.forward[1], 0.0f)
            || !Near(club.forward[2], 0.0f)) ++fails;
        if (creature.removedModes != 1) ++fails;
    }

    if (fails) { printf("00845ce0_TEST FAIL\n"); return 1; }
    printf("00845ce0_TEST PASS\n");
    return 0;
}