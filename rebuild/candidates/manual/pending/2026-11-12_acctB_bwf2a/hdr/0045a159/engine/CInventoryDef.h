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
// Full member list (Ego_r.pdb order, retail offsets) and the user operator= of 0x0045A159.
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineGraphic.h"
#include "engine/CDefString.h"
enum EInventoryIdentifier { };      // Ego_r.pdb enum (4 bytes); enumerators not needed here
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
    C2DVector NextCategoryOffset;                             // 0x00b4
    C2DVector FirstItemOffsetFromCategory;                    // 0x00bc
    C2DVector NextItemOffset;                                 // 0x00c4
    CDefString GenericFont;                                   // 0x00cc
    CRGBColour BackGroundColour;                              // 0x00d0
    long ClothingDoll;                                        // 0x00d4
    C3DVector DollCentreOffset;                               // 0x00d8
    float DollBoundingSphereRadius;                           // 0x00e4
    C2DVector TotalExperienceTextPos;                         // 0x00e8
    unsigned long TotalExperienceText;                        // 0x00f0
    C2DVector SpendingExperienceTextPos;                      // 0x00f4
    unsigned long SpendingExperienceText;                     // 0x00fc
    C2DVector StrengthLevelTextPos;                           // 0x0100
    CRGBColour StrengthLevelColour;                           // 0x0108
    C2DVector SkillLevelTextPos;                              // 0x010c
    CRGBColour SkillLevelColour;                              // 0x0114
    C2DVector WillLevelTextPos;                               // 0x0118
    CRGBColour WillLevelColour;                               // 0x0120
    C2DVector RenownLevelTextPos;                             // 0x0124
    C2DVector HeroLevelTextPos;                               // 0x012c
    C2DVector StatLevelsFirstLineTextPos;                     // 0x0134
    C2DVector StatLevelsNextLineTextOffset;                   // 0x013c
    C2DVector ButtonAssignmentTextPos;                        // 0x0144
    C2DVector ButtonAssignmentTextOffset;                     // 0x014c
    std::vector<unsigned long> StatLevelsTexts;               // 0x0154
    std::vector<CRGBColour> StatLevelsColours;                // 0x0160
    long WeaponsTextSeparator;                                // 0x016c
    long WeaponsTextNumberOfSlots;                            // 0x0170
    long WeaponsTextSlot;                                     // 0x0174
    long WeaponsTextSuccessfulAugmentation;                   // 0x0178
    long WeaponsTextNoFreeSlots;                              // 0x017c
    long WeaponsTextReadyToAugment;                           // 0x0180
    long WeaponsTextNotReadyToAugment;                        // 0x0184
    C2DVector WeaponsDescriptionBoxTLPos;                     // 0x0188
    C2DVector WeaponsDescriptionBoxExtents;                   // 0x0190
    C2DVector WeaponsActiveMeleeIconPos;                      // 0x0198
    C2DVector WeaponsActiveRangeIconPos;                      // 0x01a0
    C2DVector WeaponAugmentationTextStartPos;                 // 0x01a8
    C2DVector WeaponAugmentationTextNewLineOffset;            // 0x01b0
    C2DVector AugmentationStatusTextAreaTLPos;                // 0x01b8
    C2DVector AugmentationStatusTextAreaBRPos;                // 0x01c0
    float TimeToDisplayWarningMessageInSeconds;               // 0x01c8
    C2DVector WeaponsShadedCircleTLPos;                       // 0x01cc
    C2DVector WeaponsShadedCircleExtents;                     // 0x01d4
    float NumberOfSecondsForObjectRotation;                   // 0x01dc
    long MaxNumberOfItemsToDisplayPerCategory;                // 0x01e0
    long MaxNumberOfCategoriesToDisplay;                      // 0x01e4
    long MaxNumberOfScreenNamesToDisplay;                     // 0x01e8
    unsigned long InventoryName;                              // 0x01ec
    EInventoryIdentifier InventoryIdentifier;                 // 0x01f0
    C2DVector MapScreenMapDisplayBoxTLPos;                    // 0x01f4
    C2DVector MapScreenMapDisplayBoxBRPos;                    // 0x01fc
    C2DVector MapScreenRegionNamePos;                         // 0x0204
    long MapScreenWorldMapGraphic;                            // 0x020c
    C2DVector MapScreenWorldMapPos;                           // 0x0210
    long MapScreenRegionUnknownGraphic;                       // 0x0218
    long MapScreenRegionUnknownHighlightGraphic;              // 0x021c
    long MapScreenRegionHeroGraphic;                          // 0x0220
    long MapScreenRegionQuestGraphic;                         // 0x0224
    long MapScreenRegionTeleportGraphic;                      // 0x0228
    long MapScreenRegionHouseGraphic;                         // 0x022c
    C2DVector StatsTitleTLPos;                                // 0x0230
    C2DVector StatsShadedCircleTLPos;                         // 0x0238
    C2DVector StatsShadedCircleExtents;                       // 0x0240
    CEngineGraphic StatsProgBarL;                             // 0x0248
    CEngineGraphic StatsProgBarC;                             // 0x0256
    CEngineGraphic StatsProgBarR;                             // 0x0264
    CEngineGraphic StatsProgBarValue;                         // 0x0272
    C2DVector StatsProgBarValueOffset;                        // 0x0280
    C2DVector StatsPersonalityHeadingTLPos;                   // 0x0288
    C2DVector StatsPersonalityHeadingBRPos;                   // 0x0290
    unsigned long StatsPersonalityRenownLowerLabel;           // 0x0298
    unsigned long StatsPersonalityRenownUpperLabel;           // 0x029c
    unsigned long StatsPersonalityGoodnessLowerLabel;         // 0x02a0
    unsigned long StatsPersonalityGoodnessUpperLabel;         // 0x02a4
    unsigned long StatsPersonalityAttractivenessLowerLabel;   // 0x02a8
    unsigned long StatsPersonalityAttractivenessUpperLabel;   // 0x02ac
    unsigned long StatsPersonalityScarinessLowerLabel;        // 0x02b0
    unsigned long StatsPersonalityScarinessUpperLabel;        // 0x02b4
    unsigned long StatsPersonalityNicenessLowerLabel;         // 0x02b8
    unsigned long StatsPersonalityNicenessUpperLabel;         // 0x02bc
    C2DVector StatsPersonalityLabelTextOffset;                // 0x02c0
    C2DVector StatsPersonalityLowerLabelTLPos;                // 0x02c8
    float StatsPersonalityLowerLabelWidth;                    // 0x02d0
    float StatsPersonalityLowerLabelHeight;                   // 0x02d4
    float StatsPersonalitySliderWidth;                        // 0x02d8
    float StatsPersonalityUpperLabelWidth;                    // 0x02dc
    long StatsPersonalityDoll;                                // 0x02e0
    C3DVector StatsPersonalityDollCentreOffset;               // 0x02e4
    float StatsPersonalityDollBoundingSphereRadius;           // 0x02f0
    C2DVector StatsStatisticsLabelTLPos;                      // 0x02f4
    float StatsStatisticsLabelWidth;                          // 0x02fc
    float StatsStatisticsLabelHeight;                         // 0x0300
    C2DVector StatsStatisticsLabelTextOffset;                 // 0x0304
    C2DVector StatsStatisticsDataTextOffset;                  // 0x030c
    C2DVector StatsStatisticsHeadingTLPos;                    // 0x0314
    C2DVector StatsStatisticsHeadingBRPos;                    // 0x031c
    unsigned long StatsStatisticsPeopleKilledLabel;           // 0x0324
    unsigned long StatsStatisticsMonstersKilledLabel;         // 0x0328
    unsigned long StatsStatisticsMaxMultiplierLabel;          // 0x032c
    unsigned long StatsStatisticsPreferredFoodLabel;          // 0x0330
    unsigned long StatsStatisticsDietaryTypeLabel;            // 0x0334
    unsigned long StatsStatisticsGameTimeElapsedLabel;        // 0x0338
    unsigned long StatsStatisticsNumberOfWivesLabel;          // 0x033c
    unsigned long StatsStatisticsQuestsActiveLabel;           // 0x0340
    unsigned long StatsStatisticsQuestsFinishedLabel;         // 0x0344
    unsigned long StatsStatisticsBoastsAchievedLabel;         // 0x0348
    C2DVector StatsExperienceStrengthTextOffset;              // 0x034c
    C2DVector StatsExperienceWillTextOffset;                  // 0x0354
    C2DVector StatsExperienceSkillTextOffset;                 // 0x035c
    unsigned long StatsExperienceStrengthText;                // 0x0364
    unsigned long StatsExperienceWillText;                    // 0x0368
    unsigned long StatsExperienceSkillText;                   // 0x036c
    C2DVector StatsExperiencePoolGeneralTLPos;                // 0x0370
    C2DVector StatsExperiencePoolGeneralTextOffset;           // 0x0378
    C2DVector StatsExperiencePoolStrengthGroupTLPos;          // 0x0380
    C2DVector StatsExperiencePoolSkillGroupTLPos;             // 0x0388
    C2DVector StatsExperiencePoolWillGroupTLPos;              // 0x0390
    C2DVector StatsExperiencePoolLabelTLPos;                  // 0x0398
    C2DVector StatsExperiencePoolLabelTextOffset;             // 0x03a0
    float StatsExperiencePoolLabelWidth;                      // 0x03a8
    float StatsExperiencePoolLabelHeight;                     // 0x03ac
    float StatsExperiencePoolSliderWidth;                     // 0x03b0
    unsigned long StatsExperiencePoolStrengthLabel;           // 0x03b4
    unsigned long StatsExperiencePoolSkillLabel;              // 0x03b8
    unsigned long StatsExperiencePoolWillLabel;               // 0x03bc
    unsigned long StatsExperiencePoolPhysiqueLabel;           // 0x03c0
    unsigned long StatsExperiencePoolHealthLabel;             // 0x03c4
    unsigned long StatsExperiencePoolToughnessLabel;          // 0x03c8
    unsigned long StatsExperiencePoolSpeedLabel;              // 0x03cc
    unsigned long StatsExperiencePoolAccuracyLabel;           // 0x03d0
    unsigned long StatsExperiencePoolStealthLabel;            // 0x03d4
    unsigned long StatsExperiencePoolWeaponMagicLabel;        // 0x03d8
    unsigned long StatsExperiencePoolAbilityMagicLabel;       // 0x03dc
    unsigned long StatsExperiencePoolPureMagicLabel;          // 0x03e0
    float StatsExperienceStrengthBarValue;                    // 0x03e4
    float StatsExperienceSkillBarValue;                       // 0x03e8
    float StatsExperienceWillBarValue;                        // 0x03ec
    C2DVector ExperienceGeneralPoolLabelTLPos;                // 0x03f0
    C2DVector ExperienceGeneralPoolLabelTextOffset;           // 0x03f8
    float ExperienceGeneralPoolWidth;                         // 0x0400
    float ExperienceGeneralPoolSliderWidth;                   // 0x0404
    unsigned long ExperienceGeneralPoolBarValue;              // 0x0408
    unsigned long ExperienceGeneralPoolLabel;                 // 0x040c
    C2DVector ExperienceGeneralPoolSpendTextOffset;           // 0x0410
    C2DVector ExperienceStrengthPoolTLPos;                    // 0x0418
    C2DVector ExperienceSkillPoolTLPos;                       // 0x0420
    C2DVector ExperienceWillPoolTLPos;                        // 0x0428
    float ExperiencePoolWidth;                                // 0x0430
    float ExperiencePoolHeight;                               // 0x0434
    unsigned long ExperienceTypeStrengthLabel;                // 0x0438
    unsigned long ExperienceTypeSkillLabel;                   // 0x043c
    unsigned long ExperienceTypeWillLabel;                    // 0x0440
    unsigned long ExperienceTypePureMagicLabel;               // 0x0444
    unsigned long ExperienceTypeAbilityMagicLabel;            // 0x0448
    unsigned long ExperienceTypeWeaponMagicLabel;             // 0x044c
    C2DVector ExperienceTypeTLPos;                            // 0x0450
    C2DVector ExperienceTypeTLPosOffset;                      // 0x0458
    C2DVector ExperienceTypeTextOffset;                       // 0x0460
    C2DVector ExperienceTypeSpendTextOffset;                  // 0x0468
    float ExperienceTypeLabelWidth;                           // 0x0470
    float ExperienceTypeLabelHeight;                          // 0x0474
    float ExperienceTypeSliderWidth;                          // 0x0478
    unsigned long ExperienceStrengthPhysiqueLabel;            // 0x047c
    unsigned long ExperienceStrengthHealthLabel;              // 0x0480
    unsigned long ExperienceStrengthToughnessLabel;           // 0x0484
    unsigned long ExperienceSkillSpeedLabel;                  // 0x0488
    unsigned long ExperienceSkillAccuracyLabel;               // 0x048c
    unsigned long ExperienceSkillStealthLabel;                // 0x0490
    unsigned long ExperienceWeaponMagicBerserkLabel;          // 0x0494
    unsigned long ExperienceWeaponMagicFlamingBladeLabel;     // 0x0498
    unsigned long ExperienceWeaponMagicDoubleStrikeLabel;     // 0x049c
    unsigned long ExperienceWeaponMagicLightningBladeLabel;   // 0x04a0
    unsigned long ExperienceAbilityMagicBattleChargeLabel;    // 0x04a4
    unsigned long ExperienceAbilityMagicAssassinRushLabel;    // 0x04a8
    unsigned long ExperienceAbilityMagicTimeSpellLabel;       // 0x04ac
    unsigned long ExperiencePureMagicBurningHandsLabel;       // 0x04b0
    unsigned long ExperiencePureMagicPhysicalShieldLabel;     // 0x04b4
    unsigned long ExperiencePureMagicForcePushLabel;          // 0x04b8
    unsigned long ExperiencePureMagicEnflameSpellLabel;       // 0x04bc
    unsigned long ExperiencePureMagicDrainLifeLabel;          // 0x04c0
    unsigned long ExperiencePureMagicHealLifeLabel;           // 0x04c4
    unsigned long ExperiencePureMagicGhostSwordLabel;         // 0x04c8
    float ExperienceStrengthBarValue;                         // 0x04cc
    float ExperienceSkillBarValue;                            // 0x04d0
    float ExperienceWillBarValue;                             // 0x04d4
    float ExperienceStrengthPhysiqueBarValue;                 // 0x04d8
    float ExperienceStrengthHealthBarValue;                   // 0x04dc
    float ExperienceStrengthToughnessBarValue;                // 0x04e0
    float ExperienceSkillSpeedBarValue;                       // 0x04e4
    float ExperienceSkillAccuracyBarValue;                    // 0x04e8
    float ExperienceSkillHealthBarValue;                      // 0x04ec
    float ExperienceWeaponMagicBerserkBarValue;               // 0x04f0
    float ExperienceWeaponMagicFlamingBladeBarValue;          // 0x04f4
    float ExperienceWeaponMagicDoubleStrikeBarValue;          // 0x04f8
    float ExperienceAbilityMagicBattleChargeBarValue;         // 0x04fc
    float ExperienceAbilityMagicAssassinRushBarValue;         // 0x0500
    float ExperienceAbilityMagicTimeSpellBarValue;            // 0x0504
    float ExperiencePureMagicBurningHandsBarValue;            // 0x0508
    float ExperiencePureMagicPhysicalShieldBarValue;          // 0x050c
    float ExperiencePureMagicForcePushBarValue;               // 0x0510
    CEngineGraphic ExperienceProgBarL;                        // 0x0514
    CEngineGraphic ExperienceProgBarC;                        // 0x0522
    CEngineGraphic ExperienceProgBarR;                        // 0x0530
    CEngineGraphic ExperienceProgBarValue;                    // 0x053e
    C2DVector ExperienceProgBarValueOffset;                   // 0x054c
    long ExperienceSpendTimeOffset;                           // 0x0554
    long ExperienceSpendRate;                                 // 0x0558
    std::vector<long> ExperienceItemLabelStat;                // 0x055c
    std::vector<long> ExperienceItemLabelAbility;             // 0x0568
    CEngineGraphic ExperienceSpendGreen;                      // 0x0574
    CEngineGraphic ExperienceSpendGreenOutline;               // 0x0582
    CEngineGraphic ExperienceSpendGreenEnd;                   // 0x0590
    CEngineGraphic ExperienceSpendRed;                        // 0x059e
    CEngineGraphic ExperienceSpendRedOutline;                 // 0x05ac
    CEngineGraphic ExperienceSpendRedEnd;                     // 0x05ba
    C2DVector ExperienceDescriptionTextTLPos;                 // 0x05c8
    C2DVector ExperienceDescriptionTextOffset;                // 0x05d0
    unsigned long QuestCardInfoQuestNameLabel;                // 0x05d8
    unsigned long QuestCardInfoMoralityLabel;                 // 0x05dc
    unsigned long QuestCardInfoCoreLabel;                     // 0x05e0
    unsigned long QuestCardInfoDetailsLabel;                  // 0x05e4
    unsigned long QuestCardInfoRegionLabel;                   // 0x05e8
    unsigned long QuestCardInfoOriginatorLabel;               // 0x05ec
    unsigned long QuestCardInfoDescriptionLabel;              // 0x05f0
    unsigned long QuestCardInfoSuccessLabel;                  // 0x05f4
    unsigned long QuestCardInfoFailureLabel;                  // 0x05f8
    unsigned long QuestCardInfoRewardsLabel;                  // 0x05fc
    unsigned long QuestCardInfoRenownLabel;                   // 0x0600
    unsigned long QuestCardInfoTrophyLabel;                   // 0x0604
    unsigned long QuestCardInfoBoastsLabel;                   // 0x0608
    unsigned long QuestCardInfoStatusLabel;                   // 0x060c
    unsigned long QuestCardInfoQuestNameText;                 // 0x0610
    unsigned long QuestCardInfoMoralityText;                  // 0x0614
    unsigned long QuestCardInfoCoreText;                      // 0x0618
    unsigned long QuestCardInfoDetailsText;                   // 0x061c
    unsigned long QuestCardInfoRegionText;                    // 0x0620
    unsigned long QuestCardInfoOriginatorText;                // 0x0624
    unsigned long QuestCardInfoDescriptionText;               // 0x0628
    unsigned long QuestCardInfoSuccessText;                   // 0x062c
    unsigned long QuestCardInfoFailureText;                   // 0x0630
    unsigned long QuestCardInfoRewardsText;                   // 0x0634
    unsigned long QuestCardInfoRenownText;                    // 0x0638
    unsigned long QuestCardInfoTrophyText;                    // 0x063c
    unsigned long QuestCardInfoBoastsText;                    // 0x0640
    unsigned long QuestCardInfoStatusText;                    // 0x0644
    C2DVector TradeEdgedButtonCostTLPos;                      // 0x0648
    C2DVector TradeEdgedButtonOwnedTLPos;                     // 0x0650
    C2DVector TradeEdgedButtonProfitTLPos;                    // 0x0658
    C2DVector TradeEdgedButtonTitleTLPos;                     // 0x0660
    C2DVector TradeEdgedButtonCashTLPos;                      // 0x0668
    C2DVector TradeEdgedButtonExtents;                        // 0x0670
    C2DVector TradeAnimImageBoxTLPos;                         // 0x0678
    C2DVector TradeAnimImageBoxExtents;                       // 0x0680
    float TradeAnimImageXRotationTime;                        // 0x0688
    float TradeAnimImageYRotationTime;                        // 0x068c
    float TradeAnimImageZRotationTime;                        // 0x0690
    C2DVector TradeEquippedItemTLPos;                         // 0x0694
    C2DVector TradeEquippedItemExtents;                       // 0x069c
    CEngineGraphic TradeEquippedItemGraphic;                  // 0x06a4
    C2DVector TradeDescriptionBoxTLPos;                       // 0x06b2
    C2DVector TradeDescriptionBoxExtents;                     // 0x06ba
    unsigned long TradeTextOwned;                             // 0x06c4
    unsigned long TradeTextDivider;                           // 0x06c8
    unsigned long TradeTextCost;                              // 0x06cc
    unsigned long TradeTextGold;                              // 0x06d0
    unsigned long TradeTextProfit;                            // 0x06d4
    C2DVector TradeShadedCircleTLPos;                         // 0x06d8
    C2DVector TradeShadedCircleExtents;                       // 0x06e0
    C2DVector AugmentationMeshAreaTLPos;                      // 0x06e8
    C2DVector AugmentationMeshAreaBRPos;                      // 0x06f0
    C2DVector AugmentDescriptionBoxTLPos;                     // 0x06f8
    C2DVector AugmentDescriptionBoxExtents;                   // 0x0700
    C2DVector AugmentAugmentationInfoTLPos;                   // 0x0708
    C2DVector AugmentAugmentationLabelTLPos;                  // 0x0710
    C2DVector AugmentAugmentationInfoTextOffset;              // 0x0718
    C2DVector AugmentAugmentationSlotTLPos;                   // 0x0720
    C2DVector AugmentAugmentationSlotTextOffset;              // 0x0728
    C2DVector AugmentAugmentationWeaponTLPos;                 // 0x0730
    C2DVector AugmentAugmentationWeaponTextOffset;            // 0x0738
    C2DVector AugmentTitleTLPos;                              // 0x0740
    C2DVector AugmentTitleTextOffset;                         // 0x0748
    unsigned long AugmentTextMaxSlots;                        // 0x0750
    unsigned long AugmentTextNumSlots;                        // 0x0754
    unsigned long AugmentTextUpgradeCost;                     // 0x0758
    unsigned long AugmentTextGold;                            // 0x075c
    unsigned long AugmentTextDamageMultiplier;                // 0x0760
    unsigned long AugmentTextFireStrength;                    // 0x0764
    unsigned long AugmentTextSlot;                            // 0x0768
    unsigned long AugmentTextAugmentations;                   // 0x076c
    unsigned long AugmentTextWeapons;                         // 0x0770
    unsigned long AugmentTextDivider;                         // 0x0774
    C2DVector AugmentShadedCircleTLPos;                       // 0x0778
    C2DVector AugmentShadedCircleExtents;                     // 0x0780
    float InventoryAnimImageXRotationTime;                    // 0x0788
    float InventoryAnimImageYRotationTime;                    // 0x078c
    float InventoryAnimImageZRotationTime;                    // 0x0790
    CRGBColour QuestCardFeatColour;                           // 0x0794
    CRGBColour QuestCardCoreColour;                           // 0x0798
    CRGBColour QuestCardExclusiveColour;                      // 0x079c
    C2DVector QuestCardInfoTLPos;                             // 0x07a0
    C2DVector QuestCardInfo2TLPos;                            // 0x07a8
    C2DVector QuestCardNoQuestsTLPos;                         // 0x07b0
    unsigned long QuestCardNoQuestsText;                      // 0x07b8
    C2DVector QuestCardInfoTextOffset;                        // 0x07bc
    C2DVector QuestCardEdgedButtonTitleTLPos;                 // 0x07c4
    C2DVector QuestCardEdgedButtonExtents;                    // 0x07cc
    std::vector<long> ExpSpendItemLabelStat;                  // 0x07d4
    std::vector<long> ExpSpendItemLabelAbility;               // 0x07e0
    long ExpSpendMaxItemsToDisplay;                           // 0x07ec
    C2DVector ExpSpendButtonTLPos;                            // 0x07f0
    C2DVector ExpSpendButtonTLPosOffset;                      // 0x07f8
    C2DVector ExpSpendButtonTextOffset;                       // 0x0800
    CEngineGraphic ExpSpendButtonSelL;                        // 0x0808
    CEngineGraphic ExpSpendButtonSelC;                        // 0x0816
    CEngineGraphic ExpSpendButtonSelR;                        // 0x0824
    CEngineGraphic ExpSpendButtonL;                           // 0x0832
    CEngineGraphic ExpSpendButtonC;                           // 0x0840
    CEngineGraphic ExpSpendButtonR;                           // 0x084e
    C2DVector ExpSpendDescriptionBoxTLPos;                    // 0x085c
    C2DVector ExpSpendDescriptionBoxExtents;                  // 0x0864
    C2DVector ExpSpendArrowUpTLPos;                           // 0x086c
    C2DVector ExpSpendArrowDnTLPos;                           // 0x0874
    C2DVector ExpSpendGeneralPoolLabelTLPos;                  // 0x087c
    C2DVector ExpSpendGeneralPoolLabelTextOffset;             // 0x0884
    C2DVector ExpSpendGeneralPoolSpendTextOffset;             // 0x088c
    float ExpSpendGeneralPoolWidth;                           // 0x0894
    float ExpSpendGeneralPoolSliderWidth;                     // 0x0898
    C2DVector ExpSpendStrengthPoolTLPos;                      // 0x089c
    C2DVector ExpSpendSkillPoolTLPos;                         // 0x08a4
    C2DVector ExpSpendWillPoolTLPos;                          // 0x08ac
    float ExpSpendPoolWidth;                                  // 0x08b4
    float ExpSpendPoolHeight;                                 // 0x08b8
    CEngineGraphic ExpSpendProgBarL;                          // 0x08bc
    CEngineGraphic ExpSpendProgBarC;                          // 0x08ca
    CEngineGraphic ExpSpendProgBarR;                          // 0x08d8
    CEngineGraphic ExpSpendProgBarValue;                      // 0x08e6
    C2DVector ExpSpendProgBarValueOffset;                     // 0x08f4
    C2DVector ExpSpendTypeTextOffset;                         // 0x08fc
    long ExpSpendGeneralPoolBarValue;                         // 0x0904
    long ExpSpendGeneralPoolLabel;                            // 0x0908
    long ExpSpendTypeStrengthLabel;                           // 0x090c
    long ExpSpendTypeSkillLabel;                              // 0x0910
    long ExpSpendTypeWillLabel;                               // 0x0914
    float ExpSpendStrengthBarValue;                           // 0x0918
    float ExpSpendSkillBarValue;                              // 0x091c
    float ExpSpendWillBarValue;                               // 0x0920
    long ExpSpendRate;                                        // 0x0924
    long ExpSpendTimeOffset;                                  // 0x0928
    CEngineGraphic ExpSpendGreen;                             // 0x092c
    CEngineGraphic ExpSpendGreenOutline;                      // 0x093a
    CEngineGraphic ExpSpendGreenEnd;                          // 0x0948
    CEngineGraphic ExpSpendRed;                               // 0x0956
    CEngineGraphic ExpSpendRedOutline;                        // 0x0964
    CEngineGraphic ExpSpendRedEnd;                            // 0x0972
    C2DVector ExpSpendDescriptionTextTLPos;                   // 0x0980
    C2DVector ExpSpendDescriptionTextOffset;                  // 0x0988
    C2DVector MainEdgedButtonTitleTLPos;                      // 0x0990
    C2DVector MainEdgedButtonOwnedTLPos;                      // 0x0998
    C2DVector MainEdgedButtonPriceTLPos;                      // 0x09a0
    C2DVector MainEdgedButtonCostTLPos;                       // 0x09a8
    C2DVector MainEdgedButtonExtents;                         // 0x09b0
    CEngineGraphic MainDescriptionBoxC;                       // 0x09b8
    CEngineGraphic MainDescriptionBoxTL;                      // 0x09c6
    CEngineGraphic MainDescriptionBoxTR;                      // 0x09d4
    CEngineGraphic MainDescriptionBoxBL;                      // 0x09e2
    CEngineGraphic MainDescriptionBoxBR;                      // 0x09f0
    C2DVector MainDescriptionBoxTLPos;                        // 0x09fe
    C2DVector MainDescriptionBoxExtents;                      // 0x0a06
    C2DVector MainShadedCircleTLPos;                          // 0x0a0e
    C2DVector MainShadedCircleExtents;                        // 0x0a16
    C2DVector MainAnimBoxTLPos;                               // 0x0a1e
    C2DVector MainAnimBoxExtents;                             // 0x0a26
    float MainQuickAccessItemFlashTime;                       // 0x0a30
    CInventoryDef& operator=(const CInventoryDef& other);     // 0x0045A159
#endif
};
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialCategoryMovementRepeatDelay) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, InitialInventoryClothingItems) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaTLPos) == 0xA4);
FABLE_STATIC_ASSERT(offsetof(CInventoryDef, MeshAreaBRPos) == 0xAC);

#endif // FABLE_ENGINE_CINVENTORYDEF_H
