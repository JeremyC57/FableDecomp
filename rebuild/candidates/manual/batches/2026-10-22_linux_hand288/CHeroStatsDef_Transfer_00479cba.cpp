// CHeroStatsDef::Transfer(CPersistContext& context) -- persist the hero stats definition: stamina / health refill, death penalties, money and experience caps, age, appearance scaling, fatness and drinking, renown tables, morality changes and the per-level stat tables
// Leaf: ?Transfer@CHeroStatsDef
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00479CBA, length 570 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers; the catalog labels on the shared ones are propagated): long 0x00431102,
//   float 0x00431061, vector<CAppearanceModifierScalingDef> 0x00479EF4, vector<long> 0x00466A47,
//   vector<float> 0x00464CBF, vector<CAbilityTrackDef> 0x0047A2F5.
// Layout: Ego_r.pdb CHeroStatsDef (0x164) re-based on the retail bytes: the def base is 0x3c bytes (PDB 0x40) and every
//   STLport vector is 12 bytes (16 in the PDB): MaximumStamina +0x3c .. VisibilityMultiplierPerStealthLevel +0x12c.
//   No generated engine header for the class.
// Pragma: #pragma optimize("s", on) -- a size TU: every member address goes through eax.
// Shape: one transfer per member in declaration order, except that MoralityChangeOnVandalism (+0xec) is transferred
//   before MoralityChangeOnPicklock (+0xe8).
struct CAppearanceModifierScalingDefVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct CAbilityTrackDefVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
struct CFloatVector { float* _M_start; float* _M_finish; float* _M_end_of_storage; };

struct CPersistContext {
    void Transfer(long& value);                                    // 0x00431102
    void Transfer(float& value);                                   // 0x00431061
    void Transfer(CAppearanceModifierScalingDefVector& value);     // 0x00479EF4
    void Transfer(CLongVector& value);                             // 0x00466A47
    void Transfer(CFloatVector& value);                            // 0x00464CBF
    void Transfer(CAbilityTrackDefVector& value);                  // 0x0047A2F5
};

struct CDefClassBase {
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x3c - 4];
};

struct CHeroStatsDef : CDefClassBase {
    long  MaximumStamina;                                  // +0x3c
    float NumberOfSecondsAfterUseStaminaStartsRefilling;   // +0x40
    float NumberOfSecondsAfterChangeHealthStartsRefilling; // +0x44
    float NumberOfSecondsBetweenHealthRefills;             // +0x48
    long  StaminaToRegainPerSecond;                        // +0x4c
    long  HealthToRegainPerRefill;                         // +0x50
    long  RenownLossOnDeath;                               // +0x54
    long  MoneyLossOnDeath;                                // +0x58
    long  MaximumMoney;                                    // +0x5c
    long  MaximumExpToSpend;                               // +0x60
    long  ExpToSpendAtWhichToDisplayTutorial;              // +0x64
    float MinAge;                                          // +0x68
    float MaxAge;                                          // +0x6c
    CAppearanceModifierScalingDefVector AppearanceModifierScaling; // +0x70
    float AmountFatnessIncreasesPerPointOfOverEating;      // +0x7c
    float NumberOfMinutesBetweenFatnessReductions;         // +0x80
    float AmountFatnessDecreasesPerReduction;              // +0x84
    float DrinkAffectMultiplierMultiplier;                 // +0x88
    long  MinVomitTime;                                    // +0x8c
    long  MaxVomitTime;                                    // +0x90
    long  MinTimeBeforeVomiting;                           // +0x94
    long  MaxTimeBeforeVomiting;                           // +0x98
    float PintsOfSobrietyPerSecondOfPuke;                  // +0x9c
    float PintsToDrunk;                                    // +0xa0
    float PintsToVomit;                                    // +0xa4
    float AlcoholAbsorbtionPerSecond;                      // +0xa8
    CLongVector RenownLevelCosts;                          // +0xac
    CLongVector RenownLevelMaxFollowers;                   // +0xb8
    CLongVector RenownLevelMaxBoastingCrowd;               // +0xc4
    long  MaxMorality;                                     // +0xd0
    long  MaxMoralityAsChild;                              // +0xd4
    float MaxExtraMaxHitPoints;                            // +0xd8
    long  MaxExtraMaxStaminaPoints;                        // +0xdc
    long  MoralityChangeOnTheftFromHome;                   // +0xe0
    long  MoralityChangeOnTheftFromShop;                   // +0xe4
    long  MoralityChangeOnPicklock;                        // +0xe8
    long  MoralityChangeOnVandalism;                       // +0xec
    CFloatVector TradeSkillForGuileLevel;                  // +0xf0
    CAbilityTrackDefVector StatLevels;                     // +0xfc
    CFloatVector MaxHitPointsPerHealthLevel;               // +0x108
    CLongVector MaxStaminaPointsPerMagicPowerLevel;        // +0x114
    CFloatVector SoundDistanceMultiplierPerStealthLevel;   // +0x120
    CFloatVector VisibilityMultiplierPerStealthLevel;      // +0x12c
    virtual void Transfer(CPersistContext& context);
};

