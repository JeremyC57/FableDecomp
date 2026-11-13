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

#endif // FABLE_ENGINE_CINVENTORYDEF_H
