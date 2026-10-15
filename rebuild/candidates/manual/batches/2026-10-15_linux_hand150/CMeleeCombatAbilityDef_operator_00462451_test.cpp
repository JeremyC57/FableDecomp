// behaviour test for 0x00462451 CMeleeCombatAbilityDef::operator=
#include <cstdio>
static int g_copies = 0;
struct CParentDefClassBase {
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
struct CDefString { long m_index; };
struct CWeaponClassVector { long v; CWeaponClassVector& operator=(const CWeaponClassVector& o) { v = o.v; ++g_copies; return *this; } };
struct CHeroAbilityStatPackage { long v; CHeroAbilityStatPackage& operator=(const CHeroAbilityStatPackage& o) { v = o.v; g_copies += 10; return *this; } };
struct CMeleeCombatAbilityDef : CParentDefClassBase {
    CDefString FlourishAnimName, FlourishResponseAnimName;
    float DamageMultiplier; bool Flourish, BlockCounter; long AbilityButtonGraphicOverride;
    CWeaponClassVector ValidWeaponClasses; bool Decapitate; long KnockDownEffects; bool EvadeAllHits, CauseRecoil;
    CHeroAbilityStatPackage StatsNeeded; long ComboMultiplierLowerBound, ComboMultiplierUpperBound;
    CMeleeCombatAbilityDef& operator=(const CMeleeCombatAbilityDef& other)
    {
        CParentDefClassBase::operator=(other);
        FlourishAnimName = other.FlourishAnimName;
        FlourishResponseAnimName = other.FlourishResponseAnimName;
        DamageMultiplier = other.DamageMultiplier;
        Flourish = other.Flourish;
        BlockCounter = other.BlockCounter;
        AbilityButtonGraphicOverride = other.AbilityButtonGraphicOverride;
        ValidWeaponClasses = other.ValidWeaponClasses;
        Decapitate = other.Decapitate;
        KnockDownEffects = other.KnockDownEffects;
        EvadeAllHits = other.EvadeAllHits;
        CauseRecoil = other.CauseRecoil;
        StatsNeeded = other.StatsNeeded;
        ComboMultiplierLowerBound = other.ComboMultiplierLowerBound;
        ComboMultiplierUpperBound = other.ComboMultiplierUpperBound;
        return *this;
    }
};
int main()
{
    CMeleeCombatAbilityDef a, b;
    b.baseTag = 1; b.FlourishAnimName.m_index = 44; b.DamageMultiplier = 1.5f; b.Flourish = true; b.CauseRecoil = true;
    b.ValidWeaponClasses.v = 3; b.StatsNeeded.v = 8; b.ComboMultiplierUpperBound = 9;
    a.baseTag = 0; a.FlourishAnimName.m_index = 0; a.DamageMultiplier = 0; a.Flourish = false; a.CauseRecoil = false;
    a.ValidWeaponClasses.v = 0; a.StatsNeeded.v = 0; a.ComboMultiplierUpperBound = 0;
    CMeleeCombatAbilityDef* r = &(a = b);
    if (r != &a || a.baseTag != 1 || a.FlourishAnimName.m_index != 44 || a.DamageMultiplier != 1.5f || !a.Flourish
        || !a.CauseRecoil || a.ValidWeaponClasses.v != 3 || a.StatsNeeded.v != 8 || a.ComboMultiplierUpperBound != 9 || g_copies != 11)
    { std::printf("00462451_TEST FAIL\n"); return 1; }
    std::printf("00462451_TEST PASS\n");
    return 0;
}