void CHeroStatsDef::Transfer(CPersistContext& context)
{
    context.Transfer(MaximumStamina);
    context.Transfer(NumberOfSecondsAfterUseStaminaStartsRefilling);
    context.Transfer(NumberOfSecondsAfterChangeHealthStartsRefilling);
    context.Transfer(NumberOfSecondsBetweenHealthRefills);
    context.Transfer(StaminaToRegainPerSecond);
    context.Transfer(HealthToRegainPerRefill);
    context.Transfer(RenownLossOnDeath);
    context.Transfer(MoneyLossOnDeath);
    context.Transfer(MaximumMoney);
    context.Transfer(MaximumExpToSpend);
    context.Transfer(ExpToSpendAtWhichToDisplayTutorial);
    context.Transfer(MinAge);
    context.Transfer(MaxAge);
    context.Transfer(AppearanceModifierScaling);
    context.Transfer(AmountFatnessIncreasesPerPointOfOverEating);
    context.Transfer(NumberOfMinutesBetweenFatnessReductions);
    context.Transfer(AmountFatnessDecreasesPerReduction);
    context.Transfer(DrinkAffectMultiplierMultiplier);
    context.Transfer(MinVomitTime);
    context.Transfer(MaxVomitTime);
    context.Transfer(MinTimeBeforeVomiting);
    context.Transfer(MaxTimeBeforeVomiting);
    context.Transfer(PintsOfSobrietyPerSecondOfPuke);
    context.Transfer(PintsToDrunk);
    context.Transfer(PintsToVomit);
    context.Transfer(AlcoholAbsorbtionPerSecond);
    context.Transfer(RenownLevelCosts);
    context.Transfer(RenownLevelMaxFollowers);
    context.Transfer(RenownLevelMaxBoastingCrowd);
    context.Transfer(MaxMorality);
    context.Transfer(MaxMoralityAsChild);
    context.Transfer(MaxExtraMaxHitPoints);
    context.Transfer(MaxExtraMaxStaminaPoints);
    context.Transfer(MoralityChangeOnTheftFromHome);
    context.Transfer(MoralityChangeOnTheftFromShop);
    context.Transfer(MoralityChangeOnVandalism);
    context.Transfer(MoralityChangeOnPicklock);
    context.Transfer(TradeSkillForGuileLevel);
    context.Transfer(StatLevels);
    context.Transfer(MaxHitPointsPerHealthLevel);
    context.Transfer(MaxStaminaPointsPerMagicPowerLevel);
    context.Transfer(SoundDistanceMultiplierPerStealthLevel);
    context.Transfer(VisibilityMultiplierPerStealthLevel);
}
