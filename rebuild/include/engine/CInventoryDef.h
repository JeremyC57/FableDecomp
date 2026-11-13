#ifndef FABLE_ENGINE_CINVENTORYDEF_H
#define FABLE_ENGINE_CINVENTORYDEF_H
// Retail CInventoryDef (agentSND, 2026-11-10, row 0x005A94BD CTCInventoryTrade::OpenInventory): the def NInventory::CTCInventoryBase
// holds in PDef (Ego_r.pdb CDefPointer<const CInventoryDef>, GetPDef). Members: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv,
// sizeof 2656) in order and spelling, typed at the retail STLport offsets: the parent CParentDefClassBase is 0x39 bytes in retail
// (0x3D in the PDB), so the first float sits at +0x3C (PDB +0x40) and each 16-byte PDB vector is 12 bytes. Evidence for the
// offsets: 0x005A95E1 / 0x005A95F2 read the two C2DVector corners at +0xA4 / +0xAC (PDB MeshAreaTLPos +184 / MeshAreaBRPos +192)
// into SetInventoryAnimRenderBox's C2DBoxF. PARTIAL: only the members up to MeshAreaBRPos are modelled (no sizeof; the class is
// only reached through a def pointer).
#include <stddef.h>
#include "engine/CParentDefClassBase.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CINVENTORYDEF_FULL
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineGraphic.h"
#include "engine/CDefString.h"
enum EInventoryIdentifier;                                      // MS forward enum (Ego_r.pdb member type; enumerators not needed here)
#endif
#ifdef FABLE_RETAIL_CINVENTORYDEF_TRANSFER
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CDefString.h"
#include "engine/CEngineGraphic.h"
class CPersistContext;
namespace NInventory { enum EInventoryIdentifier { }; }
#endif

