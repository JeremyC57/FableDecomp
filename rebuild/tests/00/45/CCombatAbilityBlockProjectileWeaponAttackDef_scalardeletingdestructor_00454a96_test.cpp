#include <cstdio>
#include <new>
class CCombatAbilityBlockProjectileWeaponAttackDef { public: int f0; };
static int g_dtor_calls = 0;
void __fastcall CCombatAbilityBlockProjectileWeaponAttackDef_scalar_dtor(CCombatAbilityBlockProjectileWeaponAttackDef* def) { g_dtor_calls++; }

void* __fastcall CCombatAbilityBlockProjectileWeaponAttackDef_scalar_deleting_destructor(CCombatAbilityBlockProjectileWeaponAttackDef* def, int edx_unused, unsigned int flags)
{
    CCombatAbilityBlockProjectileWeaponAttackDef_scalar_dtor(def);
    if (flags & 1)
        ::operator delete(def);
    return def;
}

int main() {
    CCombatAbilityBlockProjectileWeaponAttackDef* obj = (CCombatAbilityBlockProjectileWeaponAttackDef*)::operator new(sizeof(CCombatAbilityBlockProjectileWeaponAttackDef));
    void* r1 = CCombatAbilityBlockProjectileWeaponAttackDef_scalar_deleting_destructor(obj, 0, 0);
    if (r1 != obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL dtor1\n"); return 1; }
    void* r2 = CCombatAbilityBlockProjectileWeaponAttackDef_scalar_deleting_destructor(obj, 0, 1);
    if (r2 != obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_calls != 2) { std::printf("FAIL dtor2\n"); return 1; }
    CCombatAbilityBlockProjectileWeaponAttackDef* obj2 = (CCombatAbilityBlockProjectileWeaponAttackDef*)::operator new(sizeof(CCombatAbilityBlockProjectileWeaponAttackDef));
    void* r3 = CCombatAbilityBlockProjectileWeaponAttackDef_scalar_deleting_destructor(obj2, 0, 3);
    if (r3 != obj2 || g_dtor_calls != 3) { std::printf("FAIL flags3\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00454a96_TEST PASS\n");
    return 0;
}