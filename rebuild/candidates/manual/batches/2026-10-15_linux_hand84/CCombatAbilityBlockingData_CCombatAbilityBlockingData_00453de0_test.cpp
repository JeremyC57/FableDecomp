#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
struct CCombatAbilityData_Calls {
    virtual void v00() { }
    long PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; long Anim; long InterfaceType;
    CCombatAbilityData_Calls(long interfaceType) : PercentageChance(100), SecondsDuration(-1.0f), SecondsToWaitBeforeRepeat(0.0f), Anim(-1), InterfaceType(interfaceType) { note("A"); }
};
struct CRetailSet_WeaponClass { void* Header; unsigned long Count; unsigned char Compare[4]; CRetailSet_WeaponClass() : Header(0), Count(0) { note("S"); } };
struct CCombatAbilityBlockingData : CCombatAbilityData_Calls {
    float BlockAngle; bool BlockCanBeBroken; bool ForceRecoilWhenBlocking; CRetailSet_WeaponClass ValidBlockWeaponTypes; long WeaponClass;
    virtual void v00() { }
    CCombatAbilityBlockingData(long interfaceType, long weaponClass)
        : CCombatAbilityData_Calls(interfaceType), BlockAngle(120.0f), BlockCanBeBroken(true), ForceRecoilWhenBlocking(true), ValidBlockWeaponTypes(), WeaponClass(weaponClass) { }
};
int main()
{
    CCombatAbilityBlockingData* b = new CCombatAbilityBlockingData(7, 3);
    if (std::strcmp(g_seq, "AS") || b->InterfaceType != 7 || b->WeaponClass != 3 || b->BlockAngle != 120.0f || !b->BlockCanBeBroken
        || !b->ForceRecoilWhenBlocking || b->ValidBlockWeaponTypes.Count != 0) { std::printf("00453de0_TEST FAIL\n"); return 1; }
    delete b;
    std::printf("00453de0_TEST PASS\n");
    return 0;
}
