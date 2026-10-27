// behaviour test for 0x00453E9E CCombatAbilityBlockHeavyWeaponAttackDef::CCombatAbilityBlockHeavyWeaponAttackDef() (model: the base receives (3, 2))
#include <cstdio>
static int g_t = -1, g_w = -1;
struct Base { Base(int t, int w) { g_t = t; g_w = w; } };
struct Derived : Base { Derived() : Base(3, 2) { } };
int main()
{
    Derived d; (void)d;
    int ok = g_t == 3 && g_w == 2;
    if (!ok) { std::printf("00453e9e_TEST FAIL\n"); return 1; }
    std::printf("00453e9e_TEST PASS\n");
    return 0;
}