// Behaviour test for CInventoryDef::Transfer (retail 0x004678E2): a copy of the retail-layout model (packed C2DVector /
// CEngineGraphic, 0x39-byte def base, 12-byte STLport vectors) and the 389-statement transfer list; a recording
// CPersistContext checks the member order (first +0x3C, last +0xA30, TradeShadedCircleTLPos twice, never
// TradeShadedCircleExtents), and a write/read round trip copies every transferred member into a fresh object.
#include <cstdio>
#include <cstring>
#include <stddef.h>
#include <new>
#pragma pack(push, 1)
class C2DVector { public: float X, Y; };
class CEngineGraphic { public: unsigned char b[14]; };
struct DefBase { virtual ~DefBase() {} unsigned char pad[0x35]; };
#pragma pack(pop)
class C3DVector { public: float X, Y, Z; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CDefString { public: long TablePos; };
namespace NInventory { enum EInventoryIdentifier { II_A, II_B, II_C }; }
struct VecLong { long* a; long* b; long* c; };
struct VecULong { unsigned long* a; unsigned long* b; unsigned long* c; };
struct VecRGB { CRGBColour* a; CRGBColour* b; CRGBColour* c; };
class CPersistContext;
class CInventoryDef : public DefBase {
public:
    float InitialCategoryMovementRepeatDelay;   // +0x3C
    float ContinuedCategoryMovementRepeatDelay;   // +0x40
    float InitialItemMovementRepeatDelay;   // +0x44
    float ContinuedItemMovementRepeatDelay;   // +0x48
    long MovementSound;   // +0x4C
    long NoMovementSound;   // +0x50
    long InventoryOpenSound;   // +0x54
    bool TrackSelectedItems;   // +0x58
    VecLong InitialInventoryClothingItems;   // +0x5C
    VecLong InitialInventoryWeaponsItems;   // +0x68
    VecLong InitialInventoryStatsItems;   // +0x74
    VecLong InitialInventoryStandardItems;   // +0x80
    C2DVector ClothingShadedCircleTLPos;   // +0x8C
    C2DVector ClothingShadedCircleExtents;   // +0x94
    C2DVector CategoriesTLPos;   // +0x9C
    C2DVector MeshAreaTLPos;   // +0xA4
    C2DVector MeshAreaBRPos;   // +0xAC
    C2DVector NextCategoryOffset;   // +0xB4
    C2DVector FirstItemOffsetFromCategory;   // +0xBC
    C2DVector NextItemOffset;   // +0xC4
    CDefString GenericFont;   // +0xCC
    CRGBColour BackGroundColour;   // +0xD0
    long ClothingDoll;   // +0xD4
    C3DVector DollCentreOffset;   // +0xD8
    float DollBoundingSphereRadius;   // +0xE4
    C2DVector TotalExperienceTextPos;   // +0xE8
    unsigned long TotalExperienceText;   // +0xF0
    C2DVector SpendingExperienceTextPos;   // +0xF4
    unsigned long SpendingExperienceText;   // +0xFC
    C2DVector StrengthLevelTextPos;   // +0x100
    CRGBColour StrengthLevelColour;   // +0x108
    C2DVector SkillLevelTextPos;   // +0x10C
    CRGBColour SkillLevelColour;   // +0x114
    C2DVector WillLevelTextPos;   // +0x118
    CRGBColour WillLevelColour;   // +0x120
    C2DVector RenownLevelTextPos;   // +0x124
    C2DVector HeroLevelTextPos;   // +0x12C
    C2DVector StatLevelsFirstLineTextPos;   // +0x134
    C2DVector StatLevelsNextLineTextOffset;   // +0x13C
    C2DVector ButtonAssignmentTextPos;   // +0x144
    C2DVector ButtonAssignmentTextOffset;   // +0x14C
    VecULong StatLevelsTexts;   // +0x154
    VecRGB StatLevelsColours;   // +0x160
    long WeaponsTextSeparator;   // +0x16C
    long WeaponsTextNumberOfSlots;   // +0x170
    long WeaponsTextSlot;   // +0x174
    long WeaponsTextSuccessfulAugmentation;   // +0x178
    long WeaponsTextNoFreeSlots;   // +0x17C
    long WeaponsTextReadyToAugment;   // +0x180
    long WeaponsTextNotReadyToAugment;   // +0x184
    C2DVector WeaponsDescriptionBoxTLPos;   // +0x188
    C2DVector WeaponsDescriptionBoxExtents;   // +0x190
    C2DVector WeaponsActiveMeleeIconPos;   // +0x198
    C2DVector WeaponsActiveRangeIconPos;   // +0x1A0
    C2DVector WeaponAugmentationTextStartPos;   // +0x1A8
    C2DVector WeaponAugmentationTextNewLineOffset;   // +0x1B0
    C2DVector AugmentationStatusTextAreaTLPos;   // +0x1B8
    C2DVector AugmentationStatusTextAreaBRPos;   // +0x1C0
    float TimeToDisplayWarningMessageInSeconds;   // +0x1C8
    C2DVector WeaponsShadedCircleTLPos;   // +0x1CC
    C2DVector WeaponsShadedCircleExtents;   // +0x1D4
    float NumberOfSecondsForObjectRotation;   // +0x1DC
    long MaxNumberOfItemsToDisplayPerCategory;   // +0x1E0
    long MaxNumberOfCategoriesToDisplay;   // +0x1E4
    long MaxNumberOfScreenNamesToDisplay;   // +0x1E8
    unsigned long InventoryName;   // +0x1EC
    NInventory::EInventoryIdentifier InventoryIdentifier;   // +0x1F0
    C2DVector MapScreenMapDisplayBoxTLPos;   // +0x1F4
    C2DVector MapScreenMapDisplayBoxBRPos;   // +0x1FC
    C2DVector MapScreenRegionNamePos;   // +0x204
    long MapScreenWorldMapGraphic;   // +0x20C
    C2DVector MapScreenWorldMapPos;   // +0x210
    long MapScreenRegionUnknownGraphic;   // +0x218
    long MapScreenRegionUnknownHighlightGraphic;   // +0x21C
    long MapScreenRegionHeroGraphic;   // +0x220
    long MapScreenRegionQuestGraphic;   // +0x224
    long MapScreenRegionTeleportGraphic;   // +0x228
    long MapScreenRegionHouseGraphic;   // +0x22C
    C2DVector StatsTitleTLPos;   // +0x230
    C2DVector StatsShadedCircleTLPos;   // +0x238
    C2DVector StatsShadedCircleExtents;   // +0x240
    CEngineGraphic StatsProgBarL;   // +0x248
    CEngineGraphic StatsProgBarC;   // +0x256
    CEngineGraphic StatsProgBarR;   // +0x264
    CEngineGraphic StatsProgBarValue;   // +0x272
    C2DVector StatsProgBarValueOffset;   // +0x280
    C2DVector StatsPersonalityHeadingTLPos;   // +0x288
    C2DVector StatsPersonalityHeadingBRPos;   // +0x290
    unsigned long StatsPersonalityRenownLowerLabel;   // +0x298
    unsigned long StatsPersonalityRenownUpperLabel;   // +0x29C
    unsigned long StatsPersonalityGoodnessLowerLabel;   // +0x2A0
    unsigned long StatsPersonalityGoodnessUpperLabel;   // +0x2A4
    unsigned long StatsPersonalityAttractivenessLowerLabel;   // +0x2A8
    unsigned long StatsPersonalityAttractivenessUpperLabel;   // +0x2AC
    unsigned long StatsPersonalityScarinessLowerLabel;   // +0x2B0
    unsigned long StatsPersonalityScarinessUpperLabel;   // +0x2B4
    unsigned long StatsPersonalityNicenessLowerLabel;   // +0x2B8
    unsigned long StatsPersonalityNicenessUpperLabel;   // +0x2BC
    C2DVector StatsPersonalityLabelTextOffset;   // +0x2C0
    C2DVector StatsPersonalityLowerLabelTLPos;   // +0x2C8
    float StatsPersonalityLowerLabelWidth;   // +0x2D0
    float StatsPersonalityLowerLabelHeight;   // +0x2D4
    float StatsPersonalitySliderWidth;   // +0x2D8
    float StatsPersonalityUpperLabelWidth;   // +0x2DC
    long StatsPersonalityDoll;   // +0x2E0
    C3DVector StatsPersonalityDollCentreOffset;   // +0x2E4
    float StatsPersonalityDollBoundingSphereRadius;   // +0x2F0
    C2DVector StatsStatisticsLabelTLPos;   // +0x2F4
    float StatsStatisticsLabelWidth;   // +0x2FC
    float StatsStatisticsLabelHeight;   // +0x300
    C2DVector StatsStatisticsLabelTextOffset;   // +0x304
    C2DVector StatsStatisticsDataTextOffset;   // +0x30C
    C2DVector StatsStatisticsHeadingTLPos;   // +0x314
    C2DVector StatsStatisticsHeadingBRPos;   // +0x31C
    unsigned long StatsStatisticsPeopleKilledLabel;   // +0x324
    unsigned long StatsStatisticsMonstersKilledLabel;   // +0x328
    unsigned long StatsStatisticsMaxMultiplierLabel;   // +0x32C
    unsigned long StatsStatisticsPreferredFoodLabel;   // +0x330
    unsigned long StatsStatisticsDietaryTypeLabel;   // +0x334
    unsigned long StatsStatisticsGameTimeElapsedLabel;   // +0x338
    unsigned long StatsStatisticsNumberOfWivesLabel;   // +0x33C
    unsigned long StatsStatisticsQuestsActiveLabel;   // +0x340
    unsigned long StatsStatisticsQuestsFinishedLabel;   // +0x344
    unsigned long StatsStatisticsBoastsAchievedLabel;   // +0x348
    C2DVector StatsExperienceStrengthTextOffset;   // +0x34C
    C2DVector StatsExperienceWillTextOffset;   // +0x354
    C2DVector StatsExperienceSkillTextOffset;   // +0x35C
    unsigned long StatsExperienceStrengthText;   // +0x364
    unsigned long StatsExperienceWillText;   // +0x368
    unsigned long StatsExperienceSkillText;   // +0x36C
    C2DVector StatsExperiencePoolGeneralTLPos;   // +0x370
    C2DVector StatsExperiencePoolGeneralTextOffset;   // +0x378
    C2DVector StatsExperiencePoolStrengthGroupTLPos;   // +0x380
    C2DVector StatsExperiencePoolSkillGroupTLPos;   // +0x388
    C2DVector StatsExperiencePoolWillGroupTLPos;   // +0x390
    C2DVector StatsExperiencePoolLabelTLPos;   // +0x398
    C2DVector StatsExperiencePoolLabelTextOffset;   // +0x3A0
    float StatsExperiencePoolLabelWidth;   // +0x3A8
    float StatsExperiencePoolLabelHeight;   // +0x3AC
    float StatsExperiencePoolSliderWidth;   // +0x3B0
    unsigned long StatsExperiencePoolStrengthLabel;   // +0x3B4
    unsigned long StatsExperiencePoolSkillLabel;   // +0x3B8
    unsigned long StatsExperiencePoolWillLabel;   // +0x3BC
    unsigned long StatsExperiencePoolPhysiqueLabel;   // +0x3C0
    unsigned long StatsExperiencePoolHealthLabel;   // +0x3C4
    unsigned long StatsExperiencePoolToughnessLabel;   // +0x3C8
    unsigned long StatsExperiencePoolSpeedLabel;   // +0x3CC
    unsigned long StatsExperiencePoolAccuracyLabel;   // +0x3D0
    unsigned long StatsExperiencePoolStealthLabel;   // +0x3D4
    unsigned long StatsExperiencePoolWeaponMagicLabel;   // +0x3D8
    unsigned long StatsExperiencePoolAbilityMagicLabel;   // +0x3DC
    unsigned long StatsExperiencePoolPureMagicLabel;   // +0x3E0
    float StatsExperienceStrengthBarValue;   // +0x3E4
    float StatsExperienceSkillBarValue;   // +0x3E8
    float StatsExperienceWillBarValue;   // +0x3EC
    C2DVector ExperienceGeneralPoolLabelTLPos;   // +0x3F0
    C2DVector ExperienceGeneralPoolLabelTextOffset;   // +0x3F8
    float ExperienceGeneralPoolWidth;   // +0x400
    float ExperienceGeneralPoolSliderWidth;   // +0x404
    unsigned long ExperienceGeneralPoolBarValue;   // +0x408
    unsigned long ExperienceGeneralPoolLabel;   // +0x40C
    C2DVector ExperienceGeneralPoolSpendTextOffset;   // +0x410
    C2DVector ExperienceStrengthPoolTLPos;   // +0x418
    C2DVector ExperienceSkillPoolTLPos;   // +0x420
    C2DVector ExperienceWillPoolTLPos;   // +0x428
    float ExperiencePoolWidth;   // +0x430
    float ExperiencePoolHeight;   // +0x434
    unsigned long ExperienceTypeStrengthLabel;   // +0x438
    unsigned long ExperienceTypeSkillLabel;   // +0x43C
    unsigned long ExperienceTypeWillLabel;   // +0x440
    unsigned long ExperienceTypePureMagicLabel;   // +0x444
    unsigned long ExperienceTypeAbilityMagicLabel;   // +0x448
    unsigned long ExperienceTypeWeaponMagicLabel;   // +0x44C
    C2DVector ExperienceTypeTLPos;   // +0x450
    C2DVector ExperienceTypeTLPosOffset;   // +0x458
    C2DVector ExperienceTypeTextOffset;   // +0x460
    C2DVector ExperienceTypeSpendTextOffset;   // +0x468
    float ExperienceTypeLabelWidth;   // +0x470
    float ExperienceTypeLabelHeight;   // +0x474
    float ExperienceTypeSliderWidth;   // +0x478
    unsigned long ExperienceStrengthPhysiqueLabel;   // +0x47C
    unsigned long ExperienceStrengthHealthLabel;   // +0x480
    unsigned long ExperienceStrengthToughnessLabel;   // +0x484
    unsigned long ExperienceSkillSpeedLabel;   // +0x488
    unsigned long ExperienceSkillAccuracyLabel;   // +0x48C
    unsigned long ExperienceSkillStealthLabel;   // +0x490
    unsigned long ExperienceWeaponMagicBerserkLabel;   // +0x494
    unsigned long ExperienceWeaponMagicFlamingBladeLabel;   // +0x498
    unsigned long ExperienceWeaponMagicDoubleStrikeLabel;   // +0x49C
    unsigned long ExperienceWeaponMagicLightningBladeLabel;   // +0x4A0
    unsigned long ExperienceAbilityMagicBattleChargeLabel;   // +0x4A4
    unsigned long ExperienceAbilityMagicAssassinRushLabel;   // +0x4A8
    unsigned long ExperienceAbilityMagicTimeSpellLabel;   // +0x4AC
    unsigned long ExperiencePureMagicBurningHandsLabel;   // +0x4B0
    unsigned long ExperiencePureMagicPhysicalShieldLabel;   // +0x4B4
    unsigned long ExperiencePureMagicForcePushLabel;   // +0x4B8
    unsigned long ExperiencePureMagicEnflameSpellLabel;   // +0x4BC
    unsigned long ExperiencePureMagicDrainLifeLabel;   // +0x4C0
    unsigned long ExperiencePureMagicHealLifeLabel;   // +0x4C4
    unsigned long ExperiencePureMagicGhostSwordLabel;   // +0x4C8
    float ExperienceStrengthBarValue;   // +0x4CC
    float ExperienceSkillBarValue;   // +0x4D0
    float ExperienceWillBarValue;   // +0x4D4
    float ExperienceStrengthPhysiqueBarValue;   // +0x4D8
    float ExperienceStrengthHealthBarValue;   // +0x4DC
    float ExperienceStrengthToughnessBarValue;   // +0x4E0
    float ExperienceSkillSpeedBarValue;   // +0x4E4
    float ExperienceSkillAccuracyBarValue;   // +0x4E8
    float ExperienceSkillHealthBarValue;   // +0x4EC
    float ExperienceWeaponMagicBerserkBarValue;   // +0x4F0
    float ExperienceWeaponMagicFlamingBladeBarValue;   // +0x4F4
    float ExperienceWeaponMagicDoubleStrikeBarValue;   // +0x4F8
    float ExperienceAbilityMagicBattleChargeBarValue;   // +0x4FC
    float ExperienceAbilityMagicAssassinRushBarValue;   // +0x500
    float ExperienceAbilityMagicTimeSpellBarValue;   // +0x504
    float ExperiencePureMagicBurningHandsBarValue;   // +0x508
    float ExperiencePureMagicPhysicalShieldBarValue;   // +0x50C
    float ExperiencePureMagicForcePushBarValue;   // +0x510
    CEngineGraphic ExperienceProgBarL;   // +0x514
    CEngineGraphic ExperienceProgBarC;   // +0x522
    CEngineGraphic ExperienceProgBarR;   // +0x530
    CEngineGraphic ExperienceProgBarValue;   // +0x53E
    C2DVector ExperienceProgBarValueOffset;   // +0x54C
    long ExperienceSpendTimeOffset;   // +0x554
    long ExperienceSpendRate;   // +0x558
    VecLong ExperienceItemLabelStat;   // +0x55C
    VecLong ExperienceItemLabelAbility;   // +0x568
    CEngineGraphic ExperienceSpendGreen;   // +0x574
    CEngineGraphic ExperienceSpendGreenOutline;   // +0x582
    CEngineGraphic ExperienceSpendGreenEnd;   // +0x590
    CEngineGraphic ExperienceSpendRed;   // +0x59E
    CEngineGraphic ExperienceSpendRedOutline;   // +0x5AC
    CEngineGraphic ExperienceSpendRedEnd;   // +0x5BA
    C2DVector ExperienceDescriptionTextTLPos;   // +0x5C8
    C2DVector ExperienceDescriptionTextOffset;   // +0x5D0
    unsigned long QuestCardInfoQuestNameLabel;   // +0x5D8
    unsigned long QuestCardInfoMoralityLabel;   // +0x5DC
    unsigned long QuestCardInfoCoreLabel;   // +0x5E0
    unsigned long QuestCardInfoDetailsLabel;   // +0x5E4
    unsigned long QuestCardInfoRegionLabel;   // +0x5E8
    unsigned long QuestCardInfoOriginatorLabel;   // +0x5EC
    unsigned long QuestCardInfoDescriptionLabel;   // +0x5F0
    unsigned long QuestCardInfoSuccessLabel;   // +0x5F4
    unsigned long QuestCardInfoFailureLabel;   // +0x5F8
    unsigned long QuestCardInfoRewardsLabel;   // +0x5FC
    unsigned long QuestCardInfoRenownLabel;   // +0x600
    unsigned long QuestCardInfoTrophyLabel;   // +0x604
    unsigned long QuestCardInfoBoastsLabel;   // +0x608
    unsigned long QuestCardInfoStatusLabel;   // +0x60C
    unsigned long QuestCardInfoQuestNameText;   // +0x610
    unsigned long QuestCardInfoMoralityText;   // +0x614
    unsigned long QuestCardInfoCoreText;   // +0x618
    unsigned long QuestCardInfoDetailsText;   // +0x61C
    unsigned long QuestCardInfoRegionText;   // +0x620
    unsigned long QuestCardInfoOriginatorText;   // +0x624
    unsigned long QuestCardInfoDescriptionText;   // +0x628
    unsigned long QuestCardInfoSuccessText;   // +0x62C
    unsigned long QuestCardInfoFailureText;   // +0x630
    unsigned long QuestCardInfoRewardsText;   // +0x634
    unsigned long QuestCardInfoRenownText;   // +0x638
    unsigned long QuestCardInfoTrophyText;   // +0x63C
    unsigned long QuestCardInfoBoastsText;   // +0x640
    unsigned long QuestCardInfoStatusText;   // +0x644
    C2DVector TradeEdgedButtonCostTLPos;   // +0x648
    C2DVector TradeEdgedButtonOwnedTLPos;   // +0x650
    C2DVector TradeEdgedButtonProfitTLPos;   // +0x658
    C2DVector TradeEdgedButtonTitleTLPos;   // +0x660
    C2DVector TradeEdgedButtonCashTLPos;   // +0x668
    C2DVector TradeEdgedButtonExtents;   // +0x670
    C2DVector TradeAnimImageBoxTLPos;   // +0x678
    C2DVector TradeAnimImageBoxExtents;   // +0x680
    float TradeAnimImageXRotationTime;   // +0x688
    float TradeAnimImageYRotationTime;   // +0x68C
    float TradeAnimImageZRotationTime;   // +0x690
    C2DVector TradeEquippedItemTLPos;   // +0x694
    C2DVector TradeEquippedItemExtents;   // +0x69C
    CEngineGraphic TradeEquippedItemGraphic;   // +0x6A4
    C2DVector TradeDescriptionBoxTLPos;   // +0x6B2
    C2DVector TradeDescriptionBoxExtents;   // +0x6BA
    unsigned long TradeTextOwned;   // +0x6C4
    unsigned long TradeTextDivider;   // +0x6C8
    unsigned long TradeTextCost;   // +0x6CC
    unsigned long TradeTextGold;   // +0x6D0
    unsigned long TradeTextProfit;   // +0x6D4
    C2DVector TradeShadedCircleTLPos;   // +0x6D8
    C2DVector TradeShadedCircleExtents;   // +0x6E0
    C2DVector AugmentationMeshAreaTLPos;   // +0x6E8
    C2DVector AugmentationMeshAreaBRPos;   // +0x6F0
    C2DVector AugmentDescriptionBoxTLPos;   // +0x6F8
    C2DVector AugmentDescriptionBoxExtents;   // +0x700
    C2DVector AugmentAugmentationInfoTLPos;   // +0x708
    C2DVector AugmentAugmentationLabelTLPos;   // +0x710
    C2DVector AugmentAugmentationInfoTextOffset;   // +0x718
    C2DVector AugmentAugmentationSlotTLPos;   // +0x720
    C2DVector AugmentAugmentationSlotTextOffset;   // +0x728
    C2DVector AugmentAugmentationWeaponTLPos;   // +0x730
    C2DVector AugmentAugmentationWeaponTextOffset;   // +0x738
    C2DVector AugmentTitleTLPos;   // +0x740
    C2DVector AugmentTitleTextOffset;   // +0x748
    unsigned long AugmentTextMaxSlots;   // +0x750
    unsigned long AugmentTextNumSlots;   // +0x754
    unsigned long AugmentTextUpgradeCost;   // +0x758
    unsigned long AugmentTextGold;   // +0x75C
    unsigned long AugmentTextDamageMultiplier;   // +0x760
    unsigned long AugmentTextFireStrength;   // +0x764
    unsigned long AugmentTextSlot;   // +0x768
    unsigned long AugmentTextAugmentations;   // +0x76C
    unsigned long AugmentTextWeapons;   // +0x770
    unsigned long AugmentTextDivider;   // +0x774
    C2DVector AugmentShadedCircleTLPos;   // +0x778
    C2DVector AugmentShadedCircleExtents;   // +0x780
    float InventoryAnimImageXRotationTime;   // +0x788
    float InventoryAnimImageYRotationTime;   // +0x78C
    float InventoryAnimImageZRotationTime;   // +0x790
    CRGBColour QuestCardFeatColour;   // +0x794
    CRGBColour QuestCardCoreColour;   // +0x798
    CRGBColour QuestCardExclusiveColour;   // +0x79C
    C2DVector QuestCardInfoTLPos;   // +0x7A0
    C2DVector QuestCardInfo2TLPos;   // +0x7A8
    C2DVector QuestCardNoQuestsTLPos;   // +0x7B0
    unsigned long QuestCardNoQuestsText;   // +0x7B8
    C2DVector QuestCardInfoTextOffset;   // +0x7BC
    C2DVector QuestCardEdgedButtonTitleTLPos;   // +0x7C4
    C2DVector QuestCardEdgedButtonExtents;   // +0x7CC
    VecLong ExpSpendItemLabelStat;   // +0x7D4
    VecLong ExpSpendItemLabelAbility;   // +0x7E0
    long ExpSpendMaxItemsToDisplay;   // +0x7EC
    C2DVector ExpSpendButtonTLPos;   // +0x7F0
    C2DVector ExpSpendButtonTLPosOffset;   // +0x7F8
    C2DVector ExpSpendButtonTextOffset;   // +0x800
    CEngineGraphic ExpSpendButtonSelL;   // +0x808
    CEngineGraphic ExpSpendButtonSelC;   // +0x816
    CEngineGraphic ExpSpendButtonSelR;   // +0x824
    CEngineGraphic ExpSpendButtonL;   // +0x832
    CEngineGraphic ExpSpendButtonC;   // +0x840
    CEngineGraphic ExpSpendButtonR;   // +0x84E
    C2DVector ExpSpendDescriptionBoxTLPos;   // +0x85C
    C2DVector ExpSpendDescriptionBoxExtents;   // +0x864
    C2DVector ExpSpendArrowUpTLPos;   // +0x86C
    C2DVector ExpSpendArrowDnTLPos;   // +0x874
    C2DVector ExpSpendGeneralPoolLabelTLPos;   // +0x87C
    C2DVector ExpSpendGeneralPoolLabelTextOffset;   // +0x884
    C2DVector ExpSpendGeneralPoolSpendTextOffset;   // +0x88C
    float ExpSpendGeneralPoolWidth;   // +0x894
    float ExpSpendGeneralPoolSliderWidth;   // +0x898
    C2DVector ExpSpendStrengthPoolTLPos;   // +0x89C
    C2DVector ExpSpendSkillPoolTLPos;   // +0x8A4
    C2DVector ExpSpendWillPoolTLPos;   // +0x8AC
    float ExpSpendPoolWidth;   // +0x8B4
    float ExpSpendPoolHeight;   // +0x8B8
    CEngineGraphic ExpSpendProgBarL;   // +0x8BC
    CEngineGraphic ExpSpendProgBarC;   // +0x8CA
    CEngineGraphic ExpSpendProgBarR;   // +0x8D8
    CEngineGraphic ExpSpendProgBarValue;   // +0x8E6
    C2DVector ExpSpendProgBarValueOffset;   // +0x8F4
    C2DVector ExpSpendTypeTextOffset;   // +0x8FC
    long ExpSpendGeneralPoolBarValue;   // +0x904
    long ExpSpendGeneralPoolLabel;   // +0x908
    long ExpSpendTypeStrengthLabel;   // +0x90C
    long ExpSpendTypeSkillLabel;   // +0x910
    long ExpSpendTypeWillLabel;   // +0x914
    float ExpSpendStrengthBarValue;   // +0x918
    float ExpSpendSkillBarValue;   // +0x91C
    float ExpSpendWillBarValue;   // +0x920
    long ExpSpendRate;   // +0x924
    long ExpSpendTimeOffset;   // +0x928
    CEngineGraphic ExpSpendGreen;   // +0x92C
    CEngineGraphic ExpSpendGreenOutline;   // +0x93A
    CEngineGraphic ExpSpendGreenEnd;   // +0x948
    CEngineGraphic ExpSpendRed;   // +0x956
    CEngineGraphic ExpSpendRedOutline;   // +0x964
    CEngineGraphic ExpSpendRedEnd;   // +0x972
    C2DVector ExpSpendDescriptionTextTLPos;   // +0x980
    C2DVector ExpSpendDescriptionTextOffset;   // +0x988
    C2DVector MainEdgedButtonTitleTLPos;   // +0x990
    C2DVector MainEdgedButtonOwnedTLPos;   // +0x998
    C2DVector MainEdgedButtonPriceTLPos;   // +0x9A0
    C2DVector MainEdgedButtonCostTLPos;   // +0x9A8
    C2DVector MainEdgedButtonExtents;   // +0x9B0
    CEngineGraphic MainDescriptionBoxC;   // +0x9B8
    CEngineGraphic MainDescriptionBoxTL;   // +0x9C6
    CEngineGraphic MainDescriptionBoxTR;   // +0x9D4
    CEngineGraphic MainDescriptionBoxBL;   // +0x9E2
    CEngineGraphic MainDescriptionBoxBR;   // +0x9F0
    C2DVector MainDescriptionBoxTLPos;   // +0x9FE
    C2DVector MainDescriptionBoxExtents;   // +0xA06
    C2DVector MainShadedCircleTLPos;   // +0xA0E
    C2DVector MainShadedCircleExtents;   // +0xA16
    C2DVector MainAnimBoxTLPos;   // +0xA1E
    C2DVector MainAnimBoxExtents;   // +0xA26
    float MainQuickAccessItemFlashTime;   // +0xA30
    void Transfer(CPersistContext& context);
};
static unsigned char g_buf[0x4000];
static int g_offs[400];
class CPersistContext {
public:
    const unsigned char* base; int mode; int n; size_t pos;
    template<class T> void Transfer(T& v)
    {
        if (n < 400) g_offs[n] = (int)((const unsigned char*)&v - base);
        ++n;
        if (mode == 0) std::memcpy(g_buf + pos, &v, sizeof(T)); else std::memcpy(&v, g_buf + pos, sizeof(T));
        pos += sizeof(T);
    }
};
void CInventoryDef::Transfer(CPersistContext& context)
{
    context.Transfer(InitialCategoryMovementRepeatDelay);     // 004678F1 +0x03c float -> 0x00431061
    context.Transfer(ContinuedCategoryMovementRepeatDelay);   // 004678FC +0x040 float -> 0x00431061
    context.Transfer(InitialItemMovementRepeatDelay);         // 00467907 +0x044 float -> 0x00431061
    context.Transfer(ContinuedItemMovementRepeatDelay);       // 00467912 +0x048 float -> 0x00431061
    context.Transfer(MovementSound);                          // 0046791D +0x04c long -> 0x00431102
    context.Transfer(NoMovementSound);                        // 00467928 +0x050 long -> 0x00431102
    context.Transfer(InventoryOpenSound);                     // 00467933 +0x054 long -> 0x00431102
    context.Transfer(TrackSelectedItems);                     // 0046793E +0x058 bool -> 0x0043314A
    context.Transfer(CategoriesTLPos);                        // 0046794C +0x09c C2DVector -> 0x004568BC
    context.Transfer(MeshAreaTLPos);                          // 0046795A +0x0a4 C2DVector -> 0x004568BC
    context.Transfer(MeshAreaBRPos);                          // 00467968 +0x0ac C2DVector -> 0x004568BC
    context.Transfer(NextCategoryOffset);                     // 00467976 +0x0b4 C2DVector -> 0x004568BC
    context.Transfer(FirstItemOffsetFromCategory);            // 00467984 +0x0bc C2DVector -> 0x004568BC
    context.Transfer(NextItemOffset);                         // 00467992 +0x0c4 C2DVector -> 0x004568BC
    context.Transfer(GenericFont);                            // 004679A0 +0x0cc CDefString -> 0x0045228F
    context.Transfer(BackGroundColour);                       // 004679AE +0x0d0 CRGBColour -> 0x004595A3
    context.Transfer(ClothingDoll);                           // 004679BC +0x0d4 long -> 0x00431102
    context.Transfer(DollCentreOffset);                       // 004679CA +0x0d8 C3DVector -> 0x00456903
    context.Transfer(DollBoundingSphereRadius);               // 004679D8 +0x0e4 float -> 0x00431061
    context.Transfer(TotalExperienceTextPos);                 // 004679E6 +0x0e8 C2DVector -> 0x004568BC
    context.Transfer(TotalExperienceText);                    // 004679F4 +0x0f0 ulong -> 0x00431020
    context.Transfer(SpendingExperienceTextPos);              // 00467A02 +0x0f4 C2DVector -> 0x004568BC
    context.Transfer(SpendingExperienceText);                 // 00467A10 +0x0fc ulong -> 0x00431020
    context.Transfer(StrengthLevelTextPos);                   // 00467A1E +0x100 C2DVector -> 0x004568BC
    context.Transfer(StrengthLevelColour);                    // 00467A2C +0x108 CRGBColour -> 0x004595A3
    context.Transfer(SkillLevelTextPos);                      // 00467A3A +0x10c C2DVector -> 0x004568BC
    context.Transfer(SkillLevelColour);                       // 00467A48 +0x114 CRGBColour -> 0x004595A3
    context.Transfer(WillLevelTextPos);                       // 00467A56 +0x118 C2DVector -> 0x004568BC
    context.Transfer(WillLevelColour);                        // 00467A64 +0x120 CRGBColour -> 0x004595A3
    context.Transfer(RenownLevelTextPos);                     // 00467A72 +0x124 C2DVector -> 0x004568BC
    context.Transfer(HeroLevelTextPos);                       // 00467A80 +0x12c C2DVector -> 0x004568BC
    context.Transfer(StatLevelsFirstLineTextPos);             // 00467A8E +0x134 C2DVector -> 0x004568BC
    context.Transfer(StatLevelsNextLineTextOffset);           // 00467A9C +0x13c C2DVector -> 0x004568BC
    context.Transfer(ButtonAssignmentTextPos);                // 00467AAA +0x144 C2DVector -> 0x004568BC
    context.Transfer(ButtonAssignmentTextOffset);             // 00467AB8 +0x14c C2DVector -> 0x004568BC
    context.Transfer(StatLevelsTexts);                        // 00467AC6 +0x154 vector<unsigned_long,alloc<unsigned_long -> 0x00464841
    context.Transfer(StatLevelsColours);                      // 00467AD4 +0x160 vector<CRGBColour,alloc<CRGBColour>_> -> 0x00468EB4
    context.Transfer(WeaponsTextNumberOfSlots);               // 00467AE2 +0x170 long -> 0x00431102
    context.Transfer(WeaponsTextSeparator);                   // 00467AF0 +0x16c long -> 0x00431102
    context.Transfer(WeaponsTextSlot);                        // 00467AFE +0x174 long -> 0x00431102
    context.Transfer(WeaponsTextSuccessfulAugmentation);      // 00467B0C +0x178 long -> 0x00431102
    context.Transfer(WeaponsTextNoFreeSlots);                 // 00467B1A +0x17c long -> 0x00431102
    context.Transfer(WeaponsTextReadyToAugment);              // 00467B28 +0x180 long -> 0x00431102
    context.Transfer(WeaponsTextNotReadyToAugment);           // 00467B36 +0x184 long -> 0x00431102
    context.Transfer(WeaponsDescriptionBoxTLPos);             // 00467B44 +0x188 C2DVector -> 0x004568BC
    context.Transfer(WeaponsDescriptionBoxExtents);           // 00467B52 +0x190 C2DVector -> 0x004568BC
    context.Transfer(WeaponsActiveMeleeIconPos);              // 00467B60 +0x198 C2DVector -> 0x004568BC
    context.Transfer(WeaponsActiveRangeIconPos);              // 00467B6E +0x1a0 C2DVector -> 0x004568BC
    context.Transfer(WeaponAugmentationTextStartPos);         // 00467B7C +0x1a8 C2DVector -> 0x004568BC
    context.Transfer(WeaponAugmentationTextNewLineOffset);    // 00467B8A +0x1b0 C2DVector -> 0x004568BC
    context.Transfer(AugmentationStatusTextAreaTLPos);        // 00467B98 +0x1b8 C2DVector -> 0x004568BC
    context.Transfer(AugmentationStatusTextAreaBRPos);        // 00467BA6 +0x1c0 C2DVector -> 0x004568BC
    context.Transfer(TimeToDisplayWarningMessageInSeconds);   // 00467BB4 +0x1c8 float -> 0x00431061
    context.Transfer(NumberOfSecondsForObjectRotation);       // 00467BC2 +0x1dc float -> 0x00431061
    context.Transfer(MaxNumberOfItemsToDisplayPerCategory);   // 00467BD0 +0x1e0 long -> 0x00431102
    context.Transfer(MaxNumberOfCategoriesToDisplay);         // 00467BDE +0x1e4 long -> 0x00431102
    context.Transfer(MaxNumberOfScreenNamesToDisplay);        // 00467BEC +0x1e8 long -> 0x00431102
    context.Transfer(WeaponsShadedCircleTLPos);               // 00467BFA +0x1cc C2DVector -> 0x004568BC
    context.Transfer(WeaponsShadedCircleExtents);             // 00467C08 +0x1d4 C2DVector -> 0x004568BC
    context.Transfer(InventoryName);                          // 00467C16 +0x1ec ulong -> 0x00431020
    context.Transfer(InventoryIdentifier);                    // 00467C24 +0x1f0 EInventoryIdentifier -> 0x00468E10
    context.Transfer(MapScreenMapDisplayBoxTLPos);            // 00467C32 +0x1f4 C2DVector -> 0x004568BC
    context.Transfer(MapScreenMapDisplayBoxBRPos);            // 00467C40 +0x1fc C2DVector -> 0x004568BC
    context.Transfer(MapScreenRegionNamePos);                 // 00467C4E +0x204 C2DVector -> 0x004568BC
    context.Transfer(MapScreenWorldMapGraphic);               // 00467C5C +0x20c long -> 0x00431102
    context.Transfer(MapScreenWorldMapPos);                   // 00467C6A +0x210 C2DVector -> 0x004568BC
    context.Transfer(MapScreenRegionUnknownGraphic);          // 00467C78 +0x218 long -> 0x00431102
    context.Transfer(MapScreenRegionUnknownHighlightGraphic); // 00467C86 +0x21c long -> 0x00431102
    context.Transfer(MapScreenRegionHeroGraphic);             // 00467C94 +0x220 long -> 0x00431102
    context.Transfer(MapScreenRegionQuestGraphic);            // 00467CA2 +0x224 long -> 0x00431102
    context.Transfer(MapScreenRegionTeleportGraphic);         // 00467CB0 +0x228 long -> 0x00431102
    context.Transfer(MapScreenRegionHouseGraphic);            // 00467CBE +0x22c long -> 0x00431102
    context.Transfer(StatsTitleTLPos);                        // 00467CCC +0x230 C2DVector -> 0x004568BC
    context.Transfer(StatsShadedCircleTLPos);                 // 00467CDA +0x238 C2DVector -> 0x004568BC
    context.Transfer(StatsShadedCircleExtents);               // 00467CE8 +0x240 C2DVector -> 0x004568BC
    context.Transfer(StatsProgBarL);                          // 00467CF6 +0x248 CEngineGraphic -> 0x00451E11
    context.Transfer(StatsProgBarC);                          // 00467D04 +0x256 CEngineGraphic -> 0x00451E11
    context.Transfer(StatsProgBarR);                          // 00467D12 +0x264 CEngineGraphic -> 0x00451E11
    context.Transfer(StatsProgBarValue);                      // 00467D20 +0x272 CEngineGraphic -> 0x00451E11
    context.Transfer(StatsProgBarValueOffset);                // 00467D2E +0x280 C2DVector -> 0x004568BC
    context.Transfer(StatsPersonalityHeadingTLPos);           // 00467D3C +0x288 C2DVector -> 0x004568BC
    context.Transfer(StatsPersonalityHeadingBRPos);           // 00467D4A +0x290 C2DVector -> 0x004568BC
    context.Transfer(StatsPersonalityRenownLowerLabel);       // 00467D58 +0x298 ulong -> 0x00431020
    context.Transfer(StatsPersonalityRenownUpperLabel);       // 00467D66 +0x29c ulong -> 0x00431020
    context.Transfer(StatsPersonalityGoodnessLowerLabel);     // 00467D74 +0x2a0 ulong -> 0x00431020
    context.Transfer(StatsPersonalityGoodnessUpperLabel);     // 00467D82 +0x2a4 ulong -> 0x00431020
    context.Transfer(StatsPersonalityAttractivenessLowerLabel);// 00467D90 +0x2a8 ulong -> 0x00431020
    context.Transfer(StatsPersonalityAttractivenessUpperLabel);// 00467D9E +0x2ac ulong -> 0x00431020
    context.Transfer(StatsPersonalityScarinessLowerLabel);    // 00467DAC +0x2b0 ulong -> 0x00431020
    context.Transfer(StatsPersonalityScarinessUpperLabel);    // 00467DBA +0x2b4 ulong -> 0x00431020
    context.Transfer(StatsPersonalityNicenessLowerLabel);     // 00467DC8 +0x2b8 ulong -> 0x00431020
    context.Transfer(StatsPersonalityNicenessUpperLabel);     // 00467DD6 +0x2bc ulong -> 0x00431020
    context.Transfer(StatsPersonalityLabelTextOffset);        // 00467DE4 +0x2c0 C2DVector -> 0x004568BC
    context.Transfer(StatsPersonalityLowerLabelTLPos);        // 00467DF2 +0x2c8 C2DVector -> 0x004568BC
    context.Transfer(StatsPersonalityLowerLabelWidth);        // 00467E00 +0x2d0 float -> 0x00431061
    context.Transfer(StatsPersonalityLowerLabelHeight);       // 00467E0E +0x2d4 float -> 0x00431061
    context.Transfer(StatsPersonalitySliderWidth);            // 00467E1C +0x2d8 float -> 0x00431061
    context.Transfer(StatsPersonalityUpperLabelWidth);        // 00467E2A +0x2dc float -> 0x00431061
    context.Transfer(StatsPersonalityDoll);                   // 00467E38 +0x2e0 long -> 0x00431102
    context.Transfer(StatsPersonalityDollCentreOffset);       // 00467E46 +0x2e4 C3DVector -> 0x00456903
    context.Transfer(StatsPersonalityDollBoundingSphereRadius);// 00467E54 +0x2f0 float -> 0x00431061
    context.Transfer(StatsStatisticsLabelTLPos);              // 00467E62 +0x2f4 C2DVector -> 0x004568BC
    context.Transfer(StatsStatisticsLabelWidth);              // 00467E70 +0x2fc float -> 0x00431061
    context.Transfer(StatsStatisticsLabelHeight);             // 00467E7E +0x300 float -> 0x00431061
    context.Transfer(StatsStatisticsLabelTextOffset);         // 00467E8C +0x304 C2DVector -> 0x004568BC
    context.Transfer(StatsStatisticsDataTextOffset);          // 00467E9A +0x30c C2DVector -> 0x004568BC
    context.Transfer(StatsStatisticsHeadingTLPos);            // 00467EA8 +0x314 C2DVector -> 0x004568BC
    context.Transfer(StatsStatisticsHeadingBRPos);            // 00467EB6 +0x31c C2DVector -> 0x004568BC
    context.Transfer(StatsStatisticsPeopleKilledLabel);       // 00467EC4 +0x324 ulong -> 0x00431020
    context.Transfer(StatsStatisticsMonstersKilledLabel);     // 00467ED2 +0x328 ulong -> 0x00431020
    context.Transfer(StatsStatisticsMaxMultiplierLabel);      // 00467EE0 +0x32c ulong -> 0x00431020
    context.Transfer(StatsStatisticsPreferredFoodLabel);      // 00467EEE +0x330 ulong -> 0x00431020
    context.Transfer(StatsStatisticsDietaryTypeLabel);        // 00467EFC +0x334 ulong -> 0x00431020
    context.Transfer(StatsStatisticsGameTimeElapsedLabel);    // 00467F0A +0x338 ulong -> 0x00431020
    context.Transfer(StatsStatisticsNumberOfWivesLabel);      // 00467F18 +0x33c ulong -> 0x00431020
    context.Transfer(StatsStatisticsQuestsActiveLabel);       // 00467F26 +0x340 ulong -> 0x00431020
    context.Transfer(StatsStatisticsQuestsFinishedLabel);     // 00467F34 +0x344 ulong -> 0x00431020
    context.Transfer(StatsStatisticsBoastsAchievedLabel);     // 00467F42 +0x348 ulong -> 0x00431020
    context.Transfer(InitialInventoryStatsItems);             // 00467F4D +0x074 vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(InitialInventoryStandardItems);          // 00467F5B +0x080 vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(InitialInventoryWeaponsItems);           // 00467F66 +0x068 vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(InitialInventoryClothingItems);          // 00467F71 +0x05c vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(ClothingShadedCircleTLPos);              // 00467F7F +0x08c C2DVector -> 0x004568BC
    context.Transfer(ClothingShadedCircleExtents);            // 00467F8D +0x094 C2DVector -> 0x004568BC
    context.Transfer(StatsExperienceStrengthTextOffset);      // 00467F9B +0x34c C2DVector -> 0x004568BC
    context.Transfer(StatsExperienceWillTextOffset);          // 00467FA9 +0x354 C2DVector -> 0x004568BC
    context.Transfer(StatsExperienceSkillTextOffset);         // 00467FB7 +0x35c C2DVector -> 0x004568BC
    context.Transfer(StatsExperienceStrengthText);            // 00467FC5 +0x364 ulong -> 0x00431020
    context.Transfer(StatsExperienceWillText);                // 00467FD3 +0x368 ulong -> 0x00431020
    context.Transfer(StatsExperienceSkillText);               // 00467FE1 +0x36c ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolGeneralTLPos);        // 00467FEF +0x370 C2DVector -> 0x004568BC
    context.Transfer(StatsExperiencePoolGeneralTextOffset);   // 00467FFD +0x378 C2DVector -> 0x004568BC
    context.Transfer(StatsExperiencePoolStrengthGroupTLPos);  // 0046800B +0x380 C2DVector -> 0x004568BC
    context.Transfer(StatsExperiencePoolSkillGroupTLPos);     // 00468019 +0x388 C2DVector -> 0x004568BC
    context.Transfer(StatsExperiencePoolWillGroupTLPos);      // 00468027 +0x390 C2DVector -> 0x004568BC
    context.Transfer(StatsExperiencePoolLabelTLPos);          // 00468035 +0x398 C2DVector -> 0x004568BC
    context.Transfer(StatsExperiencePoolLabelTextOffset);     // 00468043 +0x3a0 C2DVector -> 0x004568BC
    context.Transfer(StatsExperiencePoolLabelWidth);          // 00468051 +0x3a8 float -> 0x00431061
    context.Transfer(StatsExperiencePoolLabelHeight);         // 0046805F +0x3ac float -> 0x00431061
    context.Transfer(StatsExperiencePoolSliderWidth);         // 0046806D +0x3b0 float -> 0x00431061
    context.Transfer(StatsExperiencePoolStrengthLabel);       // 0046807B +0x3b4 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolSkillLabel);          // 00468089 +0x3b8 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolWillLabel);           // 00468097 +0x3bc ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolPhysiqueLabel);       // 004680A5 +0x3c0 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolHealthLabel);         // 004680B3 +0x3c4 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolToughnessLabel);      // 004680C1 +0x3c8 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolSpeedLabel);          // 004680CF +0x3cc ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolAccuracyLabel);       // 004680DD +0x3d0 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolStealthLabel);        // 004680EB +0x3d4 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolWeaponMagicLabel);    // 004680F9 +0x3d8 ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolAbilityMagicLabel);   // 00468107 +0x3dc ulong -> 0x00431020
    context.Transfer(StatsExperiencePoolPureMagicLabel);      // 00468115 +0x3e0 ulong -> 0x00431020
    context.Transfer(StatsExperienceStrengthBarValue);        // 00468123 +0x3e4 float -> 0x00431061
    context.Transfer(StatsExperienceSkillBarValue);           // 00468131 +0x3e8 float -> 0x00431061
    context.Transfer(StatsExperienceWillBarValue);            // 0046813F +0x3ec float -> 0x00431061
    context.Transfer(ExperienceGeneralPoolLabelTLPos);        // 0046814D +0x3f0 C2DVector -> 0x004568BC
    context.Transfer(ExperienceGeneralPoolLabelTextOffset);   // 0046815B +0x3f8 C2DVector -> 0x004568BC
    context.Transfer(ExperienceGeneralPoolWidth);             // 00468169 +0x400 float -> 0x00431061
    context.Transfer(ExperienceGeneralPoolSliderWidth);       // 00468177 +0x404 float -> 0x00431061
    context.Transfer(ExperienceGeneralPoolLabel);             // 00468185 +0x40c ulong -> 0x00431020
    context.Transfer(ExperienceGeneralPoolBarValue);          // 00468193 +0x408 ulong -> 0x00431020
    context.Transfer(ExperienceGeneralPoolSpendTextOffset);   // 004681A1 +0x410 C2DVector -> 0x004568BC
    context.Transfer(ExperienceStrengthPoolTLPos);            // 004681AF +0x418 C2DVector -> 0x004568BC
    context.Transfer(ExperienceSkillPoolTLPos);               // 004681BD +0x420 C2DVector -> 0x004568BC
    context.Transfer(ExperienceWillPoolTLPos);                // 004681CB +0x428 C2DVector -> 0x004568BC
    context.Transfer(ExperiencePoolWidth);                    // 004681D9 +0x430 float -> 0x00431061
    context.Transfer(ExperiencePoolHeight);                   // 004681E7 +0x434 float -> 0x00431061
    context.Transfer(ExperienceTypeTLPos);                    // 004681F5 +0x450 C2DVector -> 0x004568BC
    context.Transfer(ExperienceTypeTLPosOffset);              // 00468203 +0x458 C2DVector -> 0x004568BC
    context.Transfer(ExperienceTypeTextOffset);               // 00468211 +0x460 C2DVector -> 0x004568BC
    context.Transfer(ExperienceTypeSpendTextOffset);          // 0046821F +0x468 C2DVector -> 0x004568BC
    context.Transfer(ExperienceTypeLabelWidth);               // 0046822D +0x470 float -> 0x00431061
    context.Transfer(ExperienceTypeLabelHeight);              // 0046823B +0x474 float -> 0x00431061
    context.Transfer(ExperienceTypeSliderWidth);              // 00468249 +0x478 float -> 0x00431061
    context.Transfer(ExperienceTypeStrengthLabel);            // 00468257 +0x438 ulong -> 0x00431020
    context.Transfer(ExperienceTypeSkillLabel);               // 00468265 +0x43c ulong -> 0x00431020
    context.Transfer(ExperienceTypeWillLabel);                // 00468273 +0x440 ulong -> 0x00431020
    context.Transfer(ExperienceTypePureMagicLabel);           // 00468281 +0x444 ulong -> 0x00431020
    context.Transfer(ExperienceTypeAbilityMagicLabel);        // 0046828F +0x448 ulong -> 0x00431020
    context.Transfer(ExperienceTypeWeaponMagicLabel);         // 0046829D +0x44c ulong -> 0x00431020
    context.Transfer(ExperienceStrengthPhysiqueLabel);        // 004682AB +0x47c ulong -> 0x00431020
    context.Transfer(ExperienceStrengthHealthLabel);          // 004682B9 +0x480 ulong -> 0x00431020
    context.Transfer(ExperienceStrengthToughnessLabel);       // 004682C7 +0x484 ulong -> 0x00431020
    context.Transfer(ExperienceSkillSpeedLabel);              // 004682D5 +0x488 ulong -> 0x00431020
    context.Transfer(ExperienceSkillAccuracyLabel);           // 004682E3 +0x48c ulong -> 0x00431020
    context.Transfer(ExperienceSkillStealthLabel);            // 004682F1 +0x490 ulong -> 0x00431020
    context.Transfer(ExperienceWeaponMagicBerserkLabel);      // 004682FF +0x494 ulong -> 0x00431020
    context.Transfer(ExperienceWeaponMagicFlamingBladeLabel); // 0046830D +0x498 ulong -> 0x00431020
    context.Transfer(ExperienceWeaponMagicDoubleStrikeLabel); // 0046831B +0x49c ulong -> 0x00431020
    context.Transfer(ExperienceWeaponMagicLightningBladeLabel);// 00468329 +0x4a0 ulong -> 0x00431020
    context.Transfer(ExperienceAbilityMagicBattleChargeLabel);// 00468337 +0x4a4 ulong -> 0x00431020
    context.Transfer(ExperienceAbilityMagicAssassinRushLabel);// 00468345 +0x4a8 ulong -> 0x00431020
    context.Transfer(ExperienceAbilityMagicTimeSpellLabel);   // 00468353 +0x4ac ulong -> 0x00431020
    context.Transfer(ExperiencePureMagicBurningHandsLabel);   // 00468361 +0x4b0 ulong -> 0x00431020
    context.Transfer(ExperiencePureMagicPhysicalShieldLabel); // 0046836F +0x4b4 ulong -> 0x00431020
    context.Transfer(ExperiencePureMagicForcePushLabel);      // 0046837D +0x4b8 ulong -> 0x00431020
    context.Transfer(ExperiencePureMagicEnflameSpellLabel);   // 0046838B +0x4bc ulong -> 0x00431020
    context.Transfer(ExperiencePureMagicDrainLifeLabel);      // 00468399 +0x4c0 ulong -> 0x00431020
    context.Transfer(ExperiencePureMagicHealLifeLabel);       // 004683A7 +0x4c4 ulong -> 0x00431020
    context.Transfer(ExperiencePureMagicGhostSwordLabel);     // 004683B5 +0x4c8 ulong -> 0x00431020
    context.Transfer(ExperienceStrengthBarValue);             // 004683C3 +0x4cc float -> 0x00431061
    context.Transfer(ExperienceSkillBarValue);                // 004683D1 +0x4d0 float -> 0x00431061
    context.Transfer(ExperienceWillBarValue);                 // 004683DF +0x4d4 float -> 0x00431061
    context.Transfer(ExperienceStrengthPhysiqueBarValue);     // 004683ED +0x4d8 float -> 0x00431061
    context.Transfer(ExperienceStrengthHealthBarValue);       // 004683FB +0x4dc float -> 0x00431061
    context.Transfer(ExperienceStrengthToughnessBarValue);    // 00468409 +0x4e0 float -> 0x00431061
    context.Transfer(ExperienceSkillSpeedBarValue);           // 00468417 +0x4e4 float -> 0x00431061
    context.Transfer(ExperienceSkillAccuracyBarValue);        // 00468425 +0x4e8 float -> 0x00431061
    context.Transfer(ExperienceSkillHealthBarValue);          // 00468433 +0x4ec float -> 0x00431061
    context.Transfer(ExperienceWeaponMagicBerserkBarValue);   // 00468441 +0x4f0 float -> 0x00431061
    context.Transfer(ExperienceWeaponMagicFlamingBladeBarValue);// 0046844F +0x4f4 float -> 0x00431061
    context.Transfer(ExperienceWeaponMagicDoubleStrikeBarValue);// 0046845D +0x4f8 float -> 0x00431061
    context.Transfer(ExperienceAbilityMagicBattleChargeBarValue);// 0046846B +0x4fc float -> 0x00431061
    context.Transfer(ExperienceAbilityMagicAssassinRushBarValue);// 00468479 +0x500 float -> 0x00431061
    context.Transfer(ExperienceAbilityMagicTimeSpellBarValue);// 00468487 +0x504 float -> 0x00431061
    context.Transfer(ExperiencePureMagicBurningHandsBarValue);// 00468495 +0x508 float -> 0x00431061
    context.Transfer(ExperiencePureMagicPhysicalShieldBarValue);// 004684A3 +0x50c float -> 0x00431061
    context.Transfer(ExperiencePureMagicForcePushBarValue);   // 004684B1 +0x510 float -> 0x00431061
    context.Transfer(ExperienceProgBarL);                     // 004684BF +0x514 CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceProgBarC);                     // 004684CD +0x522 CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceProgBarR);                     // 004684DB +0x530 CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceProgBarValue);                 // 004684E9 +0x53e CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceProgBarValueOffset);           // 004684F7 +0x54c C2DVector -> 0x004568BC
    context.Transfer(ExperienceSpendTimeOffset);              // 00468505 +0x554 long -> 0x00431102
    context.Transfer(ExperienceSpendRate);                    // 00468513 +0x558 long -> 0x00431102
    context.Transfer(ExperienceItemLabelStat);                // 00468521 +0x55c vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(ExperienceItemLabelAbility);             // 0046852F +0x568 vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(ExperienceSpendGreen);                   // 0046853D +0x574 CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceSpendGreenOutline);            // 0046854B +0x582 CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceSpendGreenEnd);                // 00468559 +0x590 CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceSpendRed);                     // 00468567 +0x59e CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceSpendRedOutline);              // 00468575 +0x5ac CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceSpendRedEnd);                  // 00468583 +0x5ba CEngineGraphic -> 0x00451E11
    context.Transfer(ExperienceDescriptionTextTLPos);         // 00468591 +0x5c8 C2DVector -> 0x004568BC
    context.Transfer(ExperienceDescriptionTextOffset);        // 0046859F +0x5d0 C2DVector -> 0x004568BC
    context.Transfer(QuestCardFeatColour);                    // 004685AD +0x794 CRGBColour -> 0x004595A3
    context.Transfer(QuestCardCoreColour);                    // 004685BB +0x798 CRGBColour -> 0x004595A3
    context.Transfer(QuestCardExclusiveColour);               // 004685C9 +0x79c CRGBColour -> 0x004595A3
    context.Transfer(QuestCardInfoTLPos);                     // 004685D7 +0x7a0 C2DVector -> 0x004568BC
    context.Transfer(QuestCardInfo2TLPos);                    // 004685E5 +0x7a8 C2DVector -> 0x004568BC
    context.Transfer(QuestCardNoQuestsTLPos);                 // 004685F3 +0x7b0 C2DVector -> 0x004568BC
    context.Transfer(QuestCardNoQuestsText);                  // 00468601 +0x7b8 ulong -> 0x00431020
    context.Transfer(QuestCardEdgedButtonTitleTLPos);         // 0046860F +0x7c4 C2DVector -> 0x004568BC
    context.Transfer(QuestCardEdgedButtonExtents);            // 0046861D +0x7cc C2DVector -> 0x004568BC
    context.Transfer(QuestCardInfoTextOffset);                // 0046862B +0x7bc C2DVector -> 0x004568BC
    context.Transfer(QuestCardInfoQuestNameLabel);            // 00468639 +0x5d8 ulong -> 0x00431020
    context.Transfer(QuestCardInfoMoralityLabel);             // 00468647 +0x5dc ulong -> 0x00431020
    context.Transfer(QuestCardInfoCoreLabel);                 // 00468655 +0x5e0 ulong -> 0x00431020
    context.Transfer(QuestCardInfoDetailsLabel);              // 00468663 +0x5e4 ulong -> 0x00431020
    context.Transfer(QuestCardInfoRegionLabel);               // 00468671 +0x5e8 ulong -> 0x00431020
    context.Transfer(QuestCardInfoOriginatorLabel);           // 0046867F +0x5ec ulong -> 0x00431020
    context.Transfer(QuestCardInfoDescriptionLabel);          // 0046868D +0x5f0 ulong -> 0x00431020
    context.Transfer(QuestCardInfoSuccessLabel);              // 0046869B +0x5f4 ulong -> 0x00431020
    context.Transfer(QuestCardInfoFailureLabel);              // 004686A9 +0x5f8 ulong -> 0x00431020
    context.Transfer(QuestCardInfoRewardsLabel);              // 004686B7 +0x5fc ulong -> 0x00431020
    context.Transfer(QuestCardInfoRenownLabel);               // 004686C5 +0x600 ulong -> 0x00431020
    context.Transfer(QuestCardInfoTrophyLabel);               // 004686D3 +0x604 ulong -> 0x00431020
    context.Transfer(QuestCardInfoBoastsLabel);               // 004686E1 +0x608 ulong -> 0x00431020
    context.Transfer(QuestCardInfoStatusLabel);               // 004686EF +0x60c ulong -> 0x00431020
    context.Transfer(QuestCardInfoQuestNameText);             // 004686FD +0x610 ulong -> 0x00431020
    context.Transfer(QuestCardInfoMoralityText);              // 0046870B +0x614 ulong -> 0x00431020
    context.Transfer(QuestCardInfoCoreText);                  // 00468719 +0x618 ulong -> 0x00431020
    context.Transfer(QuestCardInfoDetailsText);               // 00468727 +0x61c ulong -> 0x00431020
    context.Transfer(QuestCardInfoRegionText);                // 00468735 +0x620 ulong -> 0x00431020
    context.Transfer(QuestCardInfoOriginatorText);            // 00468743 +0x624 ulong -> 0x00431020
    context.Transfer(QuestCardInfoDescriptionText);           // 00468751 +0x628 ulong -> 0x00431020
    context.Transfer(QuestCardInfoSuccessText);               // 0046875F +0x62c ulong -> 0x00431020
    context.Transfer(QuestCardInfoFailureText);               // 0046876D +0x630 ulong -> 0x00431020
    context.Transfer(QuestCardInfoRewardsText);               // 0046877B +0x634 ulong -> 0x00431020
    context.Transfer(QuestCardInfoRenownText);                // 00468789 +0x638 ulong -> 0x00431020
    context.Transfer(QuestCardInfoTrophyText);                // 00468797 +0x63c ulong -> 0x00431020
    context.Transfer(QuestCardInfoBoastsText);                // 004687A5 +0x640 ulong -> 0x00431020
    context.Transfer(QuestCardInfoStatusText);                // 004687B3 +0x644 ulong -> 0x00431020
    context.Transfer(TradeEdgedButtonCostTLPos);              // 004687C1 +0x648 C2DVector -> 0x004568BC
    context.Transfer(TradeEdgedButtonOwnedTLPos);             // 004687CF +0x650 C2DVector -> 0x004568BC
    context.Transfer(TradeEdgedButtonProfitTLPos);            // 004687DD +0x658 C2DVector -> 0x004568BC
    context.Transfer(TradeEdgedButtonCashTLPos);              // 004687EB +0x668 C2DVector -> 0x004568BC
    context.Transfer(TradeEdgedButtonTitleTLPos);             // 004687F9 +0x660 C2DVector -> 0x004568BC
    context.Transfer(TradeEdgedButtonExtents);                // 00468807 +0x670 C2DVector -> 0x004568BC
    context.Transfer(TradeAnimImageBoxTLPos);                 // 00468815 +0x678 C2DVector -> 0x004568BC
    context.Transfer(TradeAnimImageBoxExtents);               // 00468823 +0x680 C2DVector -> 0x004568BC
    context.Transfer(TradeAnimImageXRotationTime);            // 00468831 +0x688 float -> 0x00431061
    context.Transfer(TradeAnimImageYRotationTime);            // 0046883F +0x68c float -> 0x00431061
    context.Transfer(TradeAnimImageZRotationTime);            // 0046884D +0x690 float -> 0x00431061
    context.Transfer(TradeEquippedItemTLPos);                 // 0046885B +0x694 C2DVector -> 0x004568BC
    context.Transfer(TradeEquippedItemExtents);               // 00468869 +0x69c C2DVector -> 0x004568BC
    context.Transfer(TradeEquippedItemGraphic);               // 00468877 +0x6a4 CEngineGraphic -> 0x00451E11
    context.Transfer(TradeDescriptionBoxTLPos);               // 00468885 +0x6b2 C2DVector -> 0x004568BC
    context.Transfer(TradeDescriptionBoxExtents);             // 00468893 +0x6ba C2DVector -> 0x004568BC
    context.Transfer(TradeTextOwned);                         // 004688A1 +0x6c4 ulong -> 0x00431020
    context.Transfer(TradeTextDivider);                       // 004688AF +0x6c8 ulong -> 0x00431020
    context.Transfer(TradeTextCost);                          // 004688BD +0x6cc ulong -> 0x00431020
    context.Transfer(TradeTextGold);                          // 004688CB +0x6d0 ulong -> 0x00431020
    context.Transfer(TradeTextProfit);                        // 004688D9 +0x6d4 ulong -> 0x00431020
    context.Transfer(TradeShadedCircleTLPos);                 // 004688E7 +0x6d8 C2DVector -> 0x004568BC
    context.Transfer(TradeShadedCircleTLPos);                 // 004688EF +0x6d8 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendItemLabelStat);                  // 004688FD +0x7d4 vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(ExpSpendItemLabelAbility);               // 0046890B +0x7e0 vector<long,alloc<long>_> -> 0x00466A47
    context.Transfer(ExpSpendMaxItemsToDisplay);              // 00468919 +0x7ec long -> 0x00431102
    context.Transfer(ExpSpendButtonTLPos);                    // 00468927 +0x7f0 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendButtonTLPosOffset);              // 00468935 +0x7f8 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendButtonTextOffset);               // 00468943 +0x800 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendButtonSelL);                     // 00468951 +0x808 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendButtonSelC);                     // 0046895F +0x816 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendButtonSelR);                     // 0046896D +0x824 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendButtonL);                        // 0046897B +0x832 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendButtonC);                        // 00468989 +0x840 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendButtonR);                        // 00468997 +0x84e CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendDescriptionBoxTLPos);            // 004689A5 +0x85c C2DVector -> 0x004568BC
    context.Transfer(ExpSpendDescriptionBoxExtents);          // 004689B3 +0x864 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendArrowUpTLPos);                   // 004689C1 +0x86c C2DVector -> 0x004568BC
    context.Transfer(ExpSpendArrowDnTLPos);                   // 004689CF +0x874 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendGeneralPoolLabelTLPos);          // 004689DD +0x87c C2DVector -> 0x004568BC
    context.Transfer(ExpSpendGeneralPoolLabelTextOffset);     // 004689EB +0x884 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendGeneralPoolSpendTextOffset);     // 004689F9 +0x88c C2DVector -> 0x004568BC
    context.Transfer(ExpSpendGeneralPoolWidth);               // 00468A07 +0x894 float -> 0x00431061
    context.Transfer(ExpSpendGeneralPoolSliderWidth);         // 00468A15 +0x898 float -> 0x00431061
    context.Transfer(ExpSpendStrengthPoolTLPos);              // 00468A23 +0x89c C2DVector -> 0x004568BC
    context.Transfer(ExpSpendSkillPoolTLPos);                 // 00468A31 +0x8a4 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendWillPoolTLPos);                  // 00468A3F +0x8ac C2DVector -> 0x004568BC
    context.Transfer(ExpSpendPoolWidth);                      // 00468A4D +0x8b4 float -> 0x00431061
    context.Transfer(ExpSpendPoolHeight);                     // 00468A5B +0x8b8 float -> 0x00431061
    context.Transfer(ExpSpendProgBarL);                       // 00468A69 +0x8bc CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendProgBarC);                       // 00468A77 +0x8ca CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendProgBarR);                       // 00468A85 +0x8d8 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendProgBarValue);                   // 00468A93 +0x8e6 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendProgBarValueOffset);             // 00468AA1 +0x8f4 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendTypeTextOffset);                 // 00468AAF +0x8fc C2DVector -> 0x004568BC
    context.Transfer(ExpSpendGeneralPoolBarValue);            // 00468ABD +0x904 long -> 0x00431102
    context.Transfer(ExpSpendGeneralPoolLabel);               // 00468ACB +0x908 long -> 0x00431102
    context.Transfer(ExpSpendTypeStrengthLabel);              // 00468AD9 +0x90c long -> 0x00431102
    context.Transfer(ExpSpendTypeSkillLabel);                 // 00468AE7 +0x910 long -> 0x00431102
    context.Transfer(ExpSpendTypeWillLabel);                  // 00468AF5 +0x914 long -> 0x00431102
    context.Transfer(ExpSpendStrengthBarValue);               // 00468B03 +0x918 float -> 0x00431061
    context.Transfer(ExpSpendSkillBarValue);                  // 00468B11 +0x91c float -> 0x00431061
    context.Transfer(ExpSpendWillBarValue);                   // 00468B1F +0x920 float -> 0x00431061
    context.Transfer(ExpSpendRate);                           // 00468B2D +0x924 long -> 0x00431102
    context.Transfer(ExpSpendTimeOffset);                     // 00468B3B +0x928 long -> 0x00431102
    context.Transfer(ExpSpendGreen);                          // 00468B49 +0x92c CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendGreenOutline);                   // 00468B57 +0x93a CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendGreenEnd);                       // 00468B65 +0x948 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendRed);                            // 00468B73 +0x956 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendRedOutline);                     // 00468B81 +0x964 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendRedEnd);                         // 00468B8F +0x972 CEngineGraphic -> 0x00451E11
    context.Transfer(ExpSpendDescriptionTextTLPos);           // 00468B9D +0x980 C2DVector -> 0x004568BC
    context.Transfer(ExpSpendDescriptionTextOffset);          // 00468BAB +0x988 C2DVector -> 0x004568BC
    context.Transfer(InventoryAnimImageXRotationTime);        // 00468BB9 +0x788 float -> 0x00431061
    context.Transfer(InventoryAnimImageYRotationTime);        // 00468BC7 +0x78c float -> 0x00431061
    context.Transfer(InventoryAnimImageZRotationTime);        // 00468BD5 +0x790 float -> 0x00431061
    context.Transfer(AugmentationMeshAreaTLPos);              // 00468BE3 +0x6e8 C2DVector -> 0x004568BC
    context.Transfer(AugmentationMeshAreaBRPos);              // 00468BF1 +0x6f0 C2DVector -> 0x004568BC
    context.Transfer(AugmentDescriptionBoxTLPos);             // 00468BFF +0x6f8 C2DVector -> 0x004568BC
    context.Transfer(AugmentDescriptionBoxExtents);           // 00468C0D +0x700 C2DVector -> 0x004568BC
    context.Transfer(AugmentAugmentationInfoTLPos);           // 00468C1B +0x708 C2DVector -> 0x004568BC
    context.Transfer(AugmentAugmentationLabelTLPos);          // 00468C29 +0x710 C2DVector -> 0x004568BC
    context.Transfer(AugmentAugmentationInfoTextOffset);      // 00468C37 +0x718 C2DVector -> 0x004568BC
    context.Transfer(AugmentAugmentationSlotTLPos);           // 00468C45 +0x720 C2DVector -> 0x004568BC
    context.Transfer(AugmentAugmentationSlotTextOffset);      // 00468C53 +0x728 C2DVector -> 0x004568BC
    context.Transfer(AugmentAugmentationWeaponTLPos);         // 00468C61 +0x730 C2DVector -> 0x004568BC
    context.Transfer(AugmentAugmentationWeaponTextOffset);    // 00468C6F +0x738 C2DVector -> 0x004568BC
    context.Transfer(AugmentTextMaxSlots);                    // 00468C7D +0x750 ulong -> 0x00431020
    context.Transfer(AugmentTextNumSlots);                    // 00468C8B +0x754 ulong -> 0x00431020
    context.Transfer(AugmentTextUpgradeCost);                 // 00468C99 +0x758 ulong -> 0x00431020
    context.Transfer(AugmentTextGold);                        // 00468CA7 +0x75c ulong -> 0x00431020
    context.Transfer(AugmentTextDamageMultiplier);            // 00468CB5 +0x760 ulong -> 0x00431020
    context.Transfer(AugmentTextFireStrength);                // 00468CC3 +0x764 ulong -> 0x00431020
    context.Transfer(AugmentTextSlot);                        // 00468CD1 +0x768 ulong -> 0x00431020
    context.Transfer(AugmentTextWeapons);                     // 00468CDF +0x770 ulong -> 0x00431020
    context.Transfer(AugmentTextAugmentations);               // 00468CED +0x76c ulong -> 0x00431020
    context.Transfer(AugmentTextDivider);                     // 00468CFB +0x774 ulong -> 0x00431020
    context.Transfer(AugmentShadedCircleTLPos);               // 00468D09 +0x778 C2DVector -> 0x004568BC
    context.Transfer(AugmentShadedCircleExtents);             // 00468D17 +0x780 C2DVector -> 0x004568BC
    context.Transfer(MainEdgedButtonTitleTLPos);              // 00468D25 +0x990 C2DVector -> 0x004568BC
    context.Transfer(MainEdgedButtonOwnedTLPos);              // 00468D33 +0x998 C2DVector -> 0x004568BC
    context.Transfer(MainEdgedButtonPriceTLPos);              // 00468D41 +0x9a0 C2DVector -> 0x004568BC
    context.Transfer(MainEdgedButtonCostTLPos);               // 00468D4F +0x9a8 C2DVector -> 0x004568BC
    context.Transfer(MainEdgedButtonExtents);                 // 00468D5D +0x9b0 C2DVector -> 0x004568BC
    context.Transfer(MainDescriptionBoxC);                    // 00468D6B +0x9b8 CEngineGraphic -> 0x00451E11
    context.Transfer(MainDescriptionBoxTL);                   // 00468D79 +0x9c6 CEngineGraphic -> 0x00451E11
    context.Transfer(MainDescriptionBoxTR);                   // 00468D87 +0x9d4 CEngineGraphic -> 0x00451E11
    context.Transfer(MainDescriptionBoxBL);                   // 00468D95 +0x9e2 CEngineGraphic -> 0x00451E11
    context.Transfer(MainDescriptionBoxBR);                   // 00468DA3 +0x9f0 CEngineGraphic -> 0x00451E11
    context.Transfer(MainDescriptionBoxTLPos);                // 00468DB1 +0x9fe C2DVector -> 0x004568BC
    context.Transfer(MainDescriptionBoxExtents);              // 00468DBF +0xa06 C2DVector -> 0x004568BC
    context.Transfer(MainShadedCircleTLPos);                  // 00468DCD +0xa0e C2DVector -> 0x004568BC
    context.Transfer(MainShadedCircleExtents);                // 00468DDB +0xa16 C2DVector -> 0x004568BC
    context.Transfer(MainAnimBoxTLPos);                       // 00468DE9 +0xa1e C2DVector -> 0x004568BC
    context.Transfer(MainAnimBoxExtents);                     // 00468DF7 +0xa26 C2DVector -> 0x004568BC
    context.Transfer(MainQuickAccessItemFlashTime);           // 00468E05 +0xa30 float -> 0x00431061
}
int main()
{
    int ok = sizeof(CInventoryDef) == 0xA34 && offsetof(CInventoryDef, InitialCategoryMovementRepeatDelay) == 0x3C
          && offsetof(CInventoryDef, MainQuickAccessItemFlashTime) == 0xA30 && offsetof(CInventoryDef, TradeShadedCircleTLPos) == 0x6D8;
    static unsigned char src[sizeof(CInventoryDef)], dst[sizeof(CInventoryDef)];
    CInventoryDef* a = new (src) CInventoryDef; CInventoryDef* b = new (dst) CInventoryDef;
    unsigned char* pa = (unsigned char*)a; unsigned char* pb = (unsigned char*)b;
    for (size_t i = sizeof(DefBase); i < sizeof(CInventoryDef); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a->TradeShadedCircleExtents.X = 5.0f;
    CPersistContext w; w.base = pa; w.mode = 0; w.n = 0; w.pos = 0;
    a->Transfer(w);
    int dup = 0, ext = 0;
    for (int i = 0; i < w.n && i < 400; ++i) { if (g_offs[i] == 0x6D8) ++dup; if (g_offs[i] == 0x6E0) ++ext; }
    ok = ok && w.n == 389 && g_offs[0] == 0x3C && g_offs[388] == 0xA30 && dup == 2 && ext == 0;
    CPersistContext r; r.base = pb; r.mode = 1; r.n = 0; r.pos = 0;
    b->Transfer(r);
    ok = ok && r.n == 389 && r.pos == w.pos;
    ok = ok && b->MainQuickAccessItemFlashTime == a->MainQuickAccessItemFlashTime && b->TradeShadedCircleTLPos.Y == a->TradeShadedCircleTLPos.Y
            && b->InventoryIdentifier == a->InventoryIdentifier && b->TradeShadedCircleExtents.X == 0.0f
            && std::memcmp(&b->NextItemOffset, &a->NextItemOffset, sizeof(C2DVector)) == 0;
    if (!ok) { std::printf("004678e2_TEST FAIL\n"); return 1; }
    std::printf("004678e2_TEST PASS\n");
    return 0;
}