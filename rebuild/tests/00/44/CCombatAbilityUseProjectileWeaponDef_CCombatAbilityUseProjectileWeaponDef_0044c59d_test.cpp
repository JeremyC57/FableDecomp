#include <cstdio>
#include <stddef.h>
static int g_base = 0; static long g_type = -1;
class CSubDefClassBase { public: virtual void v00() { } unsigned char _body[0x24]; CSubDefClassBase() { ++g_base; } };
struct CCombatAbilityProjectileAttackData_Calls { virtual void v00() { } unsigned char _body[0x18]; CCombatAbilityProjectileAttackData_Calls(long interfaceType) { g_type = interfaceType; } };
class CCombatAbilityUseProjectileWeaponDef : public CSubDefClassBase { public: CCombatAbilityProjectileAttackData_Calls CombatAbility; virtual void v00() { } CCombatAbilityUseProjectileWeaponDef() : CSubDefClassBase(), CombatAbility(7) { } };
int main()
{
    CCombatAbilityUseProjectileWeaponDef d;
    if (g_base != 1 || !(g_type == 7) || offsetof(CCombatAbilityUseProjectileWeaponDef, CombatAbility) != 0x28) { std::printf("0044c59d_TEST FAIL\n"); return 1; }
    std::printf("0044c59d_TEST PASS\n"); return 0;
}