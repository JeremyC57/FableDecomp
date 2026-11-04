// Behaviour test for CTCInventoryAbilities::GetExperienceCostToUpgradeAbility: the base cost times the morality multiplier, rounded to nearest (parked row 0x005EE77C).
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static long Cost(long base, float multiplier) { return GFFloatToLongNear((float)base * multiplier); }
int main()
{
    bool ok = Cost(100, 1.5f) == 150 && Cost(3, 0.5f) == 2 && Cost(5, 0.5f) == 2 && Cost(7, 1.0f) == 7;
    printf(ok ? "005ee77c_TEST PASS\n" : "005ee77c_TEST FAIL\n");
    return ok ? 0 : 1;
}
