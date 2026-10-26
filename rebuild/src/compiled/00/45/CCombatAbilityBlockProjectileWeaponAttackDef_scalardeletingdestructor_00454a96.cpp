#pragma optimize("s",on)
#include <new>
class CCombatAbilityBlockProjectileWeaponAttackDef { public: int f0; };
extern void __fastcall CCombatAbilityBlockProjectileWeaponAttackDef_scalar_dtor(CCombatAbilityBlockProjectileWeaponAttackDef* def);

void* __fastcall CCombatAbilityBlockProjectileWeaponAttackDef_scalar_deleting_destructor(CCombatAbilityBlockProjectileWeaponAttackDef* def, int edx_unused, unsigned int flags)
{
    CCombatAbilityBlockProjectileWeaponAttackDef_scalar_dtor(def);
    if (flags & 1)
        ::operator delete(def);
    return def;
}