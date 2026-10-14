#include <cstdio>
#include <stddef.h>
static int g_base = 0; static long g_type = -1;
struct CSubDefClassBase { virtual void v00() { } unsigned char _body[0x24]; CSubDefClassBase() { ++g_base; } };
struct CCombatAbilityStrafingData_Calls { virtual void v00() { } unsigned char _body[0x18]; CCombatAbilityStrafingData_Calls(long interfaceType) { g_type = interfaceType; } };
struct CCombatAbilityStrafeDef_Retail : CSubDefClassBase { CCombatAbilityStrafingData_Calls CombatAbility; virtual void v00() { } CCombatAbilityStrafeDef_Retail() : CSubDefClassBase(), CombatAbility(8) { } };
int main()
{
    CCombatAbilityStrafeDef_Retail d;
    if (g_base != 1 || !(g_type == 8) || offsetof(CCombatAbilityStrafeDef_Retail, CombatAbility) != 0x28) { std::printf("0044c57d_TEST FAIL\n"); return 1; }
    std::printf("0044c57d_TEST PASS\n"); return 0;
}