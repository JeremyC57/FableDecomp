#include <cstdio>

static int g_base_called = 0;

struct CCombatAbilityBase {
    char pad[0x18];
    void assign(const CCombatAbilityBase* other) { (void)other; g_base_called = 1; }
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

int main()
{
    CCombatAbilityMeleeAttackData a, b;
    b.Blockable = 0x11; b.Knockdown = 0x22; b.d1c = 0x12345678;
    a.Blockable = 0; a.Knockdown = 0; a.d1c = 0;
    CCombatAbilityMeleeAttackData* r = CCombatAbilityMeleeAttackData_operator_assign(&a, 0, &b);
    if (r == &a && g_base_called == 1 && a.Blockable == 0x11 && a.Knockdown == 0x22 && a.d1c == 0x12345678) {
        std::printf("CCombatAbilityMeleeAttackData_00453ef1_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}