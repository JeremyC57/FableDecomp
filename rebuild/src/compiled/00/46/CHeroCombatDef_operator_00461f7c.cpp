// CHeroCombatDef::operator=(const CHeroCombatDef&) -- memberwise copy of the def: CParentDefClassBase part, then every CHeroCombatDef member in declaration order; returns *this
// Leaf: ??4CHeroCombatDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00461F7C.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, CHeroAbilityStatWeight::operator= @ 0x0046230E, CLongVector::operator= @ 0x00454886, CFloatVector::operator= @ 0x0045EF2D, CHeroAbilityStatPackage::operator= @ 0x004621E9.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CHeroCombatDef's
// vtable 0x01234CF4 is the 5-byte `jmp` at 0x00461F77 right before this body (rtti-vtables.tsv).
// Layout: PDB CHeroCombatDef members from +0x40 sit at retail +0x3c (retail CParentDefClassBase 0x3c bytes; retail
// STLport vectors/maps are 12 bytes where the PDB has 16). Names and order are the PDB's.
// Shape: compiler-generated memberwise assignment written out; scalars and small structs are copied inline,
// strings/containers through their out-of-line operator=.
#pragma optimize("s", on)

#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

class CFloatRange { public:
    float HighValue;
    float LowValue;
};

class CHeroAbilityStatWeight { public:         // retail 12 bytes: map<EHeroTrainableStatType,float>
    void* m_first;
    void* m_last;
    void* m_end;
    CHeroAbilityStatWeight& operator=(const CHeroAbilityStatWeight& other);   // 0x0046230E
};

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CFloatVector {                   // retail std::vector<float> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};

class CHeroAbilityStatPackage { public:        // retail 12 bytes: map<EHeroTrainableStatType,long>
    void* m_first;
    void* m_last;
    void* m_end;
    CHeroAbilityStatPackage& operator=(const CHeroAbilityStatPackage& other);   // 0x004621E9
};

class CHeroCombatDef : public CParentDefClassBase { public:// vtable 0x01234CF4
    CFloatRange             RangedAccuracySecondsForMaximumWithSkill;            // +0x3c
    CFloatRange             RangedAccuracyPercentFractionWithBuildup;            // +0x44
    CFloatRange             RangedAccuracyPercentWithStats;                      // +0x4c
    CHeroAbilityStatWeight  RangedAccuracyWeighting;                             // +0x54
    CFloatRange             RangedAccuracyShakeyCursor;                          // +0x60
    CHeroAbilityStatWeight  RangedAccuracyShakeyCursorWeighting;                 // +0x68
    float                   RangedAccuracyShakeFractionOfRadius;                 // +0x74
    CHeroAbilityStatWeight  RangedDamageMultiplierWeighting;                     // +0x78
    float                   RangedDamageMultiplierSpeedWithStatParameter;        // +0x84
    float                   RangedDamageMultiplierSpeedWithDistance;             // +0x88
    float                   RangedDamageMultiplierSpeedNominalBuildup;           // +0x8c
    float                   RangedDamageMultiplierScaleNominalBuildup;           // +0x90
    float                   RangedDamageMultiplierSpeedSuperBuildup;             // +0x94
    float                   RangedDamageMultiplierScaleSuperBuildup;             // +0x98
    CHeroAbilityStatWeight  RangedWeaponReloadSpeedWeighting;                    // +0x9c
    CFloatRange             MeleeDamageMultiplierWithStats;                      // +0xa8
    CHeroAbilityStatWeight  MeleeDamageWeighting;                                // +0xb0
    CFloatRange             HitDamageMultiplierWithStats;                        // +0xbc
    CHeroAbilityStatWeight  HitDamageWeighting;                                  // +0xc4
    bool                    MeleeFlourishValidOnMultiplierLevelIncrease;         // +0xd0
    long                    MeleeFlourishNumMultiplierLevelsPerFlourish;         // +0xd4
    bool                    MeleeFlourishAllowBreakingOfRotation;                // +0xd8
    float                   MeleeFlourishSecondsForAvailibility;                 // +0xdc
    bool                    MeleeFlourishOneShot;                                // +0xe0
    bool                    MeleeFlourishUsable;                                 // +0xe1
    bool                    MeleeCombatPlayStrikeSounds;                         // +0xe2
    float                   TargetingPressureFractionThresholdForToggle;         // +0xe4
    float                   MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;   // +0xe8
    long                    MeleeStrikeNumFramesPauseOnHeroStrike;               // +0xec
    CFloatRange             MeleeStrikeAnimationSpeedFractionWithStats;          // +0xf0
    CHeroAbilityStatWeight  MeleeStrikeAnimationSpeedWeighting;                  // +0xf8
    float                   AutoBlockSkillThreshold;                             // +0x104
    float                   AutoBlockStrengthThreshold;                          // +0x108
    float                   MeleeRecoilMaintainHandednessSkillThreshold;         // +0x10c
    float                   MeleeRecoilMaintainHandednessStrengthThreshold;      // +0x110
    float                   MeleeRecoilMaintainComboChainSkillThreshold;         // +0x114
    float                   MeleeRecoilMaintainComboChainStrengthThreshold;      // +0x118
    CLongVector             DefaultCombatAbilities;                              // +0x11c
    CFloatVector            ProjectileWeaponFirstPersonModeMaxFOVValues;         // +0x128
    bool                    ProjectileTargetingAcquireTargetTag;                 // +0x134
    bool                    ProjectileTargetingAcquireTargetsAuto;               // +0x135
    bool                    ProjectileTargetingUseAnalogueZoom;                  // +0x136
    float                   ProjectileWeaponAnalogueZoomControlWeight;           // +0x138
    CHeroAbilityStatPackage HeavyWeapons;                                        // +0x13c
    CHeroCombatDef& operator=(const CHeroCombatDef& other);
};

CHeroCombatDef& CHeroCombatDef::operator=(const CHeroCombatDef& other)
{
    CParentDefClassBase::operator=(other);
    RangedAccuracySecondsForMaximumWithSkill = other.RangedAccuracySecondsForMaximumWithSkill;
    RangedAccuracyPercentFractionWithBuildup = other.RangedAccuracyPercentFractionWithBuildup;
    RangedAccuracyPercentWithStats = other.RangedAccuracyPercentWithStats;
    RangedAccuracyWeighting = other.RangedAccuracyWeighting;
    RangedAccuracyShakeyCursor = other.RangedAccuracyShakeyCursor;
    RangedAccuracyShakeyCursorWeighting = other.RangedAccuracyShakeyCursorWeighting;
    RangedAccuracyShakeFractionOfRadius = other.RangedAccuracyShakeFractionOfRadius;
    RangedDamageMultiplierWeighting = other.RangedDamageMultiplierWeighting;
    RangedDamageMultiplierSpeedWithStatParameter = other.RangedDamageMultiplierSpeedWithStatParameter;
    RangedDamageMultiplierSpeedWithDistance = other.RangedDamageMultiplierSpeedWithDistance;
    RangedDamageMultiplierSpeedNominalBuildup = other.RangedDamageMultiplierSpeedNominalBuildup;
    RangedDamageMultiplierScaleNominalBuildup = other.RangedDamageMultiplierScaleNominalBuildup;
    RangedDamageMultiplierSpeedSuperBuildup = other.RangedDamageMultiplierSpeedSuperBuildup;
    RangedDamageMultiplierScaleSuperBuildup = other.RangedDamageMultiplierScaleSuperBuildup;
    RangedWeaponReloadSpeedWeighting = other.RangedWeaponReloadSpeedWeighting;
    MeleeDamageMultiplierWithStats = other.MeleeDamageMultiplierWithStats;
    MeleeDamageWeighting = other.MeleeDamageWeighting;
    HitDamageMultiplierWithStats = other.HitDamageMultiplierWithStats;
    HitDamageWeighting = other.HitDamageWeighting;
    MeleeFlourishValidOnMultiplierLevelIncrease = other.MeleeFlourishValidOnMultiplierLevelIncrease;
    MeleeFlourishNumMultiplierLevelsPerFlourish = other.MeleeFlourishNumMultiplierLevelsPerFlourish;
    MeleeFlourishAllowBreakingOfRotation = other.MeleeFlourishAllowBreakingOfRotation;
    MeleeFlourishSecondsForAvailibility = other.MeleeFlourishSecondsForAvailibility;
    MeleeFlourishOneShot = other.MeleeFlourishOneShot;
    MeleeFlourishUsable = other.MeleeFlourishUsable;
    MeleeCombatPlayStrikeSounds = other.MeleeCombatPlayStrikeSounds;
    TargetingPressureFractionThresholdForToggle = other.TargetingPressureFractionThresholdForToggle;
    MaxAngleToTurnToShootAtTargetInThirdPersonBowMode = other.MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;
    MeleeStrikeNumFramesPauseOnHeroStrike = other.MeleeStrikeNumFramesPauseOnHeroStrike;
    MeleeStrikeAnimationSpeedFractionWithStats = other.MeleeStrikeAnimationSpeedFractionWithStats;
    MeleeStrikeAnimationSpeedWeighting = other.MeleeStrikeAnimationSpeedWeighting;
    AutoBlockSkillThreshold = other.AutoBlockSkillThreshold;
    AutoBlockStrengthThreshold = other.AutoBlockStrengthThreshold;
    MeleeRecoilMaintainHandednessSkillThreshold = other.MeleeRecoilMaintainHandednessSkillThreshold;
    MeleeRecoilMaintainHandednessStrengthThreshold = other.MeleeRecoilMaintainHandednessStrengthThreshold;
    MeleeRecoilMaintainComboChainSkillThreshold = other.MeleeRecoilMaintainComboChainSkillThreshold;
    MeleeRecoilMaintainComboChainStrengthThreshold = other.MeleeRecoilMaintainComboChainStrengthThreshold;
    DefaultCombatAbilities = other.DefaultCombatAbilities;
    ProjectileWeaponFirstPersonModeMaxFOVValues = other.ProjectileWeaponFirstPersonModeMaxFOVValues;
    ProjectileTargetingAcquireTargetTag = other.ProjectileTargetingAcquireTargetTag;
    ProjectileTargetingAcquireTargetsAuto = other.ProjectileTargetingAcquireTargetsAuto;
    ProjectileTargetingUseAnalogueZoom = other.ProjectileTargetingUseAnalogueZoom;
    ProjectileWeaponAnalogueZoomControlWeight = other.ProjectileWeaponAnalogueZoomControlWeight;
    HeavyWeapons = other.HeavyWeapons;
    return *this;
}