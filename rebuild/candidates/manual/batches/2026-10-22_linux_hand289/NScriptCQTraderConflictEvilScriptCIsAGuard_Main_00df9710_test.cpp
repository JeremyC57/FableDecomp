#include <cstdio>
// Behaviour model of CIsAGuard::Main (0x00DF9710): the per-frame hit classification.
struct Hit { bool any, hero, anyAbility, ability14; float heroDistance; };
struct Out { int selfDamage; bool engaged; };
static void Frame(const Hit& h, Out& o)
{
    if (h.any && !h.hero && (!h.anyAbility || h.ability14)) ++o.selfDamage;
    else if (h.hero || (h.anyAbility && !h.ability14)) o.engaged = true;
    if (h.heroDistance < 15.0f) o.engaged = true;
}
int main()
{
    int ok = 1;
    Out o = { 0, false };
    Hit bandit = { true, false, false, false, 100.0f }; Frame(bandit, o); ok = ok && o.selfDamage == 1 && !o.engaged;
    Hit ab14 = { true, false, true, true, 100.0f }; Frame(ab14, o); ok = ok && o.selfDamage == 2 && !o.engaged;
    Hit ab = { true, false, true, false, 100.0f }; Frame(ab, o); ok = ok && o.selfDamage == 2 && o.engaged;
    Out p = { 0, false };
    Hit hero = { true, true, false, false, 100.0f }; Frame(hero, p); ok = ok && p.selfDamage == 0 && p.engaged;
    Out q = { 0, false };
    Hit near = { false, false, false, false, 10.0f }; Frame(near, q); ok = ok && q.engaged;
    Out r = { 0, false };
    Hit none = { false, false, false, false, 20.0f }; Frame(none, r); ok = ok && !r.engaged && r.selfDamage == 0;
    if (!ok) { std::printf("00df9710_TEST FAIL\n"); return 1; }
    std::printf("00df9710_TEST PASS\n");
    return 0;
}
