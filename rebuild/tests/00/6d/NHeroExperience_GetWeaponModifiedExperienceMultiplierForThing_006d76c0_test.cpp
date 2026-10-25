// Behaviour test for 0x006D76C0 NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing (model copy): 1.0 without
// the weapons component; otherwise the live augmented melee weapon's multiplier times the live augmented ranged weapon's.
#include <cstdio>

struct Augs { float mult; float GetExperienceMultiplier() const { return mult; } };
struct Thing;
struct Weapons { Thing* melee; Thing* ranged; };
struct Thing {
    bool hasWeapons; bool hasAugs; bool dead;
    Weapons* weapons; Augs* augs;
    bool IsDead() const { return dead; }
};

static float GetMultiplier(const Thing& thing)
{
    float multiplier = 1.0f;
    if (thing.hasWeapons) {
        const Thing* melee = thing.weapons->melee;
        const Thing* ranged = thing.weapons->ranged;
        if (melee && !melee->IsDead() && melee->hasAugs)
            multiplier = melee->augs->GetExperienceMultiplier();
        if (ranged && !ranged->IsDead() && ranged->hasAugs)
            multiplier *= ranged->augs->GetExperienceMultiplier();
    }
    return multiplier;
}

static Thing MakeThing(bool hasWeapons, bool hasAugs, bool dead, Weapons* w, Augs* a)
{
    Thing t; t.hasWeapons = hasWeapons; t.hasAugs = hasAugs; t.dead = dead; t.weapons = w; t.augs = a; return t;
}

int main()
{
    Augs a2; a2.mult = 2.0f; Augs a3; a3.mult = 3.0f;
    Thing sword = MakeThing(false, true, false, 0, &a2);
    Thing bow = MakeThing(false, true, false, 0, &a3);
    Thing deadBow = MakeThing(false, true, true, 0, &a3);
    Thing plainBow = MakeThing(false, false, false, 0, 0);
    Weapons both = { &sword, &bow };
    Weapons none = { 0, 0 };
    Weapons bowOnly = { 0, &bow };
    Weapons deadRanged = { &sword, &deadBow };
    Weapons plainRanged = { &sword, &plainBow };
    int ok = GetMultiplier(MakeThing(false, false, false, &both, 0)) == 1.0f;
    ok = ok && GetMultiplier(MakeThing(true, false, false, &both, 0)) == 6.0f;
    ok = ok && GetMultiplier(MakeThing(true, false, false, &none, 0)) == 1.0f;
    ok = ok && GetMultiplier(MakeThing(true, false, false, &bowOnly, 0)) == 3.0f;
    ok = ok && GetMultiplier(MakeThing(true, false, false, &deadRanged, 0)) == 2.0f;
    ok = ok && GetMultiplier(MakeThing(true, false, false, &plainRanged, 0)) == 2.0f;
    if (!ok) { std::printf("006d76c0_TEST FAIL\n"); return 1; }
    std::printf("006d76c0_TEST PASS\n");
    return 0;
}