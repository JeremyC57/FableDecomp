#pragma optimize("s",on)
struct CCombatAbilityBase {
    char pad[0x18];
    void assign(const CCombatAbilityBase* other);
};

class CCombatAbilityMeleeAttackData : public CCombatAbilityBase { public:
    unsigned char Blockable;
    unsigned char Knockdown;
    char pad2[2];
    int d1c;
};

CCombatAbilityMeleeAttackData* __fastcall CCombatAbilityMeleeAttackData_operator_assign(
    CCombatAbilityMeleeAttackData* attack, void* dummy, const CCombatAbilityMeleeAttackData* other)
{
    attack->assign(other);
    attack->Blockable = other->Blockable;
    attack->Knockdown = other->Knockdown;
    attack->d1c = other->d1c;
    return attack;
}