// Behaviour model of CGameScriptInterface::GiveThingItemInSlot (0x00895190).
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
struct Thing { std::string def; bool dead, inLimbo, inInventory; Thing(const std::string& d) : def(d), dead(false), inLimbo(false), inInventory(false) {} };
struct Carrier { bool hasCarrying; std::map<long, Thing*> slots; Carrier() : hasCarrying(true) {} };
static long DefIndex(const std::string& name) { if (name == "OBJECT_SWORD") return 5; if (name == "SLOT_LEFT_HAND") return 2; if (name == "SLOT_RIGHT_HAND") return 1; return 0; }
static const long PrimarySlot = 1;
static Thing* GiveThingItemInSlot(Carrier* owner, const std::string& itemDefName, const std::string& slotName)
{
    if (!owner) return 0;
    if (itemDefName == "") {
        if (!owner->hasCarrying) return 0;
        long slot = DefIndex(slotName);
        if (slot <= 0) return 0;
        Thing* old = owner->slots[slot]; owner->slots[slot] = 0;
        if (old && !old->dead) old->dead = true;
        return 0;
    }
    long defIndex = DefIndex(itemDefName);
    if (defIndex <= 0) return 0;
    if (!owner->hasCarrying) return 0;
    Thing* item = new Thing(itemDefName);
    long slot = DefIndex(slotName);
    if (slot <= 0) return item;            // created but not placed (as retail)
    if (owner->slots[slot]) {
        if (slot == PrimarySlot) owner->slots[slot] = 0;
        else { Thing* old = owner->slots[slot]; owner->slots[slot] = 0; if (old->inInventory) old->inLimbo = true; }
    }
    owner->slots[slot] = item;
    return item;
}
static int fail(int n) { std::printf("00895190_TEST FAIL %d\n", n); return 1; }
int main()
{
    Carrier c; Thing lamp("OBJECT_LAMP"); lamp.inInventory = true; c.slots[2] = &lamp;
    Thing* s = GiveThingItemInSlot(&c, "OBJECT_SWORD", "SLOT_LEFT_HAND");
    if (!s || c.slots[2] != s || !lamp.inLimbo) return fail(1);
    Thing* s2 = GiveThingItemInSlot(&c, "OBJECT_SWORD", "SLOT_RIGHT_HAND");
    if (c.slots[1] != s2) return fail(2);
    GiveThingItemInSlot(&c, "", "SLOT_LEFT_HAND");
    if (c.slots[2] != 0 || !s->dead) return fail(3);
    if (GiveThingItemInSlot(&c, "OBJECT_UNKNOWN", "SLOT_LEFT_HAND") != 0) return fail(4);
    c.hasCarrying = false; if (GiveThingItemInSlot(&c, "OBJECT_SWORD", "SLOT_LEFT_HAND") != 0) return fail(5);
    delete s; delete s2;
    std::printf("00895190_TEST PASS\n");
    return 0;
}