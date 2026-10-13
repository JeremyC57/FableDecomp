#include <cstdio>

static int g_un = 0;
struct V { bool out; virtual bool AreWeaponsUnsheathed() { return out; } void UnsheatheMeleeWeapon() { ++g_un; }
    void UnsheatheAllWeapons() { if (!AreWeaponsUnsheathed()) UnsheatheMeleeWeapon(); } };
int main()
{
    V v; v.out = true; v.UnsheatheAllWeapons(); if (g_un != 0) { std::printf("00666830_TEST FAIL\n"); return 1; }
    v.out = false; v.UnsheatheAllWeapons(); if (g_un != 1) { std::printf("00666830_TEST FAIL\n"); return 1; }
    std::printf("00666830_TEST PASS\n");
    return 0;
}