struct CCombatAbilityData {
    int f0;
    int PercentageChance;
    int f8;
    int fc;
    int f10;
    int InterfaceType;
};

CCombatAbilityData* __fastcall CCombatAbilityData_operator_assign(CCombatAbilityData* ability, int edx_dummy, const CCombatAbilityData* rhs) {
    ability->PercentageChance  = rhs->PercentageChance;
    ability->f8  = rhs->f8;
    ability->fc  = rhs->fc;
    ability->f10 = rhs->f10;
    ability->InterfaceType = rhs->InterfaceType;
    return ability;
}