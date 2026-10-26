#include <cstdio>
#include <cstring>

class CCombatAbilityData { public:
    long PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; long Anim; long InterfaceType;
    CCombatAbilityData(long type) : PercentageChance(100), SecondsDuration(-1.0f), SecondsToWaitBeforeRepeat(0.0f), Anim(-1), InterfaceType(type) { }
};
class CCombatAbilityMeleeAttackData : public CCombatAbilityData { public:
    bool Blockable, Knockdown; float Damage;
    CCombatAbilityMeleeAttackData(long type) : CCombatAbilityData(type), Blockable(true), Knockdown(false), Damage(0.0f) { }
};
int main()
{
    CCombatAbilityMeleeAttackData d(6);
    if (d.InterfaceType != 6 || d.PercentageChance != 100 || !d.Blockable || d.Knockdown || d.Damage != 0.0f) { std::printf("0044c47b_TEST FAIL\n"); return 1; }
    std::printf("0044c47b_TEST PASS\n");
    return 0;
}