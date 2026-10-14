#include <cstdio>
#include <stddef.h>
#include <new>
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
struct CCombatAbilityData_Retail {
    virtual void v00() { }
    long PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; CDefString Anim; long InterfaceType;
    CCombatAbilityData_Retail(long t) : PercentageChance(100), SecondsDuration(-1.0f), SecondsToWaitBeforeRepeat(0.0f), Anim(), InterfaceType(t) { }
};
int main()
{
    unsigned char raw[sizeof(CCombatAbilityData_Retail)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CCombatAbilityData_Retail* d = new (raw) CCombatAbilityData_Retail(6);
    if (d->PercentageChance != 100 || d->SecondsDuration != -1.0f || d->SecondsToWaitBeforeRepeat != 0.0f || d->Anim.TablePos != -1 || d->InterfaceType != 6
        || offsetof(CCombatAbilityData_Retail, SecondsToWaitBeforeRepeat) != 0xc || offsetof(CCombatAbilityData_Retail, InterfaceType) != 0x14 || sizeof(*d) != 0x18) { std::printf("0044c3ee_TEST FAIL\n"); return 1; }
    std::printf("0044c3ee_TEST PASS\n"); return 0;
}
