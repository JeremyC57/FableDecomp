#include <cstdio>
#include <stddef.h>
static int g_base = 0; static long g_type = -1, g_weapon = -1;
struct CSubDefClassBase { virtual void v00() { } unsigned char _body[0x24]; CSubDefClassBase() { ++g_base; } };
struct CCombatAbilityBlockingData_Calls { virtual void v00() { } unsigned char _body[0x2c]; CCombatAbilityBlockingData_Calls(long t, long w) { g_type = t; g_weapon = w; } };
struct CCombatAbilityBlockDefBase : CSubDefClassBase {
    CCombatAbilityBlockingData_Calls CombatAbility; virtual void v00() { }
    CCombatAbilityBlockDefBase(long t, long w) : CSubDefClassBase(), CombatAbility(t, w) { }
};
int main()
{
    CCombatAbilityBlockDefBase d(3, 5);
    if (g_base != 1 || g_type != 3 || g_weapon != 5 || offsetof(CCombatAbilityBlockDefBase, CombatAbility) != 0x28) { std::printf("00453e2d_TEST FAIL\n"); return 1; }
    std::printf("00453e2d_TEST PASS\n"); return 0;
}