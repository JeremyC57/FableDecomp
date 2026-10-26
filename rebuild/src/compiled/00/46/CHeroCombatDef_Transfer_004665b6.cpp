// CHeroCombatDef::Transfer(CPersistContext& context) -- persist the hero combat definition: ranged accuracy / damage ranges and stat weightings, melee and hit damage, flourish settings, targeting, auto-block thresholds, default abilities, heavy weapons package and bow FOV values
// Leaf: ?Transfer@CHeroCombatDef
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004665B6, length 605 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers; the catalog labels on the shared ones are propagated): CFloatRange 0x00458F2B,
//   CHeroAbilityStatWeight 0x00466813, float 0x00431061, bool 0x0043314A, long 0x00431102, vector<long> 0x00466A47,
//   CHeroAbilityStatPackage 0x00463CA6, vector<float> 0x00464CBF.
// Layout: Ego_r.pdb CHeroCombatDef (0x174) re-based on the retail bytes: the def base is 0x3c bytes (PDB 0x40), each
//   CHeroAbilityStatWeight is 12 bytes and each STLport vector 12 bytes (16 in the PDB), so RangedAccuracySecondsForMaximumWithSkill
//   sits at +0x3c and HeavyWeapons at +0x13c. No generated engine header for the class.
// Pragma: #pragma optimize("s", on) -- a size TU: every member address goes through eax.
// Shape: one transfer per member in declaration order, except that HeavyWeapons is transferred straight after
//   DefaultCombatAbilities, ahead of ProjectileWeaponFirstPersonModeMaxFOVValues (retail order 0x11c, 0x13c, 0x128).
class CFloatRange { public: float Min; float Max; };
class CHeroAbilityStatWeight { public: float Weights[3]; };
class CHeroAbilityStatPackage { public: unsigned char _body[0x10]; };
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
struct CFloatVector { float* _M_start; float* _M_finish; float* _M_end_of_storage; };

class CPersistContext { public:
    void Transfer(CFloatRange& value);                  // 0x00458F2B
    void Transfer(CHeroAbilityStatWeight& value);       // 0x00466813
    void Transfer(float& value);                        // 0x00431061
    void Transfer(bool& value);                         // 0x0043314A
    void Transfer(long& value);                         // 0x00431102
    void Transfer(CLongVector& value);                  // 0x00466A47
    void Transfer(CHeroAbilityStatPackage& value);      // 0x00463CA6
    void Transfer(CFloatVector& value);                 // 0x00464CBF
};

class CDefClassBase { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x3c - 4];
};

class CHeroCombatDef : public CDefClassBase { public:
    CFloatRange RangedAccuracySecondsForMaximumWithSkill;              // +0x3c
    CFloatRange RangedAccuracyPercentFractionWithBuildup;              // +0x44
    CFloatRange RangedAccuracyPercentWithStats;                        // +0x4c
    CHeroAbilityStatWeight RangedAccuracyWeighting;                    // +0x54
    CFloatRange RangedAccuracyShakeyCursor;                            // +0x60
    CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting;        // +0x68
    float RangedAccuracyShakeFractionOfRadius;                         // +0x74
    CHeroAbilityStatWeight RangedDamageMultiplierWeighting;            // +0x78
    float RangedDamageMultiplierSpeedWithStatParameter;                // +0x84
    float RangedDamageMultiplierSpeedWithDistance;                     // +0x88
    float RangedDamageMultiplierSpeedNominalBuildup;                   // +0x8c
    float RangedDamageMultiplierScaleNominalBuildup;                   // +0x90
    float RangedDamageMultiplierSpeedSuperBuildup;                     // +0x94
    float RangedDamageMultiplierScaleSuperBuildup;                     // +0x98
    CHeroAbilityStatWeight RangedWeaponReloadSpeedWeighting;           // +0x9c
    CFloatRange MeleeDamageMultiplierWithStats;                        // +0xa8
    CHeroAbilityStatWeight MeleeDamageWeighting;                       // +0xb0
    CFloatRange HitDamageMultiplierWithStats;                          // +0xbc
    CHeroAbilityStatWeight HitDamageWeighting;                         // +0xc4
    bool  MeleeFlourishValidOnMultiplierLevelIncrease;                 // +0xd0
    long  MeleeFlourishNumMultiplierLevelsPerFlourish;                 // +0xd4
    bool  MeleeFlourishAllowBreakingOfRotation;                        // +0xd8
    float MeleeFlourishSecondsForAvailibility;                         // +0xdc
    bool  MeleeFlourishOneShot;                                        // +0xe0
    bool  MeleeFlourishUsable;                                         // +0xe1
    bool  MeleeCombatPlayStrikeSounds;                                 // +0xe2
    float TargetingPressureFractionThresholdForToggle;                 // +0xe4
    float MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;           // +0xe8
    long  MeleeStrikeNumFramesPauseOnHeroStrike;                       // +0xec
    CFloatRange MeleeStrikeAnimationSpeedFractionWithStats;            // +0xf0
    CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting;         // +0xf8
    float AutoBlockSkillThreshold;                                     // +0x104
    float AutoBlockStrengthThreshold;                                  // +0x108
    float MeleeRecoilMaintainHandednessSkillThreshold;                 // +0x10c
    float MeleeRecoilMaintainHandednessStrengthThreshold;              // +0x110
    float MeleeRecoilMaintainComboChainSkillThreshold;                 // +0x114
    float MeleeRecoilMaintainComboChainStrengthThreshold;              // +0x118
    CLongVector DefaultCombatAbilities;                                // +0x11c
    CFloatVector ProjectileWeaponFirstPersonModeMaxFOVValues;          // +0x128
    bool  ProjectileTargetingAcquireTargetTag;                         // +0x134
    bool  ProjectileTargetingAcquireTargetsAuto;                       // +0x135
    bool  ProjectileTargetingUseAnalogueZoom;                          // +0x136
    float ProjectileWeaponAnalogueZoomControlWeight;                   // +0x138
    CHeroAbilityStatPackage HeavyWeapons;                              // +0x13c
    virtual void Transfer(CPersistContext& context);
};

