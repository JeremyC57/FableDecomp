// Behaviour test for 0044c503 CCombatAbilityAttackBase(long interfaceType) (copy of the model; the def base and
// the melee-data constructor are stand-ins with the landed constructors' field values).
#include <cstdio>

static int g_order[4];
static int g_n = 0;
class CShoveHaPennyRegionDef { public:
    virtual ~CShoveHaPennyRegionDef() {}
    unsigned char _body[0x24];
    CShoveHaPennyRegionDef() { g_order[g_n++] = 1; }
};
class CCombatAbilityData { public:
    virtual ~CCombatAbilityData() {}
    long  PercentageChance;
    float SecondsDuration;
    float SecondsToWaitBeforeRepeat;
    long  Anim;
    long  InterfaceType;
};
class CCombatAbilityMeleeAttackData : public CCombatAbilityData { public:
    bool  Blockable;
    bool  Knockdown;
    float Damage;
    CCombatAbilityMeleeAttackData(long interfaceType)
    {
        g_order[g_n++] = 2;
        PercentageChance = 100; SecondsDuration = -1.0f; SecondsToWaitBeforeRepeat = 0.0f; Anim = -1;
        InterfaceType = interfaceType; Blockable = true; Knockdown = false; Damage = 0.0f;
    }
};
class CCombatAbilityAttackBase : public CShoveHaPennyRegionDef { public:
    CCombatAbilityMeleeAttackData CombatAbility;
    CCombatAbilityAttackBase(long interfaceType);
};
CCombatAbilityAttackBase::CCombatAbilityAttackBase(long interfaceType)
    : CombatAbility(interfaceType)
{
}

int main()
{
    CCombatAbilityAttackBase a(6);
    int ok = 1;
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2) ok = 0;      // base first, then the member
    if (a.CombatAbility.InterfaceType != 6 || a.CombatAbility.PercentageChance != 100 || !a.CombatAbility.Blockable) ok = 0;
    CCombatAbilityAttackBase b(7);
    if (b.CombatAbility.InterfaceType != 7) ok = 0;
    if (!ok) { std::printf("0044c503_TEST FAIL\n"); return 1; }
    std::printf("0044c503_TEST PASS\n");
    return 0;
}