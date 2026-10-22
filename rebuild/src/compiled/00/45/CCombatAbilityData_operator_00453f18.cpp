struct CCombatAbilityData {
    int f0;
    int PercentageChance;
    int f8;
    int fc;
    int f10;
    int InterfaceType;
};

CCombatAbilityData* __fastcall CCombatAbilityData_operator_assign(CCombatAbilityData* self, int edx_dummy, const CCombatAbilityData* rhs) {
    self->PercentageChance  = rhs->PercentageChance;
    self->f8  = rhs->f8;
    self->fc  = rhs->fc;
    self->f10 = rhs->f10;
    self->InterfaceType = rhs->InterfaceType;
    return self;
}