void CHeroCombatDef::Transfer(CPersistContext& context)
{
    context.Transfer(RangedAccuracySecondsForMaximumWithSkill);
    context.Transfer(RangedAccuracyPercentFractionWithBuildup);
    context.Transfer(RangedAccuracyPercentWithStats);
    context.Transfer(RangedAccuracyWeighting);
    context.Transfer(RangedAccuracyShakeyCursor);
    context.Transfer(RangedAccuracyShakeyCursorWeighting);
    context.Transfer(RangedAccuracyShakeFractionOfRadius);
    context.Transfer(RangedDamageMultiplierWeighting);
    context.Transfer(RangedDamageMultiplierSpeedWithStatParameter);
    context.Transfer(RangedDamageMultiplierSpeedWithDistance);
    context.Transfer(RangedDamageMultiplierSpeedNominalBuildup);
    context.Transfer(RangedDamageMultiplierScaleNominalBuildup);
    context.Transfer(RangedDamageMultiplierSpeedSuperBuildup);
    context.Transfer(RangedDamageMultiplierScaleSuperBuildup);
    context.Transfer(RangedWeaponReloadSpeedWeighting);
    context.Transfer(MeleeDamageMultiplierWithStats);
    context.Transfer(MeleeDamageWeighting);
    context.Transfer(HitDamageMultiplierWithStats);
    context.Transfer(HitDamageWeighting);
    context.Transfer(MeleeFlourishValidOnMultiplierLevelIncrease);
    context.Transfer(MeleeFlourishNumMultiplierLevelsPerFlourish);
    context.Transfer(MeleeFlourishAllowBreakingOfRotation);
    context.Transfer(MeleeFlourishSecondsForAvailibility);
    context.Transfer(MeleeFlourishOneShot);
    context.Transfer(MeleeFlourishUsable);
    context.Transfer(MeleeCombatPlayStrikeSounds);
    context.Transfer(TargetingPressureFractionThresholdForToggle);
    context.Transfer(MaxAngleToTurnToShootAtTargetInThirdPersonBowMode);
    context.Transfer(MeleeStrikeNumFramesPauseOnHeroStrike);
    context.Transfer(MeleeStrikeAnimationSpeedFractionWithStats);
    context.Transfer(MeleeStrikeAnimationSpeedWeighting);
    context.Transfer(AutoBlockSkillThreshold);
    context.Transfer(AutoBlockStrengthThreshold);
    context.Transfer(MeleeRecoilMaintainHandednessSkillThreshold);
    context.Transfer(MeleeRecoilMaintainHandednessStrengthThreshold);
    context.Transfer(MeleeRecoilMaintainComboChainSkillThreshold);
    context.Transfer(MeleeRecoilMaintainComboChainStrengthThreshold);
    context.Transfer(DefaultCombatAbilities);
    context.Transfer(HeavyWeapons);
    context.Transfer(ProjectileWeaponFirstPersonModeMaxFOVValues);
    context.Transfer(ProjectileTargetingAcquireTargetTag);
    context.Transfer(ProjectileTargetingAcquireTargetsAuto);
    context.Transfer(ProjectileTargetingUseAnalogueZoom);
    context.Transfer(ProjectileWeaponAnalogueZoomControlWeight);
}