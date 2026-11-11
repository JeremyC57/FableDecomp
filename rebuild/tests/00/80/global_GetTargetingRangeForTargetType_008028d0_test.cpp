// Behaviour test for GetTargetingRangeForTargetType (0x008028D0): the same selection over a host model.
#include <cstdio>
#include <map>

struct Def { std::map<long, float> ranges; };
static float GetTargetingRange(const Def& def, long type)
{
    std::map<long, float>::const_iterator it = def.ranges.find(type);
    if (it != def.ranges.end())
        return it->second;
    return 0.0f;
}
static const float& Max(const float& a, const float& b) { return a < b ? b : a; }
struct Weapon { bool projectile; long range; };
struct Thing { bool dead; bool spellMode; const Weapon* weapon; };

static float GetTargetingRangeForTargetType(unsigned long type, const Def& def, const Thing& targeter, const Thing* carried)
{
    float range = 0.0f;
    if (type != 2 && type != 1 && (type & 4) == 0 && targeter.spellMode)
        range = Max(range, GetTargetingRange(def, 8));
    if (type & 2) range = Max(range, GetTargetingRange(def, 2));
    if (type & 1) range = Max(range, GetTargetingRange(def, 1));
    if (type & 0x10) range = Max(range, GetTargetingRange(def, 0x10));
    if (carried && !carried->dead && carried->weapon && (type & 8) && carried->weapon->projectile) {
        range = Max(GetTargetingRange(def, 8), range);
        range = Max((float)carried->weapon->range, range);
    }
    if (type & 4) range = Max(range, GetTargetingRange(def, 4));
    return range;
}

int main()
{
    int fails = 0;
    Def def;
    def.ranges[1] = 5.0f; def.ranges[2] = 8.0f; def.ranges[4] = 3.0f; def.ranges[8] = 20.0f;
    Thing hero = { false, false, 0 };
    if (GetTargetingRangeForTargetType(1, def, hero, 0) != 5.0f) ++fails;
    if (GetTargetingRangeForTargetType(3, def, hero, 0) != 8.0f) ++fails;
    if (GetTargetingRangeForTargetType(0x10, def, hero, 0) != 0.0f) ++fails;      // no entry: 0
    hero.spellMode = true;
    if (GetTargetingRangeForTargetType(0x10, def, hero, 0) != 20.0f) ++fails;     // spell range applies
    if (GetTargetingRangeForTargetType(1, def, hero, 0) != 5.0f) ++fails;         // but not for type 1
    if (GetTargetingRangeForTargetType(4, def, hero, 0) != 3.0f) ++fails;         // nor with flag 4
    hero.spellMode = false;
    Weapon bow = { true, 35 };
    Thing carried = { false, false, &bow };
    if (GetTargetingRangeForTargetType(8, def, hero, &carried) != 35.0f) ++fails; // the bow's own range
    bow.range = 10;
    if (GetTargetingRangeForTargetType(8, def, hero, &carried) != 20.0f) ++fails; // the def's type-8 range wins
    carried.dead = true;
    if (GetTargetingRangeForTargetType(8, def, hero, &carried) != 0.0f) ++fails;
    carried.dead = false; bow.projectile = false;
    if (GetTargetingRangeForTargetType(9, def, hero, &carried) != 5.0f) ++fails;
    if (fails) { std::printf("008028d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008028d0_TEST PASS\n");
    return 0;
}