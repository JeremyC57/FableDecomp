// Behaviour test of CTCInventoryWeapons::SetThingAsActiveWeapon (0x005CB2B0): a weapon of a def that is already active is ignored;
// otherwise the created object becomes the active melee/ranged weapon, the entry gets the assigned icon (the old icon deleted), the
// replaced weapon is unwielded and its sheathe object (if carried) removed and killed, and the carrier is set up again unless the
// inventory is open or the hero's weapons are unusable.
#include <cstdio>
#include <vector>
class CThing { public:
    long def; bool dead; bool limbo; bool killed; bool melee; bool wielded; long graphic; long sheatheObject;
};
class CTCCarrying { public:
    std::vector<CThing*> carried;            // slot n -> thing (slot = def index)
    bool IsCarryingThing(long slot) const { for (unsigned i = 0; i < carried.size(); ++i) if (carried[i]->def == slot) return true; return false; }
    CThing* GetCarriedThing(long slot) const { for (unsigned i = 0; i < carried.size(); ++i) if (carried[i]->def == slot) return carried[i]; return 0; }
    void RemoveThing(CThing& t) { for (unsigned i = 0; i < carried.size(); ++i) if (carried[i] == &t) { carried.erase(carried.begin() + i); return; } }
};
struct CEntry { int icons; int deletions; bool isDefault; };
static bool IS_THING_ALIVE(CThing* t) { return t != 0 && !t->dead; }
struct CWeapons {
    CThing* PActiveMeleeWeapon; CThing* PActiveRangedWeapon;
    CEntry* PAssignedMeleeIcon; CEntry* PAssignedRangedIcon; CEntry* PAssignedMeleeMenuEntry; CEntry* PAssignedRangedMenuEntry;
    bool InventoryOpen; bool WeaponsUsable; bool hasHero; int setups; CThing* next; CTCCarrying carrying;
    CThing* CreateInventoryObject(long def) { next->def = def; return next; }
    void SetThingAsActiveWeapon(long itemDefIndex, CEntry* entry)
    {
        if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon->def == itemDefIndex) return;
        if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon->def == itemDefIndex) return;
        CThing* thing = CreateInventoryObject(itemDefIndex);
        if (!IS_THING_ALIVE(thing)) return;
        thing->limbo = true;
        CThing* previous;
        CThing** active = thing->melee ? &PActiveMeleeWeapon : &PActiveRangedWeapon;
        CEntry** icon = thing->melee ? &PAssignedMeleeIcon : &PAssignedRangedIcon;
        CEntry** menuEntry = thing->melee ? &PAssignedMeleeMenuEntry : &PAssignedRangedMenuEntry;
        previous = *active; *active = thing;
        if (entry) {
            *menuEntry = entry;
            if (*icon) ++(*icon)->deletions;
            if (thing->graphic != 0) { ++entry->icons; *icon = entry; }
            entry->isDefault = true;
        }
        if (IS_THING_ALIVE(previous)) {
            previous->wielded = false;
            if (previous->sheatheObject > 0 && carrying.IsCarryingThing(previous->sheatheObject)) {
                CThing* sheathe = carrying.GetCarriedThing(previous->sheatheObject);
                carrying.RemoveThing(*sheathe);
                sheathe->killed = true;
            }
        }
        thing->wielded = true;
        if (!InventoryOpen && (!hasHero || WeaponsUsable)) ++setups;
    }
};
int main()
{
    CThing oldSword = { 3, false, false, false, true, true, 1, 40 }, sheathe = { 40, false, false, false, false, false, 0, 0 };
    CThing newSword = { 0, false, false, false, true, false, 7, 0 };
    CWeapons w = { &oldSword, 0, 0, 0, 0, 0, false, true, true, 0, &newSword };
    w.carrying.carried.push_back(&sheathe);
    CEntry e = { 0, 0, false };
    w.SetThingAsActiveWeapon(3, &e);                 // already active: nothing
    int bad = w.setups != 0 || e.icons != 0;
    w.SetThingAsActiveWeapon(5, &e);
    bad |= !(w.PActiveMeleeWeapon == &newSword && newSword.limbo && newSword.wielded && !oldSword.wielded && sheathe.killed
             && w.carrying.carried.empty() && e.icons == 1 && e.isDefault && w.PAssignedMeleeMenuEntry == &e && w.setups == 1);
    CThing bow = { 0, false, false, false, false, false, 0, 0 };
    w.next = &bow; w.InventoryOpen = true;
    w.SetThingAsActiveWeapon(9, 0);
    bad |= !(w.PActiveRangedWeapon == &bow && bow.wielded && w.setups == 1 && w.PAssignedRangedIcon == 0);
    if (bad) { std::printf("005cb2b0_TEST FAIL\n"); return 1; }
    std::printf("005cb2b0_TEST PASS\n");
    return 0;
}