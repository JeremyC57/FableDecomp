// Leaf: ?GetSizeofClass@CCombatAbilityGetHitCounterAttackDef@@UAEKXZ
#pragma optimize("s",on)
class CCombatAbilityGetHitCounterAttackDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CCombatAbilityGetHitCounterAttackDef::GetSizeofClass()
{
    return 0x48;
}
// Flatten route (flatten_pilot v4, NF324): CCombatAbilityGetHitCounterAttackDef_GetSizeofClass -> CCombatAbilityGetHitCounterAttackDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
