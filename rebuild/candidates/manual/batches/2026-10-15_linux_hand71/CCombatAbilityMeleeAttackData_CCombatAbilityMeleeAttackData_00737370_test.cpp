#include <cstdio>

struct CCombatAbilityData {
    long PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; long Anim_TablePos; long InterfaceType;
    virtual ~CCombatAbilityData() { }
    virtual int Kind() const { return 1; }
};

struct CCombatAbilityMeleeAttackData : CCombatAbilityData {
    bool Blockable; bool Knockdown; float Damage;
    virtual int Kind() const { return 2; }
};

int main()
{
    CCombatAbilityMeleeAttackData a;
    a.PercentageChance = 40; a.SecondsDuration = 1.5f; a.SecondsToWaitBeforeRepeat = 2.25f; a.Anim_TablePos = 12;
    a.InterfaceType = 3; a.Blockable = true; a.Knockdown = false; a.Damage = 17.5f;
    CCombatAbilityMeleeAttackData b(a);
    const CCombatAbilityData& base = b;
    if (base.Kind() != 2 || b.PercentageChance != 40 || b.SecondsDuration != 1.5f || b.SecondsToWaitBeforeRepeat != 2.25f
        || b.Anim_TablePos != 12 || b.InterfaceType != 3 || !b.Blockable || b.Knockdown || b.Damage != 17.5f)
    { std::printf("00737370_TEST FAIL\n"); return 1; }
    std::printf("00737370_TEST PASS\n");
    return 0;
}
