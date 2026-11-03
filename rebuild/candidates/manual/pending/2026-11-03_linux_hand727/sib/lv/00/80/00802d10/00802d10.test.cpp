// behaviour test for 0x00802D10 NPlayerTargeting::IsThingInTargetingRangeForTargetType(const CThing& thing, const CThing&
// target, unsigned long type): the carried thing (primary carrying slot, if the thing has the carrying component 0x46)
// feeds the targeting-def range lookup; the test is the horizontal (X/Y) distance squared against range squared.
#include <cstdio>
struct Vec { float x, y, z; };
struct Thing { Vec pos; bool carrying; const Thing* carried; };
struct Def { float baseRange; };
static float GetTargetingRangeForTargetType(unsigned long type, const Def& def, const Thing&, const Thing* carried)
{
    float range = def.baseRange + (float)type;
    if (carried) range *= 2.0f;
    return range;
}
static bool IsThingInTargetingRangeForTargetType(const Thing& thing, const Thing& target, unsigned long type, const Def& def)
{
    const Thing* carried = 0;
    if (thing.carrying) carried = thing.carried;
    Vec delta = { target.pos.x - thing.pos.x, target.pos.y - thing.pos.y, target.pos.z - thing.pos.z };
    float range = GetTargetingRangeForTargetType(type, def, target, carried);
    float distanceSquared = delta.y * delta.y;
    distanceSquared += delta.x * delta.x;
    return distanceSquared < range * range;
}
int main()
{
    Def def = { 3.0f };
    Thing sword = { { 0, 0, 0 }, false, 0 };
    Thing hero = { { 0, 0, 0 }, false, 0 };
    Thing far = { { 3.0f, 4.0f, 100.0f }, false, 0 };     // horizontal distance 5, height ignored
    bool ok = !IsThingInTargetingRangeForTargetType(hero, far, 0, def);   // 25 < 9 ? no
    ok = ok && IsThingInTargetingRangeForTargetType(hero, far, 3, def);   // range 6: 25 < 36
    hero.carrying = true; hero.carried = &sword;                           // carried doubles the range: 6 -> 36
    ok = ok && IsThingInTargetingRangeForTargetType(hero, far, 0, def);
    hero.carried = 0;                                                      // component present, empty slot: no carried thing
    ok = ok && !IsThingInTargetingRangeForTargetType(hero, far, 0, def);
    Thing edge = { { 0, 3.0f, 0 }, false, 0 };                             // exactly on the range is not inside
    ok = ok && !IsThingInTargetingRangeForTargetType(hero, edge, 0, def);
    if (!ok) { std::printf("00802d10_TEST FAIL\n"); return 1; }
    std::printf("00802d10_TEST PASS\n");
    return 0;
}