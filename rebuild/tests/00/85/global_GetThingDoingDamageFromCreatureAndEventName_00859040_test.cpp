// Behaviour test for GetThingDoingDamageFromCreatureAndEventName (0x00859040): with the carry-slot flag the creature's
// carry slot named by the first helper point (right hand / left hand, else slot -1) wins; without it a living
// primary-slot weapon with the helper points wins, then the creature itself when it has them, else null. A creature
// without the carrying interface skips straight to its own test. Every string built is destroyed again.
#include <cstdio>
#include <cstring>

static int g_live = 0;   // live name objects (constructed minus destroyed)

struct Name {
    const char* text;
    Name(const char* t) : text(t) { ++g_live; }
    Name(const Name& o) : text(o.text) { ++g_live; }
    ~Name() { --g_live; }
    bool operator==(const char* t) const { return std::strcmp(text, t) == 0; }
};

struct NameList {                    // stands in for vector<CCharString>: up to 4 names
    const char* names[4];
    unsigned int count;
    NameList() : count(0) { }
    void push_back(const Name& n) { names[count++] = n.text; }
    Name operator[](unsigned int i) const { return Name(names[i]); }
};

struct Thing;
struct Carrying {
    Thing* primary;
    Thing* right;
    Thing* left;
    Thing* GetThingInPrimarySlot() const { return primary; }
    Thing* GetThingInCarrySlot(long slot) const { return slot == 7 ? right : slot == 9 ? left : 0; }
};
struct Thing {
    bool hasCarrying;
    Carrying carrying;
    bool dead;
    bool hasHelperPoints;
};

static long GetDefGlobalIndexFromName(const Name& n)
{
    if (n == "CARRY_SLOT_RIGHT_HAND") return 7;
    if (n == "CARRY_SLOT_LEFT_HAND") return 9;
    return -1;
}

static void GetHelperPointNames(const Name& eventName, NameList& names)
{
    names.push_back(eventName);
}

static bool IsThingWithHelperPoints(const Thing& thing, NameList&) { return thing.hasHelperPoints; }

static Thing* GetThingDoingDamage(Thing& creature, const Name& eventName, bool useCarrySlot)
{
    NameList helperPointNames;
    GetHelperPointNames(eventName, helperPointNames);
    if (creature.hasCarrying) {
        Carrying* carrying = &creature.carrying;
        if (useCarrySlot) {
            Name helperPointName = helperPointNames[0];
            long slot = -1;
            if (helperPointName == "weapon_pos_right_hand") {
                Name slotName("CARRY_SLOT_RIGHT_HAND");
                slot = GetDefGlobalIndexFromName(slotName);
            } else if (helperPointName == "weapon_pos_left_hand") {
                Name slotName("CARRY_SLOT_LEFT_HAND");
                slot = GetDefGlobalIndexFromName(slotName);
            }
            return carrying->GetThingInCarrySlot(slot);
        }
        Thing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon && !weapon->dead && IsThingWithHelperPoints(*weapon, helperPointNames))
            return weapon;
    }
    if (IsThingWithHelperPoints(creature, helperPointNames))
        return &creature;
    return 0;
}

int main()
{
    int fails = 0;
    Thing sword = { false, { 0, 0, 0 }, false, true };
    Thing shield = { false, { 0, 0, 0 }, false, true };
    Thing hero = { true, { &sword, &sword, &shield }, false, true };
    {
        Name right("weapon_pos_right_hand"), left("weapon_pos_left_hand"), other("weapon_pos_a");
        if (GetThingDoingDamage(hero, right, true) != &sword) ++fails;
        if (GetThingDoingDamage(hero, left, true) != &shield) ++fails;
        if (GetThingDoingDamage(hero, other, true) != 0) ++fails;           // slot -1: nothing carried there
        if (GetThingDoingDamage(hero, other, false) != &sword) ++fails;     // living primary weapon with the points
        sword.dead = true;
        if (GetThingDoingDamage(hero, other, false) != &hero) ++fails;      // dead weapon: the creature itself
        sword.dead = false;
        sword.hasHelperPoints = false;
        if (GetThingDoingDamage(hero, other, false) != &hero) ++fails;
        hero.hasHelperPoints = false;
        if (GetThingDoingDamage(hero, other, false) != 0) ++fails;
        Thing beast = { false, { &sword, &sword, &shield }, false, true };
        if (GetThingDoingDamage(beast, right, true) != &beast) ++fails;     // no carrying interface
    }
    if (g_live != 0) ++fails;
    if (fails) { std::printf("00859040_TEST FAIL\n"); return 1; }
    std::printf("00859040_TEST PASS\n");
    return 0;
}