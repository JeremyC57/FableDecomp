#include <cstdio>
// Behaviour model of CTCStealth::vfunc_16 (0x00747110): the region-based danger limit resets to -1; when switched off it takes
// the current region def's MaxNumAttackersOnHero if the region has a def.
struct RegionDef { long maxAttackers; };
static long Apply(bool on, const RegionDef* def)
{
    long limit = -1;
    if (!on && def) limit = def->maxAttackers;
    return limit;
}
int main()
{
    RegionDef d = { 3 };
    if (Apply(true, &d) != -1 || Apply(false, &d) != 3 || Apply(false, 0) != -1) { std::printf("00747110_TEST FAIL\n"); return 1; }
    std::printf("00747110_TEST PASS\n");
    return 0;
}
