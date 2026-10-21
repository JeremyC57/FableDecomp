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
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

#define FABLE_RETAIL_VECTOR(Name) \
    struct Name { void* m_first; void* m_last; void* m_end; Name& operator=(const Name& other); }

FABLE_RETAIL_VECTOR(CFloatVector);                  // vector<float>::operator= 0x0045EF2D
FABLE_RETAIL_VECTOR(CLongVector);                   // vector<long>::operator= 0x00454886
FABLE_RETAIL_VECTOR(CAppearanceModifierScalingVector); // operator= 0x0047832D
FABLE_RETAIL_VECTOR(CAbilityTrackVector);           // operator= 0x0047857D

struct CHeroStatsDef : CParentDefClassBase {          // vtable 0x012339DC
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
    CAppearanceModifierScalingVector AppearanceModifierScaling;
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
    CLongVector RenownLevelCosts;
    CLongVector RenownLevelMaxFollowers;
    CLongVector RenownLevelMaxBoastingCrowd;
    long MaxMorality;
    long MaxMoralityAsChild;
    float MaxExtraMaxHitPoints;
    long MaxExtraMaxStaminaPoints;
    long MoralityChangeOnTheftFromHome;
    long MoralityChangeOnTheftFromShop;
    long MoralityChangeOnPicklock;
    long MoralityChangeOnVandalism;
    CFloatVector TradeSkillForGuileLevel;
    CAbilityTrackVector StatLevels;
    CFloatVector MaxHitPointsPerHealthLevel;
    CLongVector MaxStaminaPointsPerMagicPowerLevel;
    CFloatVector SoundDistanceMultiplierPerStealthLevel;
    CFloatVector VisibilityMultiplierPerStealthLevel;
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
