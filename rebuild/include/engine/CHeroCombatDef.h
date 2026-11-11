#ifndef FABLE_ENGINE_CHEROCOMBATDEF_H
#define FABLE_ENGINE_CHEROCOMBATDEF_H
#ifdef FABLE_RETAIL_FIRE_RANGED_WEAPON
#include "engine/CParentDefClassBase.h"
#include "engine/CFloatRange.h"
#include "engine/CHeroAbilityStatWeight.h"
#include "engine/CHeroAbilityStatPackage.h"
#include "stlport/fable_containers.h"
class CHeroCombatDef:public CParentDefClassBase {
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 CFloatRange RangedAccuracySecondsForMaximumWithSkill;
 CFloatRange RangedAccuracyPercentFractionWithBuildup;
 CFloatRange RangedAccuracyPercentWithStats;
 CHeroAbilityStatWeight RangedAccuracyWeighting;
 CFloatRange RangedAccuracyShakeyCursor;
 CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting;
 float RangedAccuracyShakeFractionOfRadius;
 CHeroAbilityStatWeight RangedDamageMultiplierWeighting;
 float RangedDamageMultiplierSpeedWithStatParameter;
 float RangedDamageMultiplierSpeedWithDistance;
 float RangedDamageMultiplierSpeedNominalBuildup;
 float RangedDamageMultiplierScaleNominalBuildup;
 float RangedDamageMultiplierSpeedSuperBuildup;
 float RangedDamageMultiplierScaleSuperBuildup;
 CHeroAbilityStatWeight RangedWeaponReloadSpeedWeighting;
 CFloatRange MeleeDamageMultiplierWithStats;
 CHeroAbilityStatWeight MeleeDamageWeighting;
 CFloatRange HitDamageMultiplierWithStats;
 CHeroAbilityStatWeight HitDamageWeighting;
 bool MeleeFlourishValidOnMultiplierLevelIncrease;
 long MeleeFlourishNumMultiplierLevelsPerFlourish;
 bool MeleeFlourishAllowBreakingOfRotation;
 float MeleeFlourishSecondsForAvailibility;
 bool MeleeFlourishOneShot;
 bool MeleeFlourishUsable;
 bool MeleeCombatPlayStrikeSounds;
 float TargetingPressureFractionThresholdForToggle;
 float MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;
 long MeleeStrikeNumFramesPauseOnHeroStrike;
 CFloatRange MeleeStrikeAnimationSpeedFractionWithStats;
 CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting;
 float AutoBlockSkillThreshold;
 float AutoBlockStrengthThreshold;
 float MeleeRecoilMaintainHandednessSkillThreshold;
 float MeleeRecoilMaintainHandednessStrengthThreshold;
 float MeleeRecoilMaintainComboChainSkillThreshold;
 float MeleeRecoilMaintainComboChainStrengthThreshold;
 std::vector<long,std::allocator<long> > DefaultCombatAbilities;
 std::vector<float,std::allocator<float> > ProjectileWeaponFirstPersonModeMaxFOVValues;
 bool ProjectileTargetingAcquireTargetTag;
 bool ProjectileTargetingAcquireTargetsAuto;
 bool ProjectileTargetingUseAnalogueZoom;
 float ProjectileWeaponAnalogueZoomControlWeight;
 CHeroAbilityStatPackage HeavyWeapons;
 virtual void Transfer(CPersistContext & p0);
 CHeroCombatDef(const CHeroCombatDef & p0);
 CHeroCombatDef();
 virtual ~CHeroCombatDef();
 CHeroCombatDef & operator=(const CHeroCombatDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef)==328);
#else
#include "engine/CParentDefClassBase.h"
// Raw TPI0x14e2c CParentDefClassBase; original MeleeStrikeNumFramesPauseOnHeroStrike
// is donor108, nativeec after retail weight/container reductions; native00667feb/00668055.
// All untouched original members through HeavyWeapons remain at their complete extent.
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_RANGED
// Nested opt-in (chunk 777, row 0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk): the ranged-accuracy shake
// members that row reads, Ego_r.pdb names and types. Retail offsets: the first member (RangedAccuracySecondsForMaximumWithSkill,
// Ego_r +0x40) sits at +0x3C and each CHeroAbilityStatWeight (a map) is 12 bytes, so RangedAccuracyPercentWithStats +0x4C /
// RangedAccuracyWeighting +0x54 (landed 0x006D6EA0) and RangedAccuracyShakeyCursor +0x60 (Ego_r +0x68),
// RangedAccuracyShakeyCursorWeighting +0x68 (Ego_r +0x70), RangedAccuracyShakeFractionOfRadius +0x74 (Ego_r +0x80); retail
// 0x008036E9 / 0x008036F1 / 0x0080370A. The padding keeps MeleeStrikeNumFramesPauseOnHeroStrike at +0xEC and sizeof 0x148.
#include "engine/CFloatRange.h"
#include "engine/CHeroAbilityStatWeight.h"
#endif
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
// Nested opt-in (chunk 1004, row 0x005C90FC CTCInventoryWeapons::ConstructWeaponDescriptionContainer): Ego_r.pdb
// CHeroAbilityStatPackage HeavyWeapons, the last member (Ego_r +0x164; retail +0x13C: the row walks the stat-threshold map's header at
// [def+13Ch], 0x005C9769). The padding before it keeps sizeof 0x148.
#include "engine/CHeroAbilityStatPackage.h"
#endif
class CHeroCombatDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_RANGED
    unsigned char _pad_0x39[0x60 - 0x39];
    CFloatRange RangedAccuracyShakeyCursor;                     // +0x60
    CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting; // +0x68
    float RangedAccuracyShakeFractionOfRadius;                  // +0x74
    unsigned char _pad_0x78[0xec - 0x78];
#else
    unsigned char _pad_0x39[0xb3];
#endif
    long MeleeStrikeNumFramesPauseOnHeroStrike;
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_BLOCK_THRESHOLDS
    unsigned char _pad_0xf0[0x14];
    float AutoBlockSkillThreshold,AutoBlockStrengthThreshold;
    float MeleeRecoilMaintainHandednessSkillThreshold,MeleeRecoilMaintainHandednessStrengthThreshold;
    float MeleeRecoilMaintainComboChainSkillThreshold,MeleeRecoilMaintainComboChainStrengthThreshold;
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
    unsigned char _pad_0x11c[0x20];
    CHeroAbilityStatPackage HeavyWeapons;                       // +0x13C
#else
    unsigned char _pad_0x11c[0x2c];
#endif
#else
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
    unsigned char _pad_0xf0[0x4c];
    CHeroAbilityStatPackage HeavyWeapons;                       // +0x13C
#else
    unsigned char _pad_0xf0[0x58];
#endif
#endif
};
FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef)==0x148);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,MeleeStrikeNumFramesPauseOnHeroStrike)==0xec);
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,HeavyWeapons)==0x13c);
#endif
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_RANGED
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,RangedAccuracyShakeyCursor)==0x60);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,RangedAccuracyShakeFractionOfRadius)==0x74);
#endif

#endif
#endif

