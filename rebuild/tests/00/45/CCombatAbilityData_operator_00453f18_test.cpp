#include <cstdio>

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

int main() {
    CCombatAbilityData a, b;
    a.f0 = 111; a.PercentageChance = 1; a.f8 = 2; a.fc = 3; a.f10 = 4; a.InterfaceType = 5;
    b.f0 = 999; b.PercentageChance = 0; b.f8 = 0; b.fc = 0; b.f10 = 0; b.InterfaceType = 0;
    CCombatAbilityData* r = CCombatAbilityData_operator_assign(&b, 0, &a);
    if (r != &b) { std::printf("FAIL: return\n"); return 1; }
    if (b.PercentageChance != 1 || b.f8 != 2 || b.fc != 3 || b.f10 != 4 || b.InterfaceType != 5) { std::printf("FAIL: copy\n"); return 1; }
    if (b.f0 != 999) { std::printf("FAIL: f0 clobbered\n"); return 1; }
    std::printf("CCombatAbilityData_00453f18_TEST PASS\n");
    return 0;
}