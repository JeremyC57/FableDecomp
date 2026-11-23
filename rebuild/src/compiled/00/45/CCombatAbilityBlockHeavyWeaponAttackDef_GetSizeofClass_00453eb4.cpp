// Leaf: ?GetSizeofClass@CCombatAbilityBlockHeavyWeaponAttackDef@@UAEKXZ
#pragma optimize("s",on)
class CCombatAbilityBlockHeavyWeaponAttackDef { public: char pad[0x54];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CCombatAbilityBlockHeavyWeaponAttackDef::GetSizeofClass()
{
    const CCombatAbilityBlockHeavyWeaponAttackDef* self = this;
    (void)self;
    return 0x58;
}