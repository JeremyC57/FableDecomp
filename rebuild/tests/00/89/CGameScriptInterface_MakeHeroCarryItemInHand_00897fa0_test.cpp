// Behaviour model of CGameScriptInterface::MakeHeroCarryItemInHand (0x00897FA0).
#include <cstdio>
#include <vector>
struct Thing;
struct Inventory { Thing* owner; std::vector<Thing*> items; Thing* activeWeapon; Inventory() : owner(0), activeWeapon(0) {} };
struct Thing {
    bool dead, inLimbo, isItem, isCarryable, isWeapon; int defIndex; int carrySlot; Inventory* inventory;
    bool hasInventory; Inventory heroInventory; bool hasCarrying; Thing* slots[4];
    Thing() : dead(false), inLimbo(true), isItem(true), isCarryable(true), isWeapon(false), defIndex(0), carrySlot(0), inventory(0),
              hasInventory(false), hasCarrying(true) { for (int i = 0; i < 4; ++i) slots[i] = 0; }
};
static Thing* InventoryInstance(Inventory* inv, int defIndex)
{
    for (unsigned int i = 0; i < inv->items.size(); ++i) if (inv->items[i]->defIndex == defIndex) return inv->items[i];
    return 0;
}
static void MakeHeroCarryItemInHand(Thing* hero, Thing* object, bool valid, bool makeActiveWeapon)
{
    if (!hero || hero->dead) return;
    if (!valid) return;
    bool added = false;
    if (!object->isItem) return;
    if (!object->isCarryable) return;
    if (object->inventory == 0) {
        if (!hero->hasInventory) return;
        Thing* copy = new Thing(*object);           // the inventory keeps its own instance
        copy->inventory = &hero->heroInventory;
        hero->heroInventory.items.push_back(copy);
        object->inventory = &hero->heroInventory;
        added = true;
    } else if (object->inventory->owner != hero) return;
    if (object->inventory == 0) return;
    if (makeActiveWeapon && object->isWeapon) object->inventory->activeWeapon = object;
    if (added) object = InventoryInstance(object->inventory, object->defIndex);
    int slot = object->carrySlot;
    if (hero->slots[slot] != 0) hero->slots[0] = 0;          // clear the primary slot when the target slot is taken
    for (int i = 0; i < 4; ++i) if (hero->slots[i] == object) hero->slots[i] = 0;
    hero->slots[slot] = object;
    object->inLimbo = false;
}
static int fail(int n) { std::printf("00897fa0_TEST FAIL %d\n", n); return 1; }
int main()
{
    Thing hero; hero.hasInventory = true; hero.heroInventory.owner = &hero;
    Thing sword; sword.defIndex = 7; sword.isWeapon = true; sword.carrySlot = 0;
    MakeHeroCarryItemInHand(&hero, &sword, true, true);
    if (!hero.slots[0] || hero.slots[0] == &sword || hero.slots[0]->defIndex != 7 || hero.slots[0]->inLimbo) return fail(1);
    if (hero.heroInventory.items.size() != 1) return fail(2);
    Thing* inst = hero.slots[0];
    MakeHeroCarryItemInHand(&hero, inst, true, true);
    if (hero.slots[0] != inst || hero.heroInventory.activeWeapon != inst) return fail(3);
    Inventory other; Thing stranger; other.owner = &stranger; Thing lamp; lamp.inventory = &other; lamp.carrySlot = 1;
    MakeHeroCarryItemInHand(&hero, &lamp, true, false);
    if (hero.slots[1] != 0 || !lamp.inLimbo) return fail(4);
    Thing rock; rock.isCarryable = false; MakeHeroCarryItemInHand(&hero, &rock, true, false);
    if (!rock.inLimbo) return fail(5);
    hero.dead = true; Thing gem; gem.carrySlot = 2; MakeHeroCarryItemInHand(&hero, &gem, true, false);
    if (hero.slots[2] != 0) return fail(6);
    for (unsigned int i = 0; i < hero.heroInventory.items.size(); ++i) delete hero.heroInventory.items[i];
    std::printf("00897fa0_TEST PASS\n");
    return 0;
}