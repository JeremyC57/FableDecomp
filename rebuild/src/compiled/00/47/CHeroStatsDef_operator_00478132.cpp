// CHeroStatsDef::operator=(const CHeroStatsDef&) -- memberwise copy: CParentDefClassBase part, stamina/health/money/age stats, the appearance-scaling, renown and per-level vectors; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478132, length 507 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every scalar copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<CAppearanceModifierScalingDef>::operator= @ 0x0047832D,
//          vector<long>::operator= @ 0x00454886, vector<float>::operator= @ 0x0045EF2D, vector<CAbilityTrackDef>::operator= @ 0x0047857D.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CHeroStatsDef's
// vtable 0x012339DC is the 5-byte `jmp 0x00478132` right before this body; the PDB members (+0x40..) sit at
// retail +0x3c.. (retail CParentDefClassBase is 0x39 bytes and its vectors are 12 bytes, not the PDB 16).
// Shape: the compiler-style memberwise assignment written out -- base part first (out of line), then each
// member in declaration order: containers through their out-of-line operator=, scalars as eax/al moves.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the retail
// base is modelled locally as a packed polymorphic base exactly as the landed sibling 0x00460256 does.
// NF48 (round 7): the member vectors are STLport 4.6.2 std::vector<float> / <long> / <CAppearanceModifierScalingDef> /
// <CAbilityTrackDef> (Ego_r.pdb CHeroStatsDef member types) from the shared stlport/fable_containers.h (local stand-ins before);
// FABLE_RETAIL_STLP_VECTOR_ASSIGN declares the out-of-line operator= (0x0045EF2D / 0x00454886 / 0x0047832D / 0x0047857D).
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
class CAppearanceModifierScalingDef;
class CAbilityTrackDef;
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)


class CHeroStatsDef : public CParentDefClassBase { public:          // vtable 0x012339DC
    long MaximumStamina;
    float NumberOfSecondsAfterUseStaminaStartsRefilling;
    float NumberOfSecondsAfterChangeHealthStartsRefilling;
    float NumberOfSecondsBetweenHealthRefills;
    long StaminaToRegainPerSecond;
    long HealthToRegainPerRefill;
    long RenownLossOnDeath;
    long MoneyLossOnDeath;
    long MaximumMoney;
    long MaximumExpToSpend;
    long ExpToSpendAtWhichToDisplayTutorial;
    float MinAge;
    float MaxAge;
    std::vector<CAppearanceModifierScalingDef> AppearanceModifierScaling;
    float AmountFatnessIncreasesPerPointOfOverEating;
    float NumberOfMinutesBetweenFatnessReductions;
    float AmountFatnessDecreasesPerReduction;
    float DrinkAffectMultiplierMultiplier;
    long MinVomitTime;
    long MaxVomitTime;
    long MinTimeBeforeVomiting;
    long MaxTimeBeforeVomiting;
    float PintsOfSobrietyPerSecondOfPuke;
    float PintsToDrunk;
    float PintsToVomit;
    float AlcoholAbsorbtionPerSecond;
    std::vector<long> RenownLevelCosts;
    std::vector<long> RenownLevelMaxFollowers;
    std::vector<long> RenownLevelMaxBoastingCrowd;
    long MaxMorality;
    long MaxMoralityAsChild;
    float MaxExtraMaxHitPoints;
    long MaxExtraMaxStaminaPoints;
    long MoralityChangeOnTheftFromHome;
    long MoralityChangeOnTheftFromShop;
    long MoralityChangeOnPicklock;
    long MoralityChangeOnVandalism;
    std::vector<float> TradeSkillForGuileLevel;
    std::vector<CAbilityTrackDef> StatLevels;
    std::vector<float> MaxHitPointsPerHealthLevel;
    std::vector<long> MaxStaminaPointsPerMagicPowerLevel;
    std::vector<float> SoundDistanceMultiplierPerStealthLevel;
    std::vector<float> VisibilityMultiplierPerStealthLevel;
    CHeroStatsDef& operator=(const CHeroStatsDef& other);
};