class CInventoryDef : public CParentDefClassBase {
public:
    float InitialCategoryMovementRepeatDelay;                   // +0x3C
    float ContinuedCategoryMovementRepeatDelay;                 // +0x40
    float InitialItemMovementRepeatDelay;                       // +0x44
    float ContinuedItemMovementRepeatDelay;                     // +0x48
    long MovementSound;                                         // +0x4C
    long NoMovementSound;                                       // +0x50
    long InventoryOpenSound;                                    // +0x54
    bool TrackSelectedItems;                                    // +0x58
    std::vector<long> InitialInventoryClothingItems;            // +0x5C
    std::vector<long> InitialInventoryWeaponsItems;             // +0x68
    std::vector<long> InitialInventoryStatsItems;               // +0x74
    std::vector<long> InitialInventoryStandardItems;            // +0x80
    C2DVector ClothingShadedCircleTLPos;                        // +0x8C
    C2DVector ClothingShadedCircleExtents;                      // +0x94
    C2DVector CategoriesTLPos;                                  // +0x9C
    C2DVector MeshAreaTLPos;                                    // +0xA4
    C2DVector MeshAreaBRPos;                                    // +0xAC
#ifdef FABLE_RETAIL_CINVENTORYDEF_FULL
    // Nested opt-in (chunk 2016, row 0x0061AD60 CTCInventoryQuests::DrawActiveTestQuests): the rest of the Ego_r.pdb members in order
    // and spelling (ghidra_out/struct_layouts_egor.tsv, 391 members), typed at the retail offsets: retail = PDB - 4 (the 0x39-byte
    // parent) - 4 per earlier STLport vector (10 vectors, 12 bytes each); C2DVector / C3DVector / CEngineGraphic are byte-packed
    // (alignment 1), so every PDB offset is reproduced by natural layout (checked member by member: 0 mismatches, retail sizeof 0xA34
    // = PDB 0xA60 - 0x2C). Evidence for the offsets this chunk reads: 0x0061AE0F GenericFont +0xCC (font name for CFontManager::GetFont),
    // 0x0061AE96 / 0x0061AE9C QuestCardInfo2TLPos +0x7A8 / +0x7AC, 0x0061AF1A / 0x0061AF24 QuestCardInfoTLPos +0x7A0 / +0x7A4,
    // 0x0061AF48 QuestCardInfoTextOffset.X +0x7BC.
    C2DVector NextCategoryOffset;                               // +0xB4
    C2DVector FirstItemOffsetFromCategory;                      // +0xBC
    C2DVector NextItemOffset;                                   // +0xC4
    CDefString GenericFont;                                     // +0xCC
    CRGBColour BackGroundColour;                                // +0xD0
    long ClothingDoll;                                          // +0xD4
    C3DVector DollCentreOffset;                                 // +0xD8
    float DollBoundingSphereRadius;                             // +0xE4
    C2DVector TotalExperienceTextPos;                           // +0xE8
    unsigned long TotalExperienceText;                          // +0xF0
    C2DVector SpendingExperienceTextPos;                        // +0xF4
    unsigned long SpendingExperienceText;                       // +0xFC
    C2DVector StrengthLevelTextPos;                             // +0x100
    CRGBColour StrengthLevelColour;                             // +0x108
    C2DVector SkillLevelTextPos;                                // +0x10C
    CRGBColour SkillLevelColour;                                // +0x114
    C2DVector WillLevelTextPos;                                 // +0x118
    CRGBColour WillLevelColour;                                 // +0x120
    C2DVector RenownLevelTextPos;                               // +0x124
    C2DVector HeroLevelTextPos;                                 // +0x12C
    C2DVector StatLevelsFirstLineTextPos;                       // +0x134
    C2DVector StatLevelsNextLineTextOffset;                     // +0x13C
    C2DVector ButtonAssignmentTextPos;                          // +0x144
    C2DVector ButtonAssignmentTextOffset;                       // +0x14C
    std::vector<unsigned long> StatLevelsTexts;                 // +0x154
    std::vector<CRGBColour> StatLevelsColours;                  // +0x160
    long WeaponsTextSeparator;                                  // +0x16C
    long WeaponsTextNumberOfSlots;                              // +0x170
    long WeaponsTextSlot;                                       // +0x174
    long WeaponsTextSuccessfulAugmentation;                     // +0x178
    long WeaponsTextNoFreeSlots;                                // +0x17C
    long WeaponsTextReadyToAugment;                             // +0x180
    long WeaponsTextNotReadyToAugment;                          // +0x184
    C2DVector WeaponsDescriptionBoxTLPos;                       // +0x188
    C2DVector WeaponsDescriptionBoxExtents;                     // +0x190
    C2DVector WeaponsActiveMeleeIconPos;                        // +0x198
    C2DVector WeaponsActiveRangeIconPos;                        // +0x1A0
    C2DVector WeaponAugmentationTextStartPos;                   // +0x1A8
    C2DVector WeaponAugmentationTextNewLineOffset;              // +0x1B0
    C2DVector AugmentationStatusTextAreaTLPos;                  // +0x1B8
    C2DVector AugmentationStatusTextAreaBRPos;                  // +0x1C0
    float TimeToDisplayWarningMessageInSeconds;                 // +0x1C8
    C2DVector WeaponsShadedCircleTLPos;                         // +0x1CC
    C2DVector WeaponsShadedCircleExtents;                       // +0x1D4
    float NumberOfSecondsForObjectRotation;                     // +0x1DC
    long MaxNumberOfItemsToDisplayPerCategory;                  // +0x1E0
    long MaxNumberOfCategoriesToDisplay;                        // +0x1E4
    long MaxNumberOfScreenNamesToDisplay;                       // +0x1E8
    unsigned long InventoryName;                                // +0x1EC
    EInventoryIdentifier InventoryIdentifier;                   // +0x1F0
    C2DVector MapScreenMapDisplayBoxTLPos;                      // +0x1F4
    C2DVector MapScreenMapDisplayBoxBRPos;                      // +0x1FC
    C2DVector MapScreenRegionNamePos;                           // +0x204
    long MapScreenWorldMapGraphic;                              // +0x20C
    C2DVector MapScreenWorldMapPos;                             // +0x210
    long MapScreenRegionUnknownGraphic;                         // +0x218
    long MapScreenRegionUnknownHighlightGraphic;                // +0x21C
    long MapScreenRegionHeroGraphic;                            // +0x220
    long MapScreenRegionQuestGraphic;                           // +0x224
    long MapScreenRegionTeleportGraphic;                        // +0x228
    long MapScreenRegionHouseGraphic;                           // +0x22C
    C2DVector StatsTitleTLPos;                                  // +0x230
    C2DVector StatsShadedCircleTLPos;                           // +0x238
    C2DVector StatsShadedCircleExtents;                         // +0x240
    CEngineGraphic StatsProgBarL;                               // +0x248
    CEngineGraphic StatsProgBarC;                               // +0x256
    CEngineGraphic StatsProgBarR;                               // +0x264
    CEngineGraphic StatsProgBarValue;                           // +0x272
    C2DVector StatsProgBarValueOffset;                          // +0x280
    C2DVector StatsPersonalityHeadingTLPos;                     // +0x288
    C2DVector StatsPersonalityHeadingBRPos;                     // +0x290
    unsigned long StatsPersonalityRenownLowerLabel;             // +0x298
    unsigned long StatsPersonalityRenownUpperLabel;             // +0x29C
    unsigned long StatsPersonalityGoodnessLowerLabel;           // +0x2A0
    unsigned long StatsPersonalityGoodnessUpperLabel;           // +0x2A4
    unsigned long StatsPersonalityAttractivenessLowerLabel;     // +0x2A8
    unsigned long StatsPersonalityAttractivenessUpperLabel;     // +0x2AC
    unsigned long StatsPersonalityScarinessLowerLabel;          // +0x2B0
    unsigned long StatsPersonalityScarinessUpperLabel;          // +0x2B4
    unsigned long StatsPersonalityNicenessLowerLabel;           // +0x2B8
    unsigned long StatsPersonalityNicenessUpperLabel;           // +0x2BC
    C2DVector StatsPersonalityLabelTextOffset;                  // +0x2C0
    C2DVector StatsPersonalityLowerLabelTLPos;                  // +0x2C8
    float StatsPersonalityLowerLabelWidth;                      // +0x2D0
    float StatsPersonalityLowerLabelHeight;                     // +0x2D4
    float StatsPersonalitySliderWidth;                          // +0x2D8
    float StatsPersonalityUpperLabelWidth;                      // +0x2DC
    long StatsPersonalityDoll;                                  // +0x2E0
    C3DVector StatsPersonalityDollCentreOffset;                 // +0x2E4
    float StatsPersonalityDollBoundingSphereRadius;             // +0x2F0
    C2DVector StatsStatisticsLabelTLPos;                        // +0x2F4
    float StatsStatisticsLabelWidth;                            // +0x2FC
    float StatsStatisticsLabelHeight;                           // +0x300
    C2DVector StatsStatisticsLabelTextOffset;                   // +0x304
    C2DVector StatsStatisticsDataTextOffset;                    // +0x30C
    C2DVector StatsStatisticsHeadingTLPos;                      // +0x314
    C2DVector StatsStatisticsHeadingBRPos;                      // +0x31C
    unsigned long StatsStatisticsPeopleKilledLabel;             // +0x324
    unsigned long StatsStatisticsMonstersKilledLabel;           // +0x328
    unsigned long StatsStatisticsMaxMultiplierLabel;            // +0x32C
    unsigned long StatsStatisticsPreferredFoodLabel;            // +0x330
    unsigned long StatsStatisticsDietaryTypeLabel;              // +0x334
    unsigned long StatsStatisticsGameTimeElapsedLabel;          // +0x338
    unsigned long StatsStatisticsNumberOfWivesLabel;            // +0x33C
    unsigned long StatsStatisticsQuestsActiveLabel;             // +0x340
    unsigned long StatsStatisticsQuestsFinishedLabel;           // +0x344
    unsigned long StatsStatisticsBoastsAchievedLabel;           // +0x348
    C2DVector StatsExperienceStrengthTextOffset;                // +0x34C
    C2DVector StatsExperienceWillTextOffset;                    // +0x354
    C2DVector StatsExperienceSkillTextOffset;                   // +0x35C
    unsigned long StatsExperienceStrengthText;                  // +0x364
    unsigned long StatsExperienceWillText;                      // +0x368
    unsigned long StatsExperienceSkillText;                     // +0x36C
    C2DVector StatsExperiencePoolGeneralTLPos;                  // +0x370
    C2DVector StatsExperiencePoolGeneralTextOffset;             // +0x378
    C2DVector StatsExperiencePoolStrengthGroupTLPos;            // +0x380
    C2DVector StatsExperiencePoolSkillGroupTLPos;               // +0x388
    C2DVector StatsExperiencePoolWillGroupTLPos;                // +0x390
    C2DVector StatsExperiencePoolLabelTLPos;                    // +0x398
    C2DVector StatsExperiencePoolLabelTextOffset;               // +0x3A0
    float StatsExperiencePoolLabelWidth;                        // +0x3A8
    float StatsExperiencePoolLabelHeight;                       // +0x3AC
    float StatsExperiencePoolSliderWidth;                       // +0x3B0
    unsigned long StatsExperiencePoolStrengthLabel;             // +0x3B4
    unsigned long StatsExperiencePoolSkillLabel;                // +0x3B8
    unsigned long StatsExperiencePoolWillLabel;                 // +0x3BC
    unsigned long StatsExperiencePoolPhysiqueLabel;             // +0x3C0
    unsigned long StatsExperiencePoolHealthLabel;               // +0x3C4
    unsigned long StatsExperiencePoolToughnessLabel;            // +0x3C8
    unsigned long StatsExperiencePoolSpeedLabel;                // +0x3CC
    unsigned long StatsExperiencePoolAccuracyLabel;             // +0x3D0
    unsigned long StatsExperiencePoolStealthLabel;              // +0x3D4
    unsigned long StatsExperiencePoolWeaponMagicLabel;          // +0x3D8
    unsigned long StatsExperiencePoolAbilityMagicLabel;         // +0x3DC
    unsigned long StatsExperiencePoolPureMagicLabel;            // +0x3E0
    float StatsExperienceStrengthBarValue;                      // +0x3E4
    float StatsExperienceSkillBarValue;                         // +0x3E8
    float StatsExperienceWillBarValue;                          // +0x3EC
    C2DVector ExperienceGeneralPoolLabelTLPos;                  // +0x3F0
    C2DVector ExperienceGeneralPoolLabelTextOffset;             // +0x3F8
    float ExperienceGeneralPoolWidth;                           // +0x400
    float ExperienceGeneralPoolSliderWidth;                     // +0x404
    unsigned long ExperienceGeneralPoolBarValue;                // +0x408
    unsigned long ExperienceGeneralPoolLabel;                   // +0x40C
    C2DVector ExperienceGeneralPoolSpendTextOffset;             // +0x410
    C2DVector ExperienceStrengthPoolTLPos;                      // +0x418
    C2DVector ExperienceSkillPoolTLPos;                         // +0x420
    C2DVector ExperienceWillPoolTLPos;                          // +0x428
    float ExperiencePoolWidth;                                  // +0x430
    float ExperiencePoolHeight;                                 // +0x434
    unsigned long ExperienceTypeStrengthLabel;                  // +0x438
    unsigned long ExperienceTypeSkillLabel;                     // +0x43C
    unsigned long ExperienceTypeWillLabel;                      // +0x440
    unsigned long ExperienceTypePureMagicLabel;                 // +0x444
    unsigned long ExperienceTypeAbilityMagicLabel;              // +0x448
    unsigned long ExperienceTypeWeaponMagicLabel;               // +0x44C
    C2DVector ExperienceTypeTLPos;                              // +0x450
    C2DVector ExperienceTypeTLPosOffset;                        // +0x458
    C2DVector ExperienceTypeTextOffset;                         // +0x460
    C2DVector ExperienceTypeSpendTextOffset;                    // +0x468
    float ExperienceTypeLabelWidth;                             // +0x470
    float ExperienceTypeLabelHeight;                            // +0x474
    float ExperienceTypeSliderWidth;                            // +0x478
    unsigned long ExperienceStrengthPhysiqueLabel;              // +0x47C
    unsigned long ExperienceStrengthHealthLabel;                // +0x480
    unsigned long ExperienceStrengthToughnessLabel;             // +0x484
    unsigned long ExperienceSkillSpeedLabel;                    // +0x488
    unsigned long ExperienceSkillAccuracyLabel;                 // +0x48C
    unsigned long ExperienceSkillStealthLabel;                  // +0x490
    unsigned long ExperienceWeaponMagicBerserkLabel;            // +0x494
    unsigned long ExperienceWeaponMagicFlamingBladeLabel;       // +0x498
    unsigned long ExperienceWeaponMagicDoubleStrikeLabel;       // +0x49C
    unsigned long ExperienceWeaponMagicLightningBladeLabel;     // +0x4A0
    unsigned long ExperienceAbilityMagicBattleChargeLabel;      // +0x4A4
    unsigned long ExperienceAbilityMagicAssassinRushLabel;      // +0x4A8
    unsigned long ExperienceAbilityMagicTimeSpellLabel;         // +0x4AC
    unsigned long ExperiencePureMagicBurningHandsLabel;         // +0x4B0
    unsigned long ExperiencePureMagicPhysicalShieldLabel;       // +0x4B4
    unsigned long ExperiencePureMagicForcePushLabel;            // +0x4B8
    unsigned long ExperiencePureMagicEnflameSpellLabel;         // +0x4BC
    unsigned long ExperiencePureMagicDrainLifeLabel;            // +0x4C0
    unsigned long ExperiencePureMagicHealLifeLabel;             // +0x4C4
    unsigned long ExperiencePureMagicGhostSwordLabel;           // +0x4C8
    float ExperienceStrengthBarValue;                           // +0x4CC
    float ExperienceSkillBarValue;                              // +0x4D0
    float ExperienceWillBarValue;                               // +0x4D4
    float ExperienceStrengthPhysiqueBarValue;                   // +0x4D8
    float ExperienceStrengthHealthBarValue;                     // +0x4DC
    float ExperienceStrengthToughnessBarValue;                  // +0x4E0
    float ExperienceSkillSpeedBarValue;                         // +0x4E4
    float ExperienceSkillAccuracyBarValue;                      // +0x4E8
    float ExperienceSkillHealthBarValue;                        // +0x4EC
    float ExperienceWeaponMagicBerserkBarValue;                 // +0x4F0
    float ExperienceWeaponMagicFlamingBladeBarValue;            // +0x4F4
    float ExperienceWeaponMagicDoubleStrikeBarValue;            // +0x4F8
    float ExperienceAbilityMagicBattleChargeBarValue;           // +0x4FC
    float ExperienceAbilityMagicAssassinRushBarValue;           // +0x500
    float ExperienceAbilityMagicTimeSpellBarValue;              // +0x504
    float ExperiencePureMagicBurningHandsBarValue;              // +0x508
    float ExperiencePureMagicPhysicalShieldBarValue;            // +0x50C
    float ExperiencePureMagicForcePushBarValue;                 // +0x510
    CEngineGraphic ExperienceProgBarL;                          // +0x514
    CEngineGraphic ExperienceProgBarC;                          // +0x522
    CEngineGraphic ExperienceProgBarR;                          // +0x530
    CEngineGraphic ExperienceProgBarValue;                      // +0x53E
    C2DVector ExperienceProgBarValueOffset;                     // +0x54C
    long ExperienceSpendTimeOffset;                             // +0x554
    long ExperienceSpendRate;                                   // +0x558
    std::vector<long> ExperienceItemLabelStat;                  // +0x55C
    std::vector<long> ExperienceItemLabelAbility;               // +0x568
    CEngineGraphic ExperienceSpendGreen;                        // +0x574
    CEngineGraphic ExperienceSpendGreenOutline;                 // +0x582
    CEngineGraphic ExperienceSpendGreenEnd;                     // +0x590
    CEngineGraphic ExperienceSpendRed;                          // +0x59E
    CEngineGraphic ExperienceSpendRedOutline;                   // +0x5AC
    CEngineGraphic ExperienceSpendRedEnd;                       // +0x5BA
    C2DVector ExperienceDescriptionTextTLPos;                   // +0x5C8
    C2DVector ExperienceDescriptionTextOffset;                  // +0x5D0
    unsigned long QuestCardInfoQuestNameLabel;                  // +0x5D8
    unsigned long QuestCardInfoMoralityLabel;                   // +0x5DC
    unsigned long QuestCardInfoCoreLabel;                       // +0x5E0
    unsigned long QuestCardInfoDetailsLabel;                    // +0x5E4
    unsigned long QuestCardInfoRegionLabel;                     // +0x5E8
    unsigned long QuestCardInfoOriginatorLabel;                 // +0x5EC
    unsigned long QuestCardInfoDescriptionLabel;                // +0x5F0
    unsigned long QuestCardInfoSuccessLabel;                    // +0x5F4
    unsigned long QuestCardInfoFailureLabel;                    // +0x5F8
    unsigned long QuestCardInfoRewardsLabel;                    // +0x5FC
    unsigned long QuestCardInfoRenownLabel;                     // +0x600
    unsigned long QuestCardInfoTrophyLabel;                     // +0x604
    unsigned long QuestCardInfoBoastsLabel;                     // +0x608
    unsigned long QuestCardInfoStatusLabel;                     // +0x60C
    unsigned long QuestCardInfoQuestNameText;                   // +0x610
    unsigned long QuestCardInfoMoralityText;                    // +0x614
    unsigned long QuestCardInfoCoreText;                        // +0x618
    unsigned long QuestCardInfoDetailsText;                     // +0x61C
    unsigned long QuestCardInfoRegionText;                      // +0x620
    unsigned long QuestCardInfoOriginatorText;                  // +0x624
    unsigned long QuestCardInfoDescriptionText;                 // +0x628
    unsigned long QuestCardInfoSuccessText;                     // +0x62C
    unsigned long QuestCardInfoFailureText;                     // +0x630
    unsigned long QuestCardInfoRewardsText;                     // +0x634
    unsigned long QuestCardInfoRenownText;                      // +0x638
    unsigned long QuestCardInfoTrophyText;                      // +0x63C
    unsigned long QuestCardInfoBoastsText;                      // +0x640
    unsigned long QuestCardInfoStatusText;                      // +0x644
    C2DVector TradeEdgedButtonCostTLPos;                        // +0x648
    C2DVector TradeEdgedButtonOwnedTLPos;                       // +0x650
    C2DVector TradeEdgedButtonProfitTLPos;                      // +0x658
    C2DVector TradeEdgedButtonTitleTLPos;                       // +0x660
    C2DVector TradeEdgedButtonCashTLPos;                        // +0x668
    C2DVector TradeEdgedButtonExtents;                          // +0x670
    C2DVector TradeAnimImageBoxTLPos;                           // +0x678
    C2DVector TradeAnimImageBoxExtents;                         // +0x680
    float TradeAnimImageXRotationTime;                          // +0x688
    float TradeAnimImageYRotationTime;                          // +0x68C
    float TradeAnimImageZRotationTime;                          // +0x690
    C2DVector TradeEquippedItemTLPos;                           // +0x694
    C2DVector TradeEquippedItemExtents;                         // +0x69C
    CEngineGraphic TradeEquippedItemGraphic;                    // +0x6A4
    C2DVector TradeDescriptionBoxTLPos;                         // +0x6B2
    C2DVector TradeDescriptionBoxExtents;                       // +0x6BA
    unsigned long TradeTextOwned;                               // +0x6C4
    unsigned long TradeTextDivider;                             // +0x6C8
    unsigned long TradeTextCost;                                // +0x6CC
    unsigned long TradeTextGold;                                // +0x6D0
    unsigned long TradeTextProfit;                              // +0x6D4
    C2DVector TradeShadedCircleTLPos;                           // +0x6D8
    C2DVector TradeShadedCircleExtents;                         // +0x6E0
    C2DVector AugmentationMeshAreaTLPos;                        // +0x6E8
    C2DVector AugmentationMeshAreaBRPos;                        // +0x6F0
    C2DVector AugmentDescriptionBoxTLPos;                       // +0x6F8
    C2DVector AugmentDescriptionBoxExtents;                     // +0x700
    C2DVector AugmentAugmentationInfoTLPos;                     // +0x708
    C2DVector AugmentAugmentationLabelTLPos;                    // +0x710
    C2DVector AugmentAugmentationInfoTextOffset;                // +0x718
    C2DVector AugmentAugmentationSlotTLPos;                     // +0x720
    C2DVector AugmentAugmentationSlotTextOffset;                // +0x728
    C2DVector AugmentAugmentationWeaponTLPos;                   // +0x730
    C2DVector AugmentAugmentationWeaponTextOffset;              // +0x738
    C2DVector AugmentTitleTLPos;                                // +0x740
    C2DVector AugmentTitleTextOffset;                           // +0x748
    unsigned long AugmentTextMaxSlots;                          // +0x750
    unsigned long AugmentTextNumSlots;                          // +0x754
    unsigned long AugmentTextUpgradeCost;                       // +0x758
    unsigned long AugmentTextGold;                              // +0x75C
    unsigned long AugmentTextDamageMultiplier;                  // +0x760
    unsigned long AugmentTextFireStrength;                      // +0x764
    unsigned long AugmentTextSlot;                              // +0x768
    unsigned long AugmentTextAugmentations;                     // +0x76C
    unsigned long AugmentTextWeapons;                           // +0x770
    unsigned long AugmentTextDivider;                           // +0x774
    C2DVector AugmentShadedCircleTLPos;                         // +0x778
    C2DVector AugmentShadedCircleExtents;                       // +0x780
    float InventoryAnimImageXRotationTime;                      // +0x788
    float InventoryAnimImageYRotationTime;                      // +0x78C
    float InventoryAnimImageZRotationTime;                      // +0x790
    CRGBColour QuestCardFeatColour;                             // +0x794
    CRGBColour QuestCardCoreColour;                             // +0x798
    CRGBColour QuestCardExclusiveColour;                        // +0x79C
    C2DVector QuestCardInfoTLPos;                               // +0x7A0
    C2DVector QuestCardInfo2TLPos;                              // +0x7A8
    C2DVector QuestCardNoQuestsTLPos;                           // +0x7B0
    unsigned long QuestCardNoQuestsText;                        // +0x7B8
    C2DVector QuestCardInfoTextOffset;                          // +0x7BC
    C2DVector QuestCardEdgedButtonTitleTLPos;                   // +0x7C4
    C2DVector QuestCardEdgedButtonExtents;                      // +0x7CC
    std::vector<long> ExpSpendItemLabelStat;                    // +0x7D4
    std::vector<long> ExpSpendItemLabelAbility;                 // +0x7E0
    long ExpSpendMaxItemsToDisplay;                             // +0x7EC
    C2DVector ExpSpendButtonTLPos;                              // +0x7F0
    C2DVector ExpSpendButtonTLPosOffset;                        // +0x7F8
    C2DVector ExpSpendButtonTextOffset;                         // +0x800
    CEngineGraphic ExpSpendButtonSelL;                          // +0x808
    CEngineGraphic ExpSpendButtonSelC;                          // +0x816
    CEngineGraphic ExpSpendButtonSelR;                          // +0x824
    CEngineGraphic ExpSpendButtonL;                             // +0x832
    CEngineGraphic ExpSpendButtonC;                             // +0x840
    CEngineGraphic ExpSpendButtonR;                             // +0x84E
    C2DVector ExpSpendDescriptionBoxTLPos;                      // +0x85C
    C2DVector ExpSpendDescriptionBoxExtents;                    // +0x864
    C2DVector ExpSpendArrowUpTLPos;                             // +0x86C
    C2DVector ExpSpendArrowDnTLPos;                             // +0x874
    C2DVector ExpSpendGeneralPoolLabelTLPos;                    // +0x87C
    C2DVector ExpSpendGeneralPoolLabelTextOffset;               // +0x884
    C2DVector ExpSpendGeneralPoolSpendTextOffset;               // +0x88C
    float ExpSpendGeneralPoolWidth;                             // +0x894
    float ExpSpendGeneralPoolSliderWidth;                       // +0x898
    C2DVector ExpSpendStrengthPoolTLPos;                        // +0x89C
    C2DVector ExpSpendSkillPoolTLPos;                           // +0x8A4
    C2DVector ExpSpendWillPoolTLPos;                            // +0x8AC
    float ExpSpendPoolWidth;                                    // +0x8B4
    float ExpSpendPoolHeight;                                   // +0x8B8
    CEngineGraphic ExpSpendProgBarL;                            // +0x8BC
    CEngineGraphic ExpSpendProgBarC;                            // +0x8CA
    CEngineGraphic ExpSpendProgBarR;                            // +0x8D8
    CEngineGraphic ExpSpendProgBarValue;                        // +0x8E6
    C2DVector ExpSpendProgBarValueOffset;                       // +0x8F4
    C2DVector ExpSpendTypeTextOffset;                           // +0x8FC
    long ExpSpendGeneralPoolBarValue;                           // +0x904
    long ExpSpendGeneralPoolLabel;                              // +0x908
    long ExpSpendTypeStrengthLabel;                             // +0x90C
    long ExpSpendTypeSkillLabel;                                // +0x910
    long ExpSpendTypeWillLabel;                                 // +0x914
    float ExpSpendStrengthBarValue;                             // +0x918
    float ExpSpendSkillBarValue;                                // +0x91C
    float ExpSpendWillBarValue;                                 // +0x920
    long ExpSpendRate;                                          // +0x924
    long ExpSpendTimeOffset;                                    // +0x928
    CEngineGraphic ExpSpendGreen;                               // +0x92C
    CEngineGraphic ExpSpendGreenOutline;                        // +0x93A
    CEngineGraphic ExpSpendGreenEnd;                            // +0x948
    CEngineGraphic ExpSpendRed;                                 // +0x956
    CEngineGraphic ExpSpendRedOutline;                          // +0x964
    CEngineGraphic ExpSpendRedEnd;                              // +0x972
    C2DVector ExpSpendDescriptionTextTLPos;                     // +0x980
    C2DVector ExpSpendDescriptionTextOffset;                    // +0x988
    C2DVector MainEdgedButtonTitleTLPos;                        // +0x990
    C2DVector MainEdgedButtonOwnedTLPos;                        // +0x998
    C2DVector MainEdgedButtonPriceTLPos;                        // +0x9A0
    C2DVector MainEdgedButtonCostTLPos;                         // +0x9A8
    C2DVector MainEdgedButtonExtents;                           // +0x9B0
    CEngineGraphic MainDescriptionBoxC;                         // +0x9B8
    CEngineGraphic MainDescriptionBoxTL;                        // +0x9C6
    CEngineGraphic MainDescriptionBoxTR;                        // +0x9D4
    CEngineGraphic MainDescriptionBoxBL;                        // +0x9E2
    CEngineGraphic MainDescriptionBoxBR;                        // +0x9F0
    C2DVector MainDescriptionBoxTLPos;                          // +0x9FE
    C2DVector MainDescriptionBoxExtents;                        // +0xA06
    C2DVector MainShadedCircleTLPos;                            // +0xA0E
    C2DVector MainShadedCircleExtents;                          // +0xA16
    C2DVector MainAnimBoxTLPos;                                 // +0xA1E
    C2DVector MainAnimBoxExtents;                               // +0xA26
    float MainQuickAccessItemFlashTime;                         // +0xA30
#ifdef FABLE_RETAIL_CINVENTORYDEF_ASSIGN
    // Nested opt-in (acctB bwf2a, row 0x0045A159 CInventoryDef::operator=): the user copy-assignment Ego_r.pdb / FableWin.pdb
    // ??4CInventoryDef@@QAEAAV0@ABV0@@Z, out of line in retail at 0x0045A159 (the row defines it). Needs FABLE_RETAIL_CINVENTORYDEF_FULL.
    CInventoryDef& operator=(const CInventoryDef& other);     // 0x0045A159
#endif
#endif
#ifdef FABLE_RETAIL_CINVENTORYDEF_TRANSFER
    // Retail opt-in (bwf1, row 0x004678E2 CInventoryDef::Transfer): the rest of the Ego_r.pdb member list at the retail offsets
    // (every one of the 389 transfer calls hits its member; sizeof 0xA34 = retail GetSizeofClass 0x004554A4).
    C2DVector NextCategoryOffset;                                     // +0xB4 (Ego_r +0xC8)
    C2DVector FirstItemOffsetFromCategory;                            // +0xBC (Ego_r +0xD0)
    C2DVector NextItemOffset;                                         // +0xC4 (Ego_r +0xD8)
    CDefString GenericFont;                                           // +0xCC (Ego_r +0xE0)
    CRGBColour BackGroundColour;                                      // +0xD0 (Ego_r +0xE4)
    long ClothingDoll;                                                // +0xD4 (Ego_r +0xE8)
    C3DVector DollCentreOffset;                                       // +0xD8 (Ego_r +0xEC)
    float DollBoundingSphereRadius;                                   // +0xE4 (Ego_r +0xF8)
    C2DVector TotalExperienceTextPos;                                 // +0xE8 (Ego_r +0xFC)
    unsigned long TotalExperienceText;                                // +0xF0 (Ego_r +0x104)
    C2DVector SpendingExperienceTextPos;                              // +0xF4 (Ego_r +0x108)
    unsigned long SpendingExperienceText;                             // +0xFC (Ego_r +0x110)
    C2DVector StrengthLevelTextPos;                                   // +0x100 (Ego_r +0x114)
    CRGBColour StrengthLevelColour;                                   // +0x108 (Ego_r +0x11C)
    C2DVector SkillLevelTextPos;                                      // +0x10C (Ego_r +0x120)
    CRGBColour SkillLevelColour;                                      // +0x114 (Ego_r +0x128)
    C2DVector WillLevelTextPos;                                       // +0x118 (Ego_r +0x12C)
    CRGBColour WillLevelColour;                                       // +0x120 (Ego_r +0x134)
    C2DVector RenownLevelTextPos;                                     // +0x124 (Ego_r +0x138)
    C2DVector HeroLevelTextPos;                                       // +0x12C (Ego_r +0x140)
    C2DVector StatLevelsFirstLineTextPos;                             // +0x134 (Ego_r +0x148)
    C2DVector StatLevelsNextLineTextOffset;                           // +0x13C (Ego_r +0x150)
    C2DVector ButtonAssignmentTextPos;                                // +0x144 (Ego_r +0x158)
    C2DVector ButtonAssignmentTextOffset;                             // +0x14C (Ego_r +0x160)
    std::vector<unsigned long> StatLevelsTexts;                       // +0x154 (Ego_r +0x168)
    std::vector<CRGBColour> StatLevelsColours;                        // +0x160 (Ego_r +0x178)
    long WeaponsTextSeparator;                                        // +0x16C (Ego_r +0x188)
    long WeaponsTextNumberOfSlots;                                    // +0x170 (Ego_r +0x18C)
    long WeaponsTextSlot;                                             // +0x174 (Ego_r +0x190)
    long WeaponsTextSuccessfulAugmentation;                           // +0x178 (Ego_r +0x194)
    long WeaponsTextNoFreeSlots;                                      // +0x17C (Ego_r +0x198)
    long WeaponsTextReadyToAugment;                                   // +0x180 (Ego_r +0x19C)
    long WeaponsTextNotReadyToAugment;                                // +0x184 (Ego_r +0x1A0)
    C2DVector WeaponsDescriptionBoxTLPos;                             // +0x188 (Ego_r +0x1A4)
    C2DVector WeaponsDescriptionBoxExtents;                           // +0x190 (Ego_r +0x1AC)
    C2DVector WeaponsActiveMeleeIconPos;                              // +0x198 (Ego_r +0x1B4)
    C2DVector WeaponsActiveRangeIconPos;                              // +0x1A0 (Ego_r +0x1BC)
    C2DVector WeaponAugmentationTextStartPos;                         // +0x1A8 (Ego_r +0x1C4)
    C2DVector WeaponAugmentationTextNewLineOffset;                    // +0x1B0 (Ego_r +0x1CC)
    C2DVector AugmentationStatusTextAreaTLPos;                        // +0x1B8 (Ego_r +0x1D4)
    C2DVector AugmentationStatusTextAreaBRPos;                        // +0x1C0 (Ego_r +0x1DC)
    float TimeToDisplayWarningMessageInSeconds;                       // +0x1C8 (Ego_r +0x1E4)
    C2DVector WeaponsShadedCircleTLPos;                               // +0x1CC (Ego_r +0x1E8)
    C2DVector WeaponsShadedCircleExtents;                             // +0x1D4 (Ego_r +0x1F0)
    float NumberOfSecondsForObjectRotation;                           // +0x1DC (Ego_r +0x1F8)
    long MaxNumberOfItemsToDisplayPerCategory;                        // +0x1E0 (Ego_r +0x1FC)
    long MaxNumberOfCategoriesToDisplay;                              // +0x1E4 (Ego_r +0x200)
    long MaxNumberOfScreenNamesToDisplay;                             // +0x1E8 (Ego_r +0x204)
    unsigned long InventoryName;                                      // +0x1EC (Ego_r +0x208)
    NInventory::EInventoryIdentifier InventoryIdentifier;             // +0x1F0 (Ego_r +0x20C)
    C2DVector MapScreenMapDisplayBoxTLPos;                            // +0x1F4 (Ego_r +0x210)
    C2DVector MapScreenMapDisplayBoxBRPos;                            // +0x1FC (Ego_r +0x218)
    C2DVector MapScreenRegionNamePos;                                 // +0x204 (Ego_r +0x220)
    long MapScreenWorldMapGraphic;                                    // +0x20C (Ego_r +0x228)
    C2DVector MapScreenWorldMapPos;                                   // +0x210 (Ego_r +0x22C)
    long MapScreenRegionUnknownGraphic;                               // +0x218 (Ego_r +0x234)
    long MapScreenRegionUnknownHighlightGraphic;                      // +0x21C (Ego_r +0x238)
    long MapScreenRegionHeroGraphic;                                  // +0x220 (Ego_r +0x23C)
    long MapScreenRegionQuestGraphic;                                 // +0x224 (Ego_r +0x240)
    long MapScreenRegionTeleportGraphic;                              // +0x228 (Ego_r +0x244)
    long MapScreenRegionHouseGraphic;                                 // +0x22C (Ego_r +0x248)
    C2DVector StatsTitleTLPos;                                        // +0x230 (Ego_r +0x24C)
    C2DVector StatsShadedCircleTLPos;                                 // +0x238 (Ego_r +0x254)
    C2DVector StatsShadedCircleExtents;                               // +0x240 (Ego_r +0x25C)
    CEngineGraphic StatsProgBarL;                                     // +0x248 (Ego_r +0x264)
    CEngineGraphic StatsProgBarC;                                     // +0x256 (Ego_r +0x272)
    CEngineGraphic StatsProgBarR;                                     // +0x264 (Ego_r +0x280)
    CEngineGraphic StatsProgBarValue;                                 // +0x272 (Ego_r +0x28E)
    C2DVector StatsProgBarValueOffset;                                // +0x280 (Ego_r +0x29C)
    C2DVector StatsPersonalityHeadingTLPos;                           // +0x288 (Ego_r +0x2A4)
    C2DVector StatsPersonalityHeadingBRPos;                           // +0x290 (Ego_r +0x2AC)
    unsigned long StatsPersonalityRenownLowerLabel;                   // +0x298 (Ego_r +0x2B4)
    unsigned long StatsPersonalityRenownUpperLabel;                   // +0x29C (Ego_r +0x2B8)
    unsigned long StatsPersonalityGoodnessLowerLabel;                 // +0x2A0 (Ego_r +0x2BC)
    unsigned long StatsPersonalityGoodnessUpperLabel;                 // +0x2A4 (Ego_r +0x2C0)
    unsigned long StatsPersonalityAttractivenessLowerLabel;           // +0x2A8 (Ego_r +0x2C4)
    unsigned long StatsPersonalityAttractivenessUpperLabel;           // +0x2AC (Ego_r +0x2C8)
    unsigned long StatsPersonalityScarinessLowerLabel;                // +0x2B0 (Ego_r +0x2CC)
    unsigned long StatsPersonalityScarinessUpperLabel;                // +0x2B4 (Ego_r +0x2D0)
    unsigned long StatsPersonalityNicenessLowerLabel;                 // +0x2B8 (Ego_r +0x2D4)
    unsigned long StatsPersonalityNicenessUpperLabel;                 // +0x2BC (Ego_r +0x2D8)
    C2DVector StatsPersonalityLabelTextOffset;                        // +0x2C0 (Ego_r +0x2DC)
    C2DVector StatsPersonalityLowerLabelTLPos;                        // +0x2C8 (Ego_r +0x2E4)
    float StatsPersonalityLowerLabelWidth;                            // +0x2D0 (Ego_r +0x2EC)
    float StatsPersonalityLowerLabelHeight;                           // +0x2D4 (Ego_r +0x2F0)
    float StatsPersonalitySliderWidth;                                // +0x2D8 (Ego_r +0x2F4)
    float StatsPersonalityUpperLabelWidth;                            // +0x2DC (Ego_r +0x2F8)
    long StatsPersonalityDoll;                                        // +0x2E0 (Ego_r +0x2FC)
    C3DVector StatsPersonalityDollCentreOffset;                       // +0x2E4 (Ego_r +0x300)
    float StatsPersonalityDollBoundingSphereRadius;                   // +0x2F0 (Ego_r +0x30C)
    C2DVector StatsStatisticsLabelTLPos;                              // +0x2F4 (Ego_r +0x310)
    float StatsStatisticsLabelWidth;                                  // +0x2FC (Ego_r +0x318)
    float StatsStatisticsLabelHeight;                                 // +0x300 (Ego_r +0x31C)
    C2DVector StatsStatisticsLabelTextOffset;                         // +0x304 (Ego_r +0x320)
    C2DVector StatsStatisticsDataTextOffset;                          // +0x30C (Ego_r +0x328)
    C2DVector StatsStatisticsHeadingTLPos;                            // +0x314 (Ego_r +0x330)
    C2DVector StatsStatisticsHeadingBRPos;                            // +0x31C (Ego_r +0x338)
    unsigned long StatsStatisticsPeopleKilledLabel;                   // +0x324 (Ego_r +0x340)
    unsigned long StatsStatisticsMonstersKilledLabel;                 // +0x328 (Ego_r +0x344)
    unsigned long StatsStatisticsMaxMultiplierLabel;                  // +0x32C (Ego_r +0x348)
    unsigned long StatsStatisticsPreferredFoodLabel;                  // +0x330 (Ego_r +0x34C)
    unsigned long StatsStatisticsDietaryTypeLabel;                    // +0x334 (Ego_r +0x350)
    unsigned long StatsStatisticsGameTimeElapsedLabel;                // +0x338 (Ego_r +0x354)
    unsigned long StatsStatisticsNumberOfWivesLabel;                  // +0x33C (Ego_r +0x358)
    unsigned long StatsStatisticsQuestsActiveLabel;                   // +0x340 (Ego_r +0x35C)
    unsigned long StatsStatisticsQuestsFinishedLabel;                 // +0x344 (Ego_r +0x360)
    unsigned long StatsStatisticsBoastsAchievedLabel;                 // +0x348 (Ego_r +0x364)
    C2DVector StatsExperienceStrengthTextOffset;                      // +0x34C (Ego_r +0x368)
    C2DVector StatsExperienceWillTextOffset;                          // +0x354 (Ego_r +0x370)
    C2DVector StatsExperienceSkillTextOffset;                         // +0x35C (Ego_r +0x378)
    unsigned long StatsExperienceStrengthText;                        // +0x364 (Ego_r +0x380)
    unsigned long StatsExperienceWillText;                            // +0x368 (Ego_r +0x384)
    unsigned long StatsExperienceSkillText;                           // +0x36C (Ego_r +0x388)
    C2DVector StatsExperiencePoolGeneralTLPos;                        // +0x370 (Ego_r +0x38C)
    C2DVector StatsExperiencePoolGeneralTextOffset;                   // +0x378 (Ego_r +0x394)
    C2DVector StatsExperiencePoolStrengthGroupTLPos;                  // +0x380 (Ego_r +0x39C)
    C2DVector StatsExperiencePoolSkillGroupTLPos;                     // +0x388 (Ego_r +0x3A4)
    C2DVector StatsExperiencePoolWillGroupTLPos;                      // +0x390 (Ego_r +0x3AC)
    C2DVector StatsExperiencePoolLabelTLPos;                          // +0x398 (Ego_r +0x3B4)
    C2DVector StatsExperiencePoolLabelTextOffset;                     // +0x3A0 (Ego_r +0x3BC)
    float StatsExperiencePoolLabelWidth;                              // +0x3A8 (Ego_r +0x3C4)
    float StatsExperiencePoolLabelHeight;                             // +0x3AC (Ego_r +0x3C8)
    float StatsExperiencePoolSliderWidth;                             // +0x3B0 (Ego_r +0x3CC)
    unsigned long StatsExperiencePoolStrengthLabel;                   // +0x3B4 (Ego_r +0x3D0)
    unsigned long StatsExperiencePoolSkillLabel;                      // +0x3B8 (Ego_r +0x3D4)
    unsigned long StatsExperiencePoolWillLabel;                       // +0x3BC (Ego_r +0x3D8)
    unsigned long StatsExperiencePoolPhysiqueLabel;                   // +0x3C0 (Ego_r +0x3DC)
    unsigned long StatsExperiencePoolHealthLabel;                     // +0x3C4 (Ego_r +0x3E0)
    unsigned long StatsExperiencePoolToughnessLabel;                  // +0x3C8 (Ego_r +0x3E4)
    unsigned long StatsExperiencePoolSpeedLabel;                      // +0x3CC (Ego_r +0x3E8)
    unsigned long StatsExperiencePoolAccuracyLabel;                   // +0x3D0 (Ego_r +0x3EC)
    unsigned long StatsExperiencePoolStealthLabel;                    // +0x3D4 (Ego_r +0x3F0)
    unsigned long StatsExperiencePoolWeaponMagicLabel;                // +0x3D8 (Ego_r +0x3F4)
    unsigned long StatsExperiencePoolAbilityMagicLabel;               // +0x3DC (Ego_r +0x3F8)
    unsigned long StatsExperiencePoolPureMagicLabel;                  // +0x3E0 (Ego_r +0x3FC)
    float StatsExperienceStrengthBarValue;                            // +0x3E4 (Ego_r +0x400)
    float StatsExperienceSkillBarValue;                               // +0x3E8 (Ego_r +0x404)
    float StatsExperienceWillBarValue;                                // +0x3EC (Ego_r +0x408)
    C2DVector ExperienceGeneralPoolLabelTLPos;                        // +0x3F0 (Ego_r +0x40C)
    C2DVector ExperienceGeneralPoolLabelTextOffset;                   // +0x3F8 (Ego_r +0x414)
    float ExperienceGeneralPoolWidth;                                 // +0x400 (Ego_r +0x41C)
    float ExperienceGeneralPoolSliderWidth;                           // +0x404 (Ego_r +0x420)
    unsigned long ExperienceGeneralPoolBarValue;                      // +0x408 (Ego_r +0x424)
    unsigned long ExperienceGeneralPoolLabel;                         // +0x40C (Ego_r +0x428)
    C2DVector ExperienceGeneralPoolSpendTextOffset;                   // +0x410 (Ego_r +0x42C)
    C2DVector ExperienceStrengthPoolTLPos;                            // +0x418 (Ego_r +0x434)
    C2DVector ExperienceSkillPoolTLPos;                               // +0x420 (Ego_r +0x43C)
    C2DVector ExperienceWillPoolTLPos;                                // +0x428 (Ego_r +0x444)
    float ExperiencePoolWidth;                                        // +0x430 (Ego_r +0x44C)
    float ExperiencePoolHeight;                                       // +0x434 (Ego_r +0x450)
    unsigned long ExperienceTypeStrengthLabel;                        // +0x438 (Ego_r +0x454)
    unsigned long ExperienceTypeSkillLabel;                           // +0x43C (Ego_r +0x458)
    unsigned long ExperienceTypeWillLabel;                            // +0x440 (Ego_r +0x45C)
    unsigned long ExperienceTypePureMagicLabel;                       // +0x444 (Ego_r +0x460)
    unsigned long ExperienceTypeAbilityMagicLabel;                    // +0x448 (Ego_r +0x464)
    unsigned long ExperienceTypeWeaponMagicLabel;                     // +0x44C (Ego_r +0x468)
    C2DVector ExperienceTypeTLPos;                                    // +0x450 (Ego_r +0x46C)
    C2DVector ExperienceTypeTLPosOffset;                              // +0x458 (Ego_r +0x474)
    C2DVector ExperienceTypeTextOffset;                               // +0x460 (Ego_r +0x47C)
    C2DVector ExperienceTypeSpendTextOffset;                          // +0x468 (Ego_r +0x484)
    float ExperienceTypeLabelWidth;                                   // +0x470 (Ego_r +0x48C)
    float ExperienceTypeLabelHeight;                                  // +0x474 (Ego_r +0x490)
    float ExperienceTypeSliderWidth;                                  // +0x478 (Ego_r +0x494)
    unsigned long ExperienceStrengthPhysiqueLabel;                    // +0x47C (Ego_r +0x498)
    unsigned long ExperienceStrengthHealthLabel;                      // +0x480 (Ego_r +0x49C)
    unsigned long ExperienceStrengthToughnessLabel;                   // +0x484 (Ego_r +0x4A0)
    unsigned long ExperienceSkillSpeedLabel;                          // +0x488 (Ego_r +0x4A4)
    unsigned long ExperienceSkillAccuracyLabel;                       // +0x48C (Ego_r +0x4A8)
    unsigned long ExperienceSkillStealthLabel;                        // +0x490 (Ego_r +0x4AC)
    unsigned long ExperienceWeaponMagicBerserkLabel;                  // +0x494 (Ego_r +0x4B0)
    unsigned long ExperienceWeaponMagicFlamingBladeLabel;             // +0x498 (Ego_r +0x4B4)
    unsigned long ExperienceWeaponMagicDoubleStrikeLabel;             // +0x49C (Ego_r +0x4B8)
    unsigned long ExperienceWeaponMagicLightningBladeLabel;           // +0x4A0 (Ego_r +0x4BC)
    unsigned long ExperienceAbilityMagicBattleChargeLabel;            // +0x4A4 (Ego_r +0x4C0)
    unsigned long ExperienceAbilityMagicAssassinRushLabel;            // +0x4A8 (Ego_r +0x4C4)
    unsigned long ExperienceAbilityMagicTimeSpellLabel;               // +0x4AC (Ego_r +0x4C8)
    unsigned long ExperiencePureMagicBurningHandsLabel;               // +0x4B0 (Ego_r +0x4CC)
    unsigned long ExperiencePureMagicPhysicalShieldLabel;             // +0x4B4 (Ego_r +0x4D0)
    unsigned long ExperiencePureMagicForcePushLabel;                  // +0x4B8 (Ego_r +0x4D4)
    unsigned long ExperiencePureMagicEnflameSpellLabel;               // +0x4BC (Ego_r +0x4D8)
    unsigned long ExperiencePureMagicDrainLifeLabel;                  // +0x4C0 (Ego_r +0x4DC)
    unsigned long ExperiencePureMagicHealLifeLabel;                   // +0x4C4 (Ego_r +0x4E0)
    unsigned long ExperiencePureMagicGhostSwordLabel;                 // +0x4C8 (Ego_r +0x4E4)
    float ExperienceStrengthBarValue;                                 // +0x4CC (Ego_r +0x4E8)
    float ExperienceSkillBarValue;                                    // +0x4D0 (Ego_r +0x4EC)
    float ExperienceWillBarValue;                                     // +0x4D4 (Ego_r +0x4F0)
    float ExperienceStrengthPhysiqueBarValue;                         // +0x4D8 (Ego_r +0x4F4)
    float ExperienceStrengthHealthBarValue;                           // +0x4DC (Ego_r +0x4F8)
    float ExperienceStrengthToughnessBarValue;                        // +0x4E0 (Ego_r +0x4FC)
    float ExperienceSkillSpeedBarValue;                               // +0x4E4 (Ego_r +0x500)
    float ExperienceSkillAccuracyBarValue;                            // +0x4E8 (Ego_r +0x504)
    float ExperienceSkillHealthBarValue;                              // +0x4EC (Ego_r +0x508)
    float ExperienceWeaponMagicBerserkBarValue;                       // +0x4F0 (Ego_r +0x50C)
    float ExperienceWeaponMagicFlamingBladeBarValue;                  // +0x4F4 (Ego_r +0x510)
    float ExperienceWeaponMagicDoubleStrikeBarValue;                  // +0x4F8 (Ego_r +0x514)
    float ExperienceAbilityMagicBattleChargeBarValue;                 // +0x4FC (Ego_r +0x518)
    float ExperienceAbilityMagicAssassinRushBarValue;                 // +0x500 (Ego_r +0x51C)
    float ExperienceAbilityMagicTimeSpellBarValue;                    // +0x504 (Ego_r +0x520)
    float ExperiencePureMagicBurningHandsBarValue;                    // +0x508 (Ego_r +0x524)
    float ExperiencePureMagicPhysicalShieldBarValue;                  // +0x50C (Ego_r +0x528)
    float ExperiencePureMagicForcePushBarValue;                       // +0x510 (Ego_r +0x52C)
    CEngineGraphic ExperienceProgBarL;                                // +0x514 (Ego_r +0x530)
    CEngineGraphic ExperienceProgBarC;                                // +0x522 (Ego_r +0x53E)
    CEngineGraphic ExperienceProgBarR;                                // +0x530 (Ego_r +0x54C)
    CEngineGraphic ExperienceProgBarValue;                            // +0x53E (Ego_r +0x55A)
    C2DVector ExperienceProgBarValueOffset;                           // +0x54C (Ego_r +0x568)
    long ExperienceSpendTimeOffset;                                   // +0x554 (Ego_r +0x570)
    long ExperienceSpendRate;                                         // +0x558 (Ego_r +0x574)
    std::vector<long> ExperienceItemLabelStat;                        // +0x55C (Ego_r +0x578)
    std::vector<long> ExperienceItemLabelAbility;                     // +0x568 (Ego_r +0x588)
    CEngineGraphic ExperienceSpendGreen;                              // +0x574 (Ego_r +0x598)
    CEngineGraphic ExperienceSpendGreenOutline;                       // +0x582 (Ego_r +0x5A6)
    CEngineGraphic ExperienceSpendGreenEnd;                           // +0x590 (Ego_r +0x5B4)
    CEngineGraphic ExperienceSpendRed;                                // +0x59E (Ego_r +0x5C2)
    CEngineGraphic ExperienceSpendRedOutline;                         // +0x5AC (Ego_r +0x5D0)
    CEngineGraphic ExperienceSpendRedEnd;                             // +0x5BA (Ego_r +0x5DE)
    C2DVector ExperienceDescriptionTextTLPos;                         // +0x5C8 (Ego_r +0x5EC)
    C2DVector ExperienceDescriptionTextOffset;                        // +0x5D0 (Ego_r +0x5F4)
    unsigned long QuestCardInfoQuestNameLabel;                        // +0x5D8 (Ego_r +0x5FC)
    unsigned long QuestCardInfoMoralityLabel;                         // +0x5DC (Ego_r +0x600)
    unsigned long QuestCardInfoCoreLabel;                             // +0x5E0 (Ego_r +0x604)
    unsigned long QuestCardInfoDetailsLabel;                          // +0x5E4 (Ego_r +0x608)
    unsigned long QuestCardInfoRegionLabel;                           // +0x5E8 (Ego_r +0x60C)
    unsigned long QuestCardInfoOriginatorLabel;                       // +0x5EC (Ego_r +0x610)
    unsigned long QuestCardInfoDescriptionLabel;                      // +0x5F0 (Ego_r +0x614)
    unsigned long QuestCardInfoSuccessLabel;                          // +0x5F4 (Ego_r +0x618)
    unsigned long QuestCardInfoFailureLabel;                          // +0x5F8 (Ego_r +0x61C)
    unsigned long QuestCardInfoRewardsLabel;                          // +0x5FC (Ego_r +0x620)
    unsigned long QuestCardInfoRenownLabel;                           // +0x600 (Ego_r +0x624)
    unsigned long QuestCardInfoTrophyLabel;                           // +0x604 (Ego_r +0x628)
    unsigned long QuestCardInfoBoastsLabel;                           // +0x608 (Ego_r +0x62C)
    unsigned long QuestCardInfoStatusLabel;                           // +0x60C (Ego_r +0x630)
    unsigned long QuestCardInfoQuestNameText;                         // +0x610 (Ego_r +0x634)
    unsigned long QuestCardInfoMoralityText;                          // +0x614 (Ego_r +0x638)
    unsigned long QuestCardInfoCoreText;                              // +0x618 (Ego_r +0x63C)
    unsigned long QuestCardInfoDetailsText;                           // +0x61C (Ego_r +0x640)
    unsigned long QuestCardInfoRegionText;                            // +0x620 (Ego_r +0x644)
    unsigned long QuestCardInfoOriginatorText;                        // +0x624 (Ego_r +0x648)
    unsigned long QuestCardInfoDescriptionText;                       // +0x628 (Ego_r +0x64C)
    unsigned long QuestCardInfoSuccessText;                           // +0x62C (Ego_r +0x650)
    unsigned long QuestCardInfoFailureText;                           // +0x630 (Ego_r +0x654)
    unsigned long QuestCardInfoRewardsText;                           // +0x634 (Ego_r +0x658)
    unsigned long QuestCardInfoRenownText;                            // +0x638 (Ego_r +0x65C)
    unsigned long QuestCardInfoTrophyText;                            // +0x63C (Ego_r +0x660)
    unsigned long QuestCardInfoBoastsText;                            // +0x640 (Ego_r +0x664)
    unsigned long QuestCardInfoStatusText;                            // +0x644 (Ego_r +0x668)
    C2DVector TradeEdgedButtonCostTLPos;                              // +0x648 (Ego_r +0x66C)
    C2DVector TradeEdgedButtonOwnedTLPos;                             // +0x650 (Ego_r +0x674)
    C2DVector TradeEdgedButtonProfitTLPos;                            // +0x658 (Ego_r +0x67C)
    C2DVector TradeEdgedButtonTitleTLPos;                             // +0x660 (Ego_r +0x684)
    C2DVector TradeEdgedButtonCashTLPos;                              // +0x668 (Ego_r +0x68C)
    C2DVector TradeEdgedButtonExtents;                                // +0x670 (Ego_r +0x694)
    C2DVector TradeAnimImageBoxTLPos;                                 // +0x678 (Ego_r +0x69C)
    C2DVector TradeAnimImageBoxExtents;                               // +0x680 (Ego_r +0x6A4)
    float TradeAnimImageXRotationTime;                                // +0x688 (Ego_r +0x6AC)
    float TradeAnimImageYRotationTime;                                // +0x68C (Ego_r +0x6B0)
    float TradeAnimImageZRotationTime;                                // +0x690 (Ego_r +0x6B4)
    C2DVector TradeEquippedItemTLPos;                                 // +0x694 (Ego_r +0x6B8)
    C2DVector TradeEquippedItemExtents;                               // +0x69C (Ego_r +0x6C0)
    CEngineGraphic TradeEquippedItemGraphic;                          // +0x6A4 (Ego_r +0x6C8)
    C2DVector TradeDescriptionBoxTLPos;                               // +0x6B2 (Ego_r +0x6D6)
    C2DVector TradeDescriptionBoxExtents;                             // +0x6BA (Ego_r +0x6DE)
    unsigned long TradeTextOwned;                                     // +0x6C4 (Ego_r +0x6E8)
    unsigned long TradeTextDivider;                                   // +0x6C8 (Ego_r +0x6EC)
    unsigned long TradeTextCost;                                      // +0x6CC (Ego_r +0x6F0)
    unsigned long TradeTextGold;                                      // +0x6D0 (Ego_r +0x6F4)
    unsigned long TradeTextProfit;                                    // +0x6D4 (Ego_r +0x6F8)
    C2DVector TradeShadedCircleTLPos;                                 // +0x6D8 (Ego_r +0x6FC)
    C2DVector TradeShadedCircleExtents;                               // +0x6E0 (Ego_r +0x704)
    C2DVector AugmentationMeshAreaTLPos;                              // +0x6E8 (Ego_r +0x70C)
    C2DVector AugmentationMeshAreaBRPos;                              // +0x6F0 (Ego_r +0x714)
    C2DVector AugmentDescriptionBoxTLPos;                             // +0x6F8 (Ego_r +0x71C)
    C2DVector AugmentDescriptionBoxExtents;                           // +0x700 (Ego_r +0x724)
    C2DVector AugmentAugmentationInfoTLPos;                           // +0x708 (Ego_r +0x72C)
    C2DVector AugmentAugmentationLabelTLPos;                          // +0x710 (Ego_r +0x734)
    C2DVector AugmentAugmentationInfoTextOffset;                      // +0x718 (Ego_r +0x73C)
    C2DVector AugmentAugmentationSlotTLPos;                           // +0x720 (Ego_r +0x744)
    C2DVector AugmentAugmentationSlotTextOffset;                      // +0x728 (Ego_r +0x74C)
    C2DVector AugmentAugmentationWeaponTLPos;                         // +0x730 (Ego_r +0x754)
    C2DVector AugmentAugmentationWeaponTextOffset;                    // +0x738 (Ego_r +0x75C)
    C2DVector AugmentTitleTLPos;                                      // +0x740 (Ego_r +0x764)
    C2DVector AugmentTitleTextOffset;                                 // +0x748 (Ego_r +0x76C)
    unsigned long AugmentTextMaxSlots;                                // +0x750 (Ego_r +0x774)
    unsigned long AugmentTextNumSlots;                                // +0x754 (Ego_r +0x778)
    unsigned long AugmentTextUpgradeCost;                             // +0x758 (Ego_r +0x77C)
    unsigned long AugmentTextGold;                                    // +0x75C (Ego_r +0x780)
    unsigned long AugmentTextDamageMultiplier;                        // +0x760 (Ego_r +0x784)
    unsigned long AugmentTextFireStrength;                            // +0x764 (Ego_r +0x788)
    unsigned long AugmentTextSlot;                                    // +0x768 (Ego_r +0x78C)
    unsigned long AugmentTextAugmentations;                           // +0x76C (Ego_r +0x790)
    unsigned long AugmentTextWeapons;                                 // +0x770 (Ego_r +0x794)
    unsigned long AugmentTextDivider;                                 // +0x774 (Ego_r +0x798)
    C2DVector AugmentShadedCircleTLPos;                               // +0x778 (Ego_r +0x79C)
    C2DVector AugmentShadedCircleExtents;                             // +0x780 (Ego_r +0x7A4)
    float InventoryAnimImageXRotationTime;                            // +0x788 (Ego_r +0x7AC)
    float InventoryAnimImageYRotationTime;                            // +0x78C (Ego_r +0x7B0)
    float InventoryAnimImageZRotationTime;                            // +0x790 (Ego_r +0x7B4)
    CRGBColour QuestCardFeatColour;                                   // +0x794 (Ego_r +0x7B8)
    CRGBColour QuestCardCoreColour;                                   // +0x798 (Ego_r +0x7BC)
    CRGBColour QuestCardExclusiveColour;                              // +0x79C (Ego_r +0x7C0)
    C2DVector QuestCardInfoTLPos;                                     // +0x7A0 (Ego_r +0x7C4)
    C2DVector QuestCardInfo2TLPos;                                    // +0x7A8 (Ego_r +0x7CC)
    C2DVector QuestCardNoQuestsTLPos;                                 // +0x7B0 (Ego_r +0x7D4)
    unsigned long QuestCardNoQuestsText;                              // +0x7B8 (Ego_r +0x7DC)
    C2DVector QuestCardInfoTextOffset;                                // +0x7BC (Ego_r +0x7E0)
    C2DVector QuestCardEdgedButtonTitleTLPos;                         // +0x7C4 (Ego_r +0x7E8)
    C2DVector QuestCardEdgedButtonExtents;                            // +0x7CC (Ego_r +0x7F0)
    std::vector<long> ExpSpendItemLabelStat;                          // +0x7D4 (Ego_r +0x7F8)
    std::vector<long> ExpSpendItemLabelAbility;                       // +0x7E0 (Ego_r +0x808)
    long ExpSpendMaxItemsToDisplay;                                   // +0x7EC (Ego_r +0x818)
    C2DVector ExpSpendButtonTLPos;                                    // +0x7F0 (Ego_r +0x81C)
    C2DVector ExpSpendButtonTLPosOffset;                              // +0x7F8 (Ego_r +0x824)
    C2DVector ExpSpendButtonTextOffset;                               // +0x800 (Ego_r +0x82C)
    CEngineGraphic ExpSpendButtonSelL;                                // +0x808 (Ego_r +0x834)
    CEngineGraphic ExpSpendButtonSelC;                                // +0x816 (Ego_r +0x842)
    CEngineGraphic ExpSpendButtonSelR;                                // +0x824 (Ego_r +0x850)
    CEngineGraphic ExpSpendButtonL;                                   // +0x832 (Ego_r +0x85E)
    CEngineGraphic ExpSpendButtonC;                                   // +0x840 (Ego_r +0x86C)
    CEngineGraphic ExpSpendButtonR;                                   // +0x84E (Ego_r +0x87A)
    C2DVector ExpSpendDescriptionBoxTLPos;                            // +0x85C (Ego_r +0x888)
    C2DVector ExpSpendDescriptionBoxExtents;                          // +0x864 (Ego_r +0x890)
    C2DVector ExpSpendArrowUpTLPos;                                   // +0x86C (Ego_r +0x898)
    C2DVector ExpSpendArrowDnTLPos;                                   // +0x874 (Ego_r +0x8A0)
    C2DVector ExpSpendGeneralPoolLabelTLPos;                          // +0x87C (Ego_r +0x8A8)
    C2DVector ExpSpendGeneralPoolLabelTextOffset;                     // +0x884 (Ego_r +0x8B0)
    C2DVector ExpSpendGeneralPoolSpendTextOffset;                     // +0x88C (Ego_r +0x8B8)
    float ExpSpendGeneralPoolWidth;                                   // +0x894 (Ego_r +0x8C0)
    float ExpSpendGeneralPoolSliderWidth;                             // +0x898 (Ego_r +0x8C4)
    C2DVector ExpSpendStrengthPoolTLPos;                              // +0x89C (Ego_r +0x8C8)
    C2DVector ExpSpendSkillPoolTLPos;                                 // +0x8A4 (Ego_r +0x8D0)
    C2DVector ExpSpendWillPoolTLPos;                                  // +0x8AC (Ego_r +0x8D8)
    float ExpSpendPoolWidth;                                          // +0x8B4 (Ego_r +0x8E0)
    float ExpSpendPoolHeight;                                         // +0x8B8 (Ego_r +0x8E4)
    CEngineGraphic ExpSpendProgBarL;                                  // +0x8BC (Ego_r +0x8E8)
    CEngineGraphic ExpSpendProgBarC;                                  // +0x8CA (Ego_r +0x8F6)
    CEngineGraphic ExpSpendProgBarR;                                  // +0x8D8 (Ego_r +0x904)
    CEngineGraphic ExpSpendProgBarValue;                              // +0x8E6 (Ego_r +0x912)
    C2DVector ExpSpendProgBarValueOffset;                             // +0x8F4 (Ego_r +0x920)
    C2DVector ExpSpendTypeTextOffset;                                 // +0x8FC (Ego_r +0x928)
    long ExpSpendGeneralPoolBarValue;                                 // +0x904 (Ego_r +0x930)
    long ExpSpendGeneralPoolLabel;                                    // +0x908 (Ego_r +0x934)
    long ExpSpendTypeStrengthLabel;                                   // +0x90C (Ego_r +0x938)
    long ExpSpendTypeSkillLabel;                                      // +0x910 (Ego_r +0x93C)
    long ExpSpendTypeWillLabel;                                       // +0x914 (Ego_r +0x940)
    float ExpSpendStrengthBarValue;                                   // +0x918 (Ego_r +0x944)
    float ExpSpendSkillBarValue;                                      // +0x91C (Ego_r +0x948)
    float ExpSpendWillBarValue;                                       // +0x920 (Ego_r +0x94C)
    long ExpSpendRate;                                                // +0x924 (Ego_r +0x950)
    long ExpSpendTimeOffset;                                          // +0x928 (Ego_r +0x954)
    CEngineGraphic ExpSpendGreen;                                     // +0x92C (Ego_r +0x958)
    CEngineGraphic ExpSpendGreenOutline;                              // +0x93A (Ego_r +0x966)
    CEngineGraphic ExpSpendGreenEnd;                                  // +0x948 (Ego_r +0x974)
    CEngineGraphic ExpSpendRed;                                       // +0x956 (Ego_r +0x982)
    CEngineGraphic ExpSpendRedOutline;                                // +0x964 (Ego_r +0x990)
    CEngineGraphic ExpSpendRedEnd;                                    // +0x972 (Ego_r +0x99E)
    C2DVector ExpSpendDescriptionTextTLPos;                           // +0x980 (Ego_r +0x9AC)
    C2DVector ExpSpendDescriptionTextOffset;                          // +0x988 (Ego_r +0x9B4)
    C2DVector MainEdgedButtonTitleTLPos;                              // +0x990 (Ego_r +0x9BC)
    C2DVector MainEdgedButtonOwnedTLPos;                              // +0x998 (Ego_r +0x9C4)
    C2DVector MainEdgedButtonPriceTLPos;                              // +0x9A0 (Ego_r +0x9CC)
    C2DVector MainEdgedButtonCostTLPos;                               // +0x9A8 (Ego_r +0x9D4)
    C2DVector MainEdgedButtonExtents;                                 // +0x9B0 (Ego_r +0x9DC)
    CEngineGraphic MainDescriptionBoxC;                               // +0x9B8 (Ego_r +0x9E4)
    CEngineGraphic MainDescriptionBoxTL;                              // +0x9C6 (Ego_r +0x9F2)
    CEngineGraphic MainDescriptionBoxTR;                              // +0x9D4 (Ego_r +0xA00)
    CEngineGraphic MainDescriptionBoxBL;                              // +0x9E2 (Ego_r +0xA0E)
    CEngineGraphic MainDescriptionBoxBR;                              // +0x9F0 (Ego_r +0xA1C)
    C2DVector MainDescriptionBoxTLPos;                                // +0x9FE (Ego_r +0xA2A)
    C2DVector MainDescriptionBoxExtents;                              // +0xA06 (Ego_r +0xA32)
    C2DVector MainShadedCircleTLPos;                                  // +0xA0E (Ego_r +0xA3A)
    C2DVector MainShadedCircleExtents;                                // +0xA16 (Ego_r +0xA42)
    C2DVector MainAnimBoxTLPos;                                       // +0xA1E (Ego_r +0xA4A)
    C2DVector MainAnimBoxExtents;                                     // +0xA26 (Ego_r +0xA52)
    float MainQuickAccessItemFlashTime;                               // +0xA30 (Ego_r +0xA5C)
    virtual void Transfer(CPersistContext& context);   // slot 18
#endif
};
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialCategoryMovementRepeatDelay) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialInventoryClothingItems) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaTLPos) == 0xA4);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaBRPos) == 0xAC);
#ifdef FABLE_RETAIL_CINVENTORYDEF_FULL
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, GenericFont) == 0xCC);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, QuestCardInfoTLPos) == 0x7A0);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, QuestCardInfo2TLPos) == 0x7A8);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, QuestCardInfoTextOffset) == 0x7BC);
FABLE_STATIC_ASSERT(sizeof(CInventoryDef) == 0xA34);
#endif

#ifdef FABLE_RETAIL_CINVENTORYDEF_TRANSFER
FABLE_STATIC_ASSERT(sizeof(CInventoryDef) == 0xA34);
#endif
#endif // FABLE_ENGINE_CINVENTORYDEF_H
