// behaviour test for 0x00453EB8 CCombatAbilityBlockProjectileWeaponAttackDef::CCombatAbilityBlockProjectileWeaponAttackDef() (model: the base receives (4, 3))
#include <cstdio>
static int g_t = -1, g_w = -1;
struct Base { Base(int t, int w) { g_t = t; g_w = w; } };
struct Derived : Base { Derived() : Base(4, 3) { } };
int main()
{
    Derived d; (void)d;
    int ok = g_t == 4 && g_w == 3;
    if (!ok) { std::printf("00453eb8_TEST FAIL\n"); return 1; }
    std::printf("00453eb8_TEST PASS\n");
    return 0;
}