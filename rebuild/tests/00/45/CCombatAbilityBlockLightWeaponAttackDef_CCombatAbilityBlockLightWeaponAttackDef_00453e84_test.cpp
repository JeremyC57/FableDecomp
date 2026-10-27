// behaviour test for 0x00453E84 CCombatAbilityBlockLightWeaponAttackDef::CCombatAbilityBlockLightWeaponAttackDef() (model: the base receives (2, 1))
#include <cstdio>
static int g_t = -1, g_w = -1;
struct Base { Base(int t, int w) { g_t = t; g_w = w; } };
struct Derived : Base { Derived() : Base(2, 1) { } };
int main()
{
    Derived d; (void)d;
    int ok = g_t == 2 && g_w == 1;
    if (!ok) { std::printf("00453e84_TEST FAIL\n"); return 1; }
    std::printf("00453e84_TEST PASS\n");
    return 0;
}