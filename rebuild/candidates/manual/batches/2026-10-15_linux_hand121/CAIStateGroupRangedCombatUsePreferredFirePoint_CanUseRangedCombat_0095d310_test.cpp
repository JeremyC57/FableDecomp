#include <cstdio>
// RangedCombatUsePreferredFirePoint::CanUseRangedCombat: the primary-slot thing is a projectile weapon
struct Item { bool hasWeapon; bool projectile; };
struct Carrying { Item* primary; Item* GetThingInPrimarySlot() { return primary; } };
struct Group {
    Carrying* carrying;
    bool CanUseRangedCombat() { Item* item = carrying->GetThingInPrimarySlot(); if (item != 0) { if (item->hasWeapon) { if (item->projectile) return true; } } return false; }
};
int main()
{
    Item i; i.hasWeapon = true; i.projectile = true; Carrying c; c.primary = &i; Group g; g.carrying = &c;
    if (!g.CanUseRangedCombat()) { std::printf("0095d310_TEST FAIL\n"); return 1; }
    i.projectile = false; if (g.CanUseRangedCombat()) { std::printf("0095d310_TEST FAIL\n"); return 1; }
    i.projectile = true; i.hasWeapon = false; if (g.CanUseRangedCombat()) { std::printf("0095d310_TEST FAIL\n"); return 1; }
    c.primary = 0; if (g.CanUseRangedCombat()) { std::printf("0095d310_TEST FAIL\n"); return 1; }
    std::printf("0095d310_TEST PASS\n"); return 0;
}
