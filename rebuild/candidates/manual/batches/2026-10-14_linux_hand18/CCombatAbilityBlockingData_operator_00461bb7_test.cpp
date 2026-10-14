#include <cstdio>
#include <stddef.h>
static int g_setcopies = 0; static int g_setassign = 0;
struct CCombatAbilityData_Retail {
    virtual void v00() { }
    long PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; long Anim_TablePos; long InterfaceType;
    CCombatAbilityData_Retail& operator=(const CCombatAbilityData_Retail& o) { PercentageChance = o.PercentageChance; SecondsDuration = o.SecondsDuration; SecondsToWaitBeforeRepeat = o.SecondsToWaitBeforeRepeat; Anim_TablePos = o.Anim_TablePos; InterfaceType = o.InterfaceType; return *this; }
};
struct CRetailSet_EWeaponClass {
    void* head; unsigned long size; unsigned long comp;
    CRetailSet_EWeaponClass() : head(0), size(0), comp(0) { }
    CRetailSet_EWeaponClass(const CRetailSet_EWeaponClass& o) : head(o.head), size(o.size), comp(o.comp) { ++g_setcopies; }
    CRetailSet_EWeaponClass& operator=(const CRetailSet_EWeaponClass& o) { head = o.head; size = o.size; comp = o.comp; ++g_setassign; return *this; }
};
struct CCombatAbilityBlockingData_Retail : CCombatAbilityData_Retail {
    float BlockAngle; bool BlockCanBeBroken; bool ForceRecoilWhenBlocking; CRetailSet_EWeaponClass ValidBlockWeaponTypes; long WeaponClass;
    virtual void v00() { }
    CCombatAbilityBlockingData_Retail() { }
    CCombatAbilityBlockingData_Retail(const CCombatAbilityBlockingData_Retail& o) : CCombatAbilityData_Retail(o), BlockAngle(o.BlockAngle), BlockCanBeBroken(o.BlockCanBeBroken), ForceRecoilWhenBlocking(o.ForceRecoilWhenBlocking), ValidBlockWeaponTypes(o.ValidBlockWeaponTypes), WeaponClass(o.WeaponClass) { }
    CCombatAbilityBlockingData_Retail& operator=(const CCombatAbilityBlockingData_Retail& o) { CCombatAbilityData_Retail::operator=(o); BlockAngle = o.BlockAngle; BlockCanBeBroken = o.BlockCanBeBroken; ForceRecoilWhenBlocking = o.ForceRecoilWhenBlocking; ValidBlockWeaponTypes = o.ValidBlockWeaponTypes; WeaponClass = o.WeaponClass; return *this; }
};
static void Fill(CCombatAbilityBlockingData_Retail& a)
{
    a.PercentageChance = 42; a.SecondsDuration = 1.5f; a.SecondsToWaitBeforeRepeat = 2.5f; a.Anim_TablePos = 77; a.InterfaceType = 3;
    a.BlockAngle = 120.0f; a.BlockCanBeBroken = true; a.ForceRecoilWhenBlocking = false; a.ValidBlockWeaponTypes.head = (void*)0x1234; a.ValidBlockWeaponTypes.size = 2; a.WeaponClass = 5;
}
static bool Same(const CCombatAbilityBlockingData_Retail& a, const CCombatAbilityBlockingData_Retail& b)
{
    return a.PercentageChance == b.PercentageChance && a.SecondsDuration == b.SecondsDuration && a.SecondsToWaitBeforeRepeat == b.SecondsToWaitBeforeRepeat
        && a.Anim_TablePos == b.Anim_TablePos && a.InterfaceType == b.InterfaceType && a.BlockAngle == b.BlockAngle && a.BlockCanBeBroken == b.BlockCanBeBroken
        && a.ForceRecoilWhenBlocking == b.ForceRecoilWhenBlocking && a.ValidBlockWeaponTypes.head == b.ValidBlockWeaponTypes.head && a.ValidBlockWeaponTypes.size == b.ValidBlockWeaponTypes.size
        && a.WeaponClass == b.WeaponClass && offsetof(CCombatAbilityBlockingData_Retail, BlockAngle) == 0x18 && offsetof(CCombatAbilityBlockingData_Retail, ValidBlockWeaponTypes) == 0x20
        && offsetof(CCombatAbilityBlockingData_Retail, WeaponClass) == 0x2c && sizeof(a) == 0x30;
}
int main()
{
    CCombatAbilityBlockingData_Retail a; Fill(a);
    CCombatAbilityBlockingData_Retail b; b.PercentageChance = 0; b.BlockAngle = 0.0f; b.WeaponClass = 0; b.BlockCanBeBroken = false; b.ForceRecoilWhenBlocking = true;
    CCombatAbilityBlockingData_Retail& r = (b = a);
    if (&r != &b || !Same(a, b) || g_setassign != 1 || g_setcopies != 0) { std::printf("00461bb7_TEST FAIL\n"); return 1; }
    std::printf("00461bb7_TEST PASS\n"); return 0;
}
