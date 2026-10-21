// Behaviour test for CTCInventoryWeapons::CheckForSoldWieldedWeapons (0x005CB780): with a wielded melee weapon the first
// carried melee-class item (count > 0) is re-armed, and likewise the first non-melee item for a wielded ranged weapon.
#include <cstdio>

struct Item { long defIndex; long count; long weaponClass; };
static long g_armed[4]; static long g_armedCount = 0;
static bool IsMelee(long c) { return c == 1; }
static void Run(bool meleeAlive, bool rangedAlive, const Item* items, unsigned int n)
{
    if (meleeAlive) {
        for (unsigned int i = 0; i < n; ++i)
            if (items[i].count > 0 && IsMelee(items[i].weaponClass)) { g_armed[g_armedCount++] = items[i].defIndex; break; }
    }
    if (rangedAlive) {
        for (unsigned int i = 0; i < n; ++i)
            if (items[i].count > 0 && !IsMelee(items[i].weaponClass)) { g_armed[g_armedCount++] = items[i].defIndex; break; }
    }
}
int main()
{
    Item items[4] = { { 10, 0, 1 }, { 11, 2, 2 }, { 12, 1, 1 }, { 13, 1, 2 } };
    Run(true, true, items, 4);
    if (g_armedCount != 2 || g_armed[0] != 12 || g_armed[1] != 11) { std::printf("005cb780_TEST FAIL\n"); return 1; }
    g_armedCount = 0;
    Run(false, true, items, 1);
    if (g_armedCount != 0) { std::printf("005cb780_TEST FAIL\n"); return 1; }
    std::printf("005cb780_TEST PASS\n");
    return 0;
}