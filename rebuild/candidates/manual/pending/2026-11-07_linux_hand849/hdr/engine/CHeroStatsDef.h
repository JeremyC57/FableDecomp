#ifndef FABLE_ENGINE_CHEROSTATSDEF_H
#define FABLE_ENGINE_CHEROSTATSDEF_H
// CHeroStatsDef (chunk 849, row 0x0057E2F1 CTCHeroStats::OnSerialise): Ego_r.pdb members and order (struct_layouts_egor.tsv, PDB
// sizeof 0x164) on the retail containers (STLport vector 0xC) and the packed 0x39-byte CParentDefClassBase, so the first member
// lands at +0x3C and sizeof is 0x138. Retail evidence: OnSerialise reads MaximumStamina at +0x3C (0x0057E448 `mov eax,[eax+0DCh] ;
// mov eax,[eax+3Ch]`), and the landed 0x0057DE14 ApplyPenaltyForDeath reads RenownLossOnDeath +0x54 / MoneyLossOnDeath +0x58 (each
// 4 below the PDB, as here). Container members are opaque bytes with their real type in the comment.
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CHeroStatsDef : public CParentDefClassBase {
public:
    long          MaximumStamina;                                // +0x3c
    float         NumberOfSecondsAfterUseStaminaStartsRefilling; // +0x40
    float         NumberOfSecondsAfterChangeHealthStartsRefilling; // +0x44
    float         NumberOfSecondsBetweenHealthRefills;           // +0x48
    long          StaminaToRegainPerSecond;                      // +0x4c
    long          HealthToRegainPerRefill;                       // +0x50
    long          RenownLossOnDeath;                             // +0x54
    long          MoneyLossOnDeath;                              // +0x58
    long          MaximumMoney;                                  // +0x5c
    long          MaximumExpToSpend;                             // +0x60
    long          ExpToSpendAtWhichToDisplayTutorial;            // +0x64
    float         MinAge;                                        // +0x68
    float         MaxAge;                                        // +0x6c
    unsigned char AppearanceModifierScaling[0xc];                // +0x70 vector<CAppearanceModifierScalingDef>
    float         AmountFatnessIncreasesPerPointOfOverEating;    // +0x7c
    float         NumberOfMinutesBetweenFatnessReductions;       // +0x80
    float         AmountFatnessDecreasesPerReduction;            // +0x84
    float         DrinkAffectMultiplierMultiplier;               // +0x88
    long          MinVomitTime;                                  // +0x8c
    long          MaxVomitTime;                                  // +0x90
    long          MinTimeBeforeVomiting;                         // +0x94
    long          MaxTimeBeforeVomiting;                         // +0x98
    float         PintsOfSobrietyPerSecondOfPuke;                // +0x9c
    float         PintsToDrunk;                                  // +0xa0
    float         PintsToVomit;                                  // +0xa4
    float         AlcoholAbsorbtionPerSecond;                    // +0xa8
    unsigned char RenownLevelCosts[0xc];                         // +0xac vector<long>
    unsigned char RenownLevelMaxFollowers[0xc];                  // +0xb8 vector<long>
    unsigned char RenownLevelMaxBoastingCrowd[0xc];              // +0xc4 vector<long>
    long          MaxMorality;                                   // +0xd0
    long          MaxMoralityAsChild;                            // +0xd4
    float         MaxExtraMaxHitPoints;                          // +0xd8
    long          MaxExtraMaxStaminaPoints;                      // +0xdc
    long          MoralityChangeOnTheftFromHome;                 // +0xe0
    long          MoralityChangeOnTheftFromShop;                 // +0xe4
    long          MoralityChangeOnPicklock;                      // +0xe8
    long          MoralityChangeOnVandalism;                     // +0xec
    unsigned char TradeSkillForGuileLevel[0xc];                  // +0xf0 vector<float>
    unsigned char StatLevels[0xc];                               // +0xfc vector<CAbilityTrackDef>
    unsigned char MaxHitPointsPerHealthLevel[0xc];               // +0x108 vector<float>
    unsigned char MaxStaminaPointsPerMagicPowerLevel[0xc];       // +0x114 vector<long>
    unsigned char SoundDistanceMultiplierPerStealthLevel[0xc];   // +0x120 vector<float>
    unsigned char VisibilityMultiplierPerStealthLevel[0xc];      // +0x12c vector<float>
};
FABLE_STATIC_ASSERT(sizeof(CHeroStatsDef) == 0x138);
#endif
