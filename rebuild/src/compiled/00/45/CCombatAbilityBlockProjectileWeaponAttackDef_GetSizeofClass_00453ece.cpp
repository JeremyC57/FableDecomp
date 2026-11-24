// Leaf: ?GetSizeofClass@CCombatAbilityBlockProjectileWeaponAttackDef@@UAEKXZ
#pragma optimize("s",on)
typedef unsigned long ulong;

class CCombatAbilityBlockProjectileWeaponAttackDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CCombatAbilityBlockProjectileWeaponAttackDef::GetSizeofClass()
{
    return 0x58;
}
// Flatten route (flatten_pilot v4, NF324): CCombatAbilityBlockProjectileWeaponAttackDef_GetSizeofClass -> CCombatAbilityBlockProjectileWeaponAttackDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