CHeroStatsDef& CHeroStatsDef::operator=(const CHeroStatsDef& other)
{
    CParentDefClassBase::operator=(other);
    MaximumStamina = other.MaximumStamina;
    NumberOfSecondsAfterUseStaminaStartsRefilling = other.NumberOfSecondsAfterUseStaminaStartsRefilling;
    NumberOfSecondsAfterChangeHealthStartsRefilling = other.NumberOfSecondsAfterChangeHealthStartsRefilling;
    NumberOfSecondsBetweenHealthRefills = other.NumberOfSecondsBetweenHealthRefills;
    StaminaToRegainPerSecond = other.StaminaToRegainPerSecond;
    HealthToRegainPerRefill = other.HealthToRegainPerRefill;
    RenownLossOnDeath = other.RenownLossOnDeath;
    MoneyLossOnDeath = other.MoneyLossOnDeath;
    MaximumMoney = other.MaximumMoney;
    MaximumExpToSpend = other.MaximumExpToSpend;
    ExpToSpendAtWhichToDisplayTutorial = other.ExpToSpendAtWhichToDisplayTutorial;
    MinAge = other.MinAge;
    MaxAge = other.MaxAge;
    AppearanceModifierScaling = other.AppearanceModifierScaling;
    AmountFatnessIncreasesPerPointOfOverEating = other.AmountFatnessIncreasesPerPointOfOverEating;
    NumberOfMinutesBetweenFatnessReductions = other.NumberOfMinutesBetweenFatnessReductions;
    AmountFatnessDecreasesPerReduction = other.AmountFatnessDecreasesPerReduction;
    DrinkAffectMultiplierMultiplier = other.DrinkAffectMultiplierMultiplier;
    MinVomitTime = other.MinVomitTime;
    MaxVomitTime = other.MaxVomitTime;
    MinTimeBeforeVomiting = other.MinTimeBeforeVomiting;
    MaxTimeBeforeVomiting = other.MaxTimeBeforeVomiting;
    PintsOfSobrietyPerSecondOfPuke = other.PintsOfSobrietyPerSecondOfPuke;
    PintsToDrunk = other.PintsToDrunk;
    PintsToVomit = other.PintsToVomit;
    AlcoholAbsorbtionPerSecond = other.AlcoholAbsorbtionPerSecond;
    RenownLevelCosts = other.RenownLevelCosts;
    RenownLevelMaxFollowers = other.RenownLevelMaxFollowers;
    RenownLevelMaxBoastingCrowd = other.RenownLevelMaxBoastingCrowd;
    MaxMorality = other.MaxMorality;
    MaxMoralityAsChild = other.MaxMoralityAsChild;
    MaxExtraMaxHitPoints = other.MaxExtraMaxHitPoints;
    MaxExtraMaxStaminaPoints = other.MaxExtraMaxStaminaPoints;
    MoralityChangeOnTheftFromHome = other.MoralityChangeOnTheftFromHome;
    MoralityChangeOnTheftFromShop = other.MoralityChangeOnTheftFromShop;
    MoralityChangeOnPicklock = other.MoralityChangeOnPicklock;
    MoralityChangeOnVandalism = other.MoralityChangeOnVandalism;
    TradeSkillForGuileLevel = other.TradeSkillForGuileLevel;
    StatLevels = other.StatLevels;
    MaxHitPointsPerHealthLevel = other.MaxHitPointsPerHealthLevel;
    MaxStaminaPointsPerMagicPowerLevel = other.MaxStaminaPointsPerMagicPowerLevel;
    SoundDistanceMultiplierPerStealthLevel = other.SoundDistanceMultiplierPerStealthLevel;
    VisibilityMultiplierPerStealthLevel = other.VisibilityMultiplierPerStealthLevel;
    return *this;
}