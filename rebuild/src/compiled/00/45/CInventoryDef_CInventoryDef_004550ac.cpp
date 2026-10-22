// CInventoryDef::CInventoryDef() -- construct the inventory screen definition: the def base, then the member constructors (the item lists and colour tables empty, the font string reference unset, every engine graphic cleared)
// Leaf: CInventoryDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004550AC, 1016 bytes (`ret`; unaligned entry, size-optimised def module).
// Identity: stores vtable 0x01233404 (CInventoryDef); the catalog's CPSCOrbit label is wrong.
// Callees: the def base ctor 0x00430370 (CDefClassBase, 0x3c bytes).
// Layout: Ego_r.pdb CInventoryDef member list in order; the retail STLport vectors are 12 bytes (PDB 16), CDefString a
//   long, and C2DVector / CEngineGraphic (long, two floats, two bools: 14 bytes) are 2-byte aligned (the PDB offsets
//   0x264, 0x272, 0xa4a, the retail inline stride 0xe). No engine header.
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 2)
struct C2DVector { float X, Y; };
struct CEngineGraphic {
    long  Graphic;
    float A;
    float B;
    bool  C;
    bool  D;
    CEngineGraphic() : Graphic(0), A(0.0f), B(0.0f), C(false), D(false) {}
};
#pragma pack(pop)
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
enum EInventoryIdentifier { INVENTORY_IDENTIFIER_0 };
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) {}
};
struct CRetailVector {                                 // STLport vector (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CDefClassBase {
    unsigned char _body[0x3c - 4];
    CDefClassBase();                                    // 0x00430370
    virtual ~CDefClassBase();
};
struct CInventoryDef : CDefClassBase {
    float                  InitialCategoryMovementRepeatDelay;   // PDB +0x40
    float                  ContinuedCategoryMovementRepeatDelay;   // PDB +0x44
    float                  InitialItemMovementRepeatDelay;   // PDB +0x48
    float                  ContinuedItemMovementRepeatDelay;   // PDB +0x4c
    long                   MovementSound;   // PDB +0x50
    long                   NoMovementSound;   // PDB +0x54
    long                   InventoryOpenSound;   // PDB +0x58
    bool                   TrackSelectedItems;   // PDB +0x5c
    CRetailVector          InitialInventoryClothingItems;   // PDB +0x60
    CRetailVector          InitialInventoryWeaponsItems;   // PDB +0x70
    CRetailVector          InitialInventoryStatsItems;   // PDB +0x80
    CRetailVector          InitialInventoryStandardItems;   // PDB +0x90
    C2DVector              ClothingShadedCircleTLPos;   // PDB +0xa0
    C2DVector              ClothingShadedCircleExtents;   // PDB +0xa8
    C2DVector              CategoriesTLPos;   // PDB +0xb0
    C2DVector              MeshAreaTLPos;   // PDB +0xb8
    C2DVector              MeshAreaBRPos;   // PDB +0xc0
    C2DVector              NextCategoryOffset;   // PDB +0xc8
    C2DVector              FirstItemOffsetFromCategory;   // PDB +0xd0
    C2DVector              NextItemOffset;   // PDB +0xd8
    CDefString             GenericFont;   // PDB +0xe0
    CRGBColour             BackGroundColour;   // PDB +0xe4
    long                   ClothingDoll;   // PDB +0xe8
    C3DVector              DollCentreOffset;   // PDB +0xec
    float                  DollBoundingSphereRadius;   // PDB +0xf8
    C2DVector              TotalExperienceTextPos;   // PDB +0xfc
    unsigned long          TotalExperienceText;   // PDB +0x104
    C2DVector              SpendingExperienceTextPos;   // PDB +0x108
    unsigned long          SpendingExperienceText;   // PDB +0x110
    C2DVector              StrengthLevelTextPos;   // PDB +0x114
    CRGBColour             StrengthLevelColour;   // PDB +0x11c
    C2DVector              SkillLevelTextPos;   // PDB +0x120
    CRGBColour             SkillLevelColour;   // PDB +0x128
    C2DVector              WillLevelTextPos;   // PDB +0x12c
    CRGBColour             WillLevelColour;   // PDB +0x134
    C2DVector              RenownLevelTextPos;   // PDB +0x138
    C2DVector              HeroLevelTextPos;   // PDB +0x140
    C2DVector              StatLevelsFirstLineTextPos;   // PDB +0x148
    C2DVector              StatLevelsNextLineTextOffset;   // PDB +0x150
    C2DVector              ButtonAssignmentTextPos;   // PDB +0x158
    C2DVector              ButtonAssignmentTextOffset;   // PDB +0x160
    CRetailVector          StatLevelsTexts;   // PDB +0x168
    CRetailVector          StatLevelsColours;   // PDB +0x178
    long                   WeaponsTextSeparator;   // PDB +0x188
    long                   WeaponsTextNumberOfSlots;   // PDB +0x18c
    long                   WeaponsTextSlot;   // PDB +0x190
    long                   WeaponsTextSuccessfulAugmentation;   // PDB +0x194
    long                   WeaponsTextNoFreeSlots;   // PDB +0x198
    long                   WeaponsTextReadyToAugment;   // PDB +0x19c
    long                   WeaponsTextNotReadyToAugment;   // PDB +0x1a0
    C2DVector              WeaponsDescriptionBoxTLPos;   // PDB +0x1a4
    C2DVector              WeaponsDescriptionBoxExtents;   // PDB +0x1ac
    C2DVector              WeaponsActiveMeleeIconPos;   // PDB +0x1b4
    C2DVector              WeaponsActiveRangeIconPos;   // PDB +0x1bc
    C2DVector              WeaponAugmentationTextStartPos;   // PDB +0x1c4
    C2DVector              WeaponAugmentationTextNewLineOffset;   // PDB +0x1cc
    C2DVector              AugmentationStatusTextAreaTLPos;   // PDB +0x1d4
    C2DVector              AugmentationStatusTextAreaBRPos;   // PDB +0x1dc
    float                  TimeToDisplayWarningMessageInSeconds;   // PDB +0x1e4
    C2DVector              WeaponsShadedCircleTLPos;   // PDB +0x1e8
    C2DVector              WeaponsShadedCircleExtents;   // PDB +0x1f0
    float                  NumberOfSecondsForObjectRotation;   // PDB +0x1f8
    long                   MaxNumberOfItemsToDisplayPerCategory;   // PDB +0x1fc
    long                   MaxNumberOfCategoriesToDisplay;   // PDB +0x200
    long                   MaxNumberOfScreenNamesToDisplay;   // PDB +0x204
    unsigned long          InventoryName;   // PDB +0x208
    EInventoryIdentifier   InventoryIdentifier;   // PDB +0x20c
    C2DVector              MapScreenMapDisplayBoxTLPos;   // PDB +0x210
    C2DVector              MapScreenMapDisplayBoxBRPos;   // PDB +0x218
    C2DVector              MapScreenRegionNamePos;   // PDB +0x220
    long                   MapScreenWorldMapGraphic;   // PDB +0x228
    C2DVector              MapScreenWorldMapPos;   // PDB +0x22c
    long                   MapScreenRegionUnknownGraphic;   // PDB +0x234
    long                   MapScreenRegionUnknownHighlightGraphic;   // PDB +0x238
    long                   MapScreenRegionHeroGraphic;   // PDB +0x23c
    long                   MapScreenRegionQuestGraphic;   // PDB +0x240
    long                   MapScreenRegionTeleportGraphic;   // PDB +0x244
    long                   MapScreenRegionHouseGraphic;   // PDB +0x248
    C2DVector              StatsTitleTLPos;   // PDB +0x24c
    C2DVector              StatsShadedCircleTLPos;   // PDB +0x254
    C2DVector              StatsShadedCircleExtents;   // PDB +0x25c
    CEngineGraphic         StatsProgBarL;   // PDB +0x264
    CEngineGraphic         StatsProgBarC;   // PDB +0x272
    CEngineGraphic         StatsProgBarR;   // PDB +0x280
    CEngineGraphic         StatsProgBarValue;   // PDB +0x28e
    C2DVector              StatsProgBarValueOffset;   // PDB +0x29c
    C2DVector              StatsPersonalityHeadingTLPos;   // PDB +0x2a4
    C2DVector              StatsPersonalityHeadingBRPos;   // PDB +0x2ac
    unsigned long          StatsPersonalityRenownLowerLabel;   // PDB +0x2b4
    unsigned long          StatsPersonalityRenownUpperLabel;   // PDB +0x2b8
    unsigned long          StatsPersonalityGoodnessLowerLabel;   // PDB +0x2bc
    unsigned long          StatsPersonalityGoodnessUpperLabel;   // PDB +0x2c0
    unsigned long          StatsPersonalityAttractivenessLowerLabel;   // PDB +0x2c4
    unsigned long          StatsPersonalityAttractivenessUpperLabel;   // PDB +0x2c8
    unsigned long          StatsPersonalityScarinessLowerLabel;   // PDB +0x2cc
    unsigned long          StatsPersonalityScarinessUpperLabel;   // PDB +0x2d0
    unsigned long          StatsPersonalityNicenessLowerLabel;   // PDB +0x2d4
    unsigned long          StatsPersonalityNicenessUpperLabel;   // PDB +0x2d8
    C2DVector              StatsPersonalityLabelTextOffset;   // PDB +0x2dc
    C2DVector              StatsPersonalityLowerLabelTLPos;   // PDB +0x2e4
    float                  StatsPersonalityLowerLabelWidth;   // PDB +0x2ec
    float                  StatsPersonalityLowerLabelHeight;   // PDB +0x2f0
    float                  StatsPersonalitySliderWidth;   // PDB +0x2f4
    float                  StatsPersonalityUpperLabelWidth;   // PDB +0x2f8
    long                   StatsPersonalityDoll;   // PDB +0x2fc
    C3DVector              StatsPersonalityDollCentreOffset;   // PDB +0x300
    float                  StatsPersonalityDollBoundingSphereRadius;   // PDB +0x30c
    C2DVector              StatsStatisticsLabelTLPos;   // PDB +0x310
    float                  StatsStatisticsLabelWidth;   // PDB +0x318
    float                  StatsStatisticsLabelHeight;   // PDB +0x31c
    C2DVector              StatsStatisticsLabelTextOffset;   // PDB +0x320
    C2DVector              StatsStatisticsDataTextOffset;   // PDB +0x328
    C2DVector              StatsStatisticsHeadingTLPos;   // PDB +0x330
    C2DVector              StatsStatisticsHeadingBRPos;   // PDB +0x338
    unsigned long          StatsStatisticsPeopleKilledLabel;   // PDB +0x340
    unsigned long          StatsStatisticsMonstersKilledLabel;   // PDB +0x344
    unsigned long          StatsStatisticsMaxMultiplierLabel;   // PDB +0x348
    unsigned long          StatsStatisticsPreferredFoodLabel;   // PDB +0x34c
    unsigned long          StatsStatisticsDietaryTypeLabel;   // PDB +0x350
    unsigned long          StatsStatisticsGameTimeElapsedLabel;   // PDB +0x354
    unsigned long          StatsStatisticsNumberOfWivesLabel;   // PDB +0x358
    unsigned long          StatsStatisticsQuestsActiveLabel;   // PDB +0x35c
    unsigned long          StatsStatisticsQuestsFinishedLabel;   // PDB +0x360
    unsigned long          StatsStatisticsBoastsAchievedLabel;   // PDB +0x364
    C2DVector              StatsExperienceStrengthTextOffset;   // PDB +0x368
    C2DVector              StatsExperienceWillTextOffset;   // PDB +0x370
    C2DVector              StatsExperienceSkillTextOffset;   // PDB +0x378
    unsigned long          StatsExperienceStrengthText;   // PDB +0x380
    unsigned long          StatsExperienceWillText;   // PDB +0x384
    unsigned long          StatsExperienceSkillText;   // PDB +0x388
    C2DVector              StatsExperiencePoolGeneralTLPos;   // PDB +0x38c
    C2DVector              StatsExperiencePoolGeneralTextOffset;   // PDB +0x394
    C2DVector              StatsExperiencePoolStrengthGroupTLPos;   // PDB +0x39c
    C2DVector              StatsExperiencePoolSkillGroupTLPos;   // PDB +0x3a4
    C2DVector              StatsExperiencePoolWillGroupTLPos;   // PDB +0x3ac
    C2DVector              StatsExperiencePoolLabelTLPos;   // PDB +0x3b4
    C2DVector              StatsExperiencePoolLabelTextOffset;   // PDB +0x3bc
    float                  StatsExperiencePoolLabelWidth;   // PDB +0x3c4
    float                  StatsExperiencePoolLabelHeight;   // PDB +0x3c8
    float                  StatsExperiencePoolSliderWidth;   // PDB +0x3cc
    unsigned long          StatsExperiencePoolStrengthLabel;   // PDB +0x3d0
    unsigned long          StatsExperiencePoolSkillLabel;   // PDB +0x3d4
    unsigned long          StatsExperiencePoolWillLabel;   // PDB +0x3d8
    unsigned long          StatsExperiencePoolPhysiqueLabel;   // PDB +0x3dc
    unsigned long          StatsExperiencePoolHealthLabel;   // PDB +0x3e0
    unsigned long          StatsExperiencePoolToughnessLabel;   // PDB +0x3e4
    unsigned long          StatsExperiencePoolSpeedLabel;   // PDB +0x3e8
    unsigned long          StatsExperiencePoolAccuracyLabel;   // PDB +0x3ec
    unsigned long          StatsExperiencePoolStealthLabel;   // PDB +0x3f0
    unsigned long          StatsExperiencePoolWeaponMagicLabel;   // PDB +0x3f4
    unsigned long          StatsExperiencePoolAbilityMagicLabel;   // PDB +0x3f8
    unsigned long          StatsExperiencePoolPureMagicLabel;   // PDB +0x3fc
    float                  StatsExperienceStrengthBarValue;   // PDB +0x400
    float                  StatsExperienceSkillBarValue;   // PDB +0x404
    float                  StatsExperienceWillBarValue;   // PDB +0x408
    C2DVector              ExperienceGeneralPoolLabelTLPos;   // PDB +0x40c
    C2DVector              ExperienceGeneralPoolLabelTextOffset;   // PDB +0x414
    float                  ExperienceGeneralPoolWidth;   // PDB +0x41c
    float                  ExperienceGeneralPoolSliderWidth;   // PDB +0x420
    unsigned long          ExperienceGeneralPoolBarValue;   // PDB +0x424
    unsigned long          ExperienceGeneralPoolLabel;   // PDB +0x428
    C2DVector              ExperienceGeneralPoolSpendTextOffset;   // PDB +0x42c
    C2DVector              ExperienceStrengthPoolTLPos;   // PDB +0x434
    C2DVector              ExperienceSkillPoolTLPos;   // PDB +0x43c
    C2DVector              ExperienceWillPoolTLPos;   // PDB +0x444
    float                  ExperiencePoolWidth;   // PDB +0x44c
    float                  ExperiencePoolHeight;   // PDB +0x450
    unsigned long          ExperienceTypeStrengthLabel;   // PDB +0x454
    unsigned long          ExperienceTypeSkillLabel;   // PDB +0x458
    unsigned long          ExperienceTypeWillLabel;   // PDB +0x45c
    unsigned long          ExperienceTypePureMagicLabel;   // PDB +0x460
    unsigned long          ExperienceTypeAbilityMagicLabel;   // PDB +0x464
    unsigned long          ExperienceTypeWeaponMagicLabel;   // PDB +0x468
    C2DVector              ExperienceTypeTLPos;   // PDB +0x46c
    C2DVector              ExperienceTypeTLPosOffset;   // PDB +0x474
    C2DVector              ExperienceTypeTextOffset;   // PDB +0x47c
    C2DVector              ExperienceTypeSpendTextOffset;   // PDB +0x484
    float                  ExperienceTypeLabelWidth;   // PDB +0x48c
    float                  ExperienceTypeLabelHeight;   // PDB +0x490
    float                  ExperienceTypeSliderWidth;   // PDB +0x494
    unsigned long          ExperienceStrengthPhysiqueLabel;   // PDB +0x498
    unsigned long          ExperienceStrengthHealthLabel;   // PDB +0x49c
    unsigned long          ExperienceStrengthToughnessLabel;   // PDB +0x4a0
    unsigned long          ExperienceSkillSpeedLabel;   // PDB +0x4a4
    unsigned long          ExperienceSkillAccuracyLabel;   // PDB +0x4a8
    unsigned long          ExperienceSkillStealthLabel;   // PDB +0x4ac
    unsigned long          ExperienceWeaponMagicBerserkLabel;   // PDB +0x4b0
    unsigned long          ExperienceWeaponMagicFlamingBladeLabel;   // PDB +0x4b4
    unsigned long          ExperienceWeaponMagicDoubleStrikeLabel;   // PDB +0x4b8
    unsigned long          ExperienceWeaponMagicLightningBladeLabel;   // PDB +0x4bc
    unsigned long          ExperienceAbilityMagicBattleChargeLabel;   // PDB +0x4c0
    unsigned long          ExperienceAbilityMagicAssassinRushLabel;   // PDB +0x4c4
    unsigned long          ExperienceAbilityMagicTimeSpellLabel;   // PDB +0x4c8
    unsigned long          ExperiencePureMagicBurningHandsLabel;   // PDB +0x4cc
    unsigned long          ExperiencePureMagicPhysicalShieldLabel;   // PDB +0x4d0
    unsigned long          ExperiencePureMagicForcePushLabel;   // PDB +0x4d4
    unsigned long          ExperiencePureMagicEnflameSpellLabel;   // PDB +0x4d8
    unsigned long          ExperiencePureMagicDrainLifeLabel;   // PDB +0x4dc
    unsigned long          ExperiencePureMagicHealLifeLabel;   // PDB +0x4e0
    unsigned long          ExperiencePureMagicGhostSwordLabel;   // PDB +0x4e4
    float                  ExperienceStrengthBarValue;   // PDB +0x4e8
    float                  ExperienceSkillBarValue;   // PDB +0x4ec
    float                  ExperienceWillBarValue;   // PDB +0x4f0
    float                  ExperienceStrengthPhysiqueBarValue;   // PDB +0x4f4
    float                  ExperienceStrengthHealthBarValue;   // PDB +0x4f8
    float                  ExperienceStrengthToughnessBarValue;   // PDB +0x4fc
    float                  ExperienceSkillSpeedBarValue;   // PDB +0x500
    float                  ExperienceSkillAccuracyBarValue;   // PDB +0x504
    float                  ExperienceSkillHealthBarValue;   // PDB +0x508
    float                  ExperienceWeaponMagicBerserkBarValue;   // PDB +0x50c
    float                  ExperienceWeaponMagicFlamingBladeBarValue;   // PDB +0x510
    float                  ExperienceWeaponMagicDoubleStrikeBarValue;   // PDB +0x514
    float                  ExperienceAbilityMagicBattleChargeBarValue;   // PDB +0x518
    float                  ExperienceAbilityMagicAssassinRushBarValue;   // PDB +0x51c
    float                  ExperienceAbilityMagicTimeSpellBarValue;   // PDB +0x520
    float                  ExperiencePureMagicBurningHandsBarValue;   // PDB +0x524
    float                  ExperiencePureMagicPhysicalShieldBarValue;   // PDB +0x528
    float                  ExperiencePureMagicForcePushBarValue;   // PDB +0x52c
    CEngineGraphic         ExperienceProgBarL;   // PDB +0x530
    CEngineGraphic         ExperienceProgBarC;   // PDB +0x53e
    CEngineGraphic         ExperienceProgBarR;   // PDB +0x54c
    CEngineGraphic         ExperienceProgBarValue;   // PDB +0x55a
    C2DVector              ExperienceProgBarValueOffset;   // PDB +0x568
    long                   ExperienceSpendTimeOffset;   // PDB +0x570
    long                   ExperienceSpendRate;   // PDB +0x574
    CRetailVector          ExperienceItemLabelStat;   // PDB +0x578
    CRetailVector          ExperienceItemLabelAbility;   // PDB +0x588
    CEngineGraphic         ExperienceSpendGreen;   // PDB +0x598
    CEngineGraphic         ExperienceSpendGreenOutline;   // PDB +0x5a6
    CEngineGraphic         ExperienceSpendGreenEnd;   // PDB +0x5b4
    CEngineGraphic         ExperienceSpendRed;   // PDB +0x5c2
    CEngineGraphic         ExperienceSpendRedOutline;   // PDB +0x5d0
    CEngineGraphic         ExperienceSpendRedEnd;   // PDB +0x5de
    C2DVector              ExperienceDescriptionTextTLPos;   // PDB +0x5ec
    C2DVector              ExperienceDescriptionTextOffset;   // PDB +0x5f4
    unsigned long          QuestCardInfoQuestNameLabel;   // PDB +0x5fc
    unsigned long          QuestCardInfoMoralityLabel;   // PDB +0x600
    unsigned long          QuestCardInfoCoreLabel;   // PDB +0x604
    unsigned long          QuestCardInfoDetailsLabel;   // PDB +0x608
    unsigned long          QuestCardInfoRegionLabel;   // PDB +0x60c
    unsigned long          QuestCardInfoOriginatorLabel;   // PDB +0x610
    unsigned long          QuestCardInfoDescriptionLabel;   // PDB +0x614
    unsigned long          QuestCardInfoSuccessLabel;   // PDB +0x618
    unsigned long          QuestCardInfoFailureLabel;   // PDB +0x61c
    unsigned long          QuestCardInfoRewardsLabel;   // PDB +0x620
    unsigned long          QuestCardInfoRenownLabel;   // PDB +0x624
    unsigned long          QuestCardInfoTrophyLabel;   // PDB +0x628
    unsigned long          QuestCardInfoBoastsLabel;   // PDB +0x62c
    unsigned long          QuestCardInfoStatusLabel;   // PDB +0x630
    unsigned long          QuestCardInfoQuestNameText;   // PDB +0x634
    unsigned long          QuestCardInfoMoralityText;   // PDB +0x638
    unsigned long          QuestCardInfoCoreText;   // PDB +0x63c
    unsigned long          QuestCardInfoDetailsText;   // PDB +0x640
    unsigned long          QuestCardInfoRegionText;   // PDB +0x644
    unsigned long          QuestCardInfoOriginatorText;   // PDB +0x648
    unsigned long          QuestCardInfoDescriptionText;   // PDB +0x64c
    unsigned long          QuestCardInfoSuccessText;   // PDB +0x650
    unsigned long          QuestCardInfoFailureText;   // PDB +0x654
    unsigned long          QuestCardInfoRewardsText;   // PDB +0x658
    unsigned long          QuestCardInfoRenownText;   // PDB +0x65c
    unsigned long          QuestCardInfoTrophyText;   // PDB +0x660
    unsigned long          QuestCardInfoBoastsText;   // PDB +0x664
    unsigned long          QuestCardInfoStatusText;   // PDB +0x668
    C2DVector              TradeEdgedButtonCostTLPos;   // PDB +0x66c
    C2DVector              TradeEdgedButtonOwnedTLPos;   // PDB +0x674
    C2DVector              TradeEdgedButtonProfitTLPos;   // PDB +0x67c
    C2DVector              TradeEdgedButtonTitleTLPos;   // PDB +0x684
    C2DVector              TradeEdgedButtonCashTLPos;   // PDB +0x68c
    C2DVector              TradeEdgedButtonExtents;   // PDB +0x694
    C2DVector              TradeAnimImageBoxTLPos;   // PDB +0x69c
    C2DVector              TradeAnimImageBoxExtents;   // PDB +0x6a4
    float                  TradeAnimImageXRotationTime;   // PDB +0x6ac
    float                  TradeAnimImageYRotationTime;   // PDB +0x6b0
    float                  TradeAnimImageZRotationTime;   // PDB +0x6b4
    C2DVector              TradeEquippedItemTLPos;   // PDB +0x6b8
    C2DVector              TradeEquippedItemExtents;   // PDB +0x6c0
    CEngineGraphic         TradeEquippedItemGraphic;   // PDB +0x6c8
    C2DVector              TradeDescriptionBoxTLPos;   // PDB +0x6d6
    C2DVector              TradeDescriptionBoxExtents;   // PDB +0x6de
    unsigned long          TradeTextOwned;   // PDB +0x6e8
    unsigned long          TradeTextDivider;   // PDB +0x6ec
    unsigned long          TradeTextCost;   // PDB +0x6f0
    unsigned long          TradeTextGold;   // PDB +0x6f4
    unsigned long          TradeTextProfit;   // PDB +0x6f8
    C2DVector              TradeShadedCircleTLPos;   // PDB +0x6fc
    C2DVector              TradeShadedCircleExtents;   // PDB +0x704
    C2DVector              AugmentationMeshAreaTLPos;   // PDB +0x70c
    C2DVector              AugmentationMeshAreaBRPos;   // PDB +0x714
    C2DVector              AugmentDescriptionBoxTLPos;   // PDB +0x71c
    C2DVector              AugmentDescriptionBoxExtents;   // PDB +0x724
    C2DVector              AugmentAugmentationInfoTLPos;   // PDB +0x72c
    C2DVector              AugmentAugmentationLabelTLPos;   // PDB +0x734
    C2DVector              AugmentAugmentationInfoTextOffset;   // PDB +0x73c
    C2DVector              AugmentAugmentationSlotTLPos;   // PDB +0x744
    C2DVector              AugmentAugmentationSlotTextOffset;   // PDB +0x74c
    C2DVector              AugmentAugmentationWeaponTLPos;   // PDB +0x754
    C2DVector              AugmentAugmentationWeaponTextOffset;   // PDB +0x75c
    C2DVector              AugmentTitleTLPos;   // PDB +0x764
    C2DVector              AugmentTitleTextOffset;   // PDB +0x76c
    unsigned long          AugmentTextMaxSlots;   // PDB +0x774
    unsigned long          AugmentTextNumSlots;   // PDB +0x778
    unsigned long          AugmentTextUpgradeCost;   // PDB +0x77c
    unsigned long          AugmentTextGold;   // PDB +0x780
    unsigned long          AugmentTextDamageMultiplier;   // PDB +0x784
    unsigned long          AugmentTextFireStrength;   // PDB +0x788
    unsigned long          AugmentTextSlot;   // PDB +0x78c
    unsigned long          AugmentTextAugmentations;   // PDB +0x790
    unsigned long          AugmentTextWeapons;   // PDB +0x794
    unsigned long          AugmentTextDivider;   // PDB +0x798
    C2DVector              AugmentShadedCircleTLPos;   // PDB +0x79c
    C2DVector              AugmentShadedCircleExtents;   // PDB +0x7a4
    float                  InventoryAnimImageXRotationTime;   // PDB +0x7ac
    float                  InventoryAnimImageYRotationTime;   // PDB +0x7b0
    float                  InventoryAnimImageZRotationTime;   // PDB +0x7b4
    CRGBColour             QuestCardFeatColour;   // PDB +0x7b8
    CRGBColour             QuestCardCoreColour;   // PDB +0x7bc
    CRGBColour             QuestCardExclusiveColour;   // PDB +0x7c0
    C2DVector              QuestCardInfoTLPos;   // PDB +0x7c4
    C2DVector              QuestCardInfo2TLPos;   // PDB +0x7cc
    C2DVector              QuestCardNoQuestsTLPos;   // PDB +0x7d4
    unsigned long          QuestCardNoQuestsText;   // PDB +0x7dc
    C2DVector              QuestCardInfoTextOffset;   // PDB +0x7e0
    C2DVector              QuestCardEdgedButtonTitleTLPos;   // PDB +0x7e8
    C2DVector              QuestCardEdgedButtonExtents;   // PDB +0x7f0
    CRetailVector          ExpSpendItemLabelStat;   // PDB +0x7f8
    CRetailVector          ExpSpendItemLabelAbility;   // PDB +0x808
    long                   ExpSpendMaxItemsToDisplay;   // PDB +0x818
    C2DVector              ExpSpendButtonTLPos;   // PDB +0x81c
    C2DVector              ExpSpendButtonTLPosOffset;   // PDB +0x824
    C2DVector              ExpSpendButtonTextOffset;   // PDB +0x82c
    CEngineGraphic         ExpSpendButtonSelL;   // PDB +0x834
    CEngineGraphic         ExpSpendButtonSelC;   // PDB +0x842
    CEngineGraphic         ExpSpendButtonSelR;   // PDB +0x850
    CEngineGraphic         ExpSpendButtonL;   // PDB +0x85e
    CEngineGraphic         ExpSpendButtonC;   // PDB +0x86c
    CEngineGraphic         ExpSpendButtonR;   // PDB +0x87a
    C2DVector              ExpSpendDescriptionBoxTLPos;   // PDB +0x888
    C2DVector              ExpSpendDescriptionBoxExtents;   // PDB +0x890
    C2DVector              ExpSpendArrowUpTLPos;   // PDB +0x898
    C2DVector              ExpSpendArrowDnTLPos;   // PDB +0x8a0
    C2DVector              ExpSpendGeneralPoolLabelTLPos;   // PDB +0x8a8
    C2DVector              ExpSpendGeneralPoolLabelTextOffset;   // PDB +0x8b0
    C2DVector              ExpSpendGeneralPoolSpendTextOffset;   // PDB +0x8b8
    float                  ExpSpendGeneralPoolWidth;   // PDB +0x8c0
    float                  ExpSpendGeneralPoolSliderWidth;   // PDB +0x8c4
    C2DVector              ExpSpendStrengthPoolTLPos;   // PDB +0x8c8
    C2DVector              ExpSpendSkillPoolTLPos;   // PDB +0x8d0
    C2DVector              ExpSpendWillPoolTLPos;   // PDB +0x8d8
    float                  ExpSpendPoolWidth;   // PDB +0x8e0
    float                  ExpSpendPoolHeight;   // PDB +0x8e4
    CEngineGraphic         ExpSpendProgBarL;   // PDB +0x8e8
    CEngineGraphic         ExpSpendProgBarC;   // PDB +0x8f6
    CEngineGraphic         ExpSpendProgBarR;   // PDB +0x904
    CEngineGraphic         ExpSpendProgBarValue;   // PDB +0x912
    C2DVector              ExpSpendProgBarValueOffset;   // PDB +0x920
    C2DVector              ExpSpendTypeTextOffset;   // PDB +0x928
    long                   ExpSpendGeneralPoolBarValue;   // PDB +0x930
    long                   ExpSpendGeneralPoolLabel;   // PDB +0x934
    long                   ExpSpendTypeStrengthLabel;   // PDB +0x938
    long                   ExpSpendTypeSkillLabel;   // PDB +0x93c
    long                   ExpSpendTypeWillLabel;   // PDB +0x940
    float                  ExpSpendStrengthBarValue;   // PDB +0x944
    float                  ExpSpendSkillBarValue;   // PDB +0x948
    float                  ExpSpendWillBarValue;   // PDB +0x94c
    long                   ExpSpendRate;   // PDB +0x950
    long                   ExpSpendTimeOffset;   // PDB +0x954
    CEngineGraphic         ExpSpendGreen;   // PDB +0x958
    CEngineGraphic         ExpSpendGreenOutline;   // PDB +0x966
    CEngineGraphic         ExpSpendGreenEnd;   // PDB +0x974
    CEngineGraphic         ExpSpendRed;   // PDB +0x982
    CEngineGraphic         ExpSpendRedOutline;   // PDB +0x990
    CEngineGraphic         ExpSpendRedEnd;   // PDB +0x99e
    C2DVector              ExpSpendDescriptionTextTLPos;   // PDB +0x9ac
    C2DVector              ExpSpendDescriptionTextOffset;   // PDB +0x9b4
    C2DVector              MainEdgedButtonTitleTLPos;   // PDB +0x9bc
    C2DVector              MainEdgedButtonOwnedTLPos;   // PDB +0x9c4
    C2DVector              MainEdgedButtonPriceTLPos;   // PDB +0x9cc
    C2DVector              MainEdgedButtonCostTLPos;   // PDB +0x9d4
    C2DVector              MainEdgedButtonExtents;   // PDB +0x9dc
    CEngineGraphic         MainDescriptionBoxC;   // PDB +0x9e4
    CEngineGraphic         MainDescriptionBoxTL;   // PDB +0x9f2
    CEngineGraphic         MainDescriptionBoxTR;   // PDB +0xa00
    CEngineGraphic         MainDescriptionBoxBL;   // PDB +0xa0e
    CEngineGraphic         MainDescriptionBoxBR;   // PDB +0xa1c
    C2DVector              MainDescriptionBoxTLPos;   // PDB +0xa2a
    C2DVector              MainDescriptionBoxExtents;   // PDB +0xa32
    C2DVector              MainShadedCircleTLPos;   // PDB +0xa3a
    C2DVector              MainShadedCircleExtents;   // PDB +0xa42
    C2DVector              MainAnimBoxTLPos;   // PDB +0xa4a
    C2DVector              MainAnimBoxExtents;   // PDB +0xa52
    float                  MainQuickAccessItemFlashTime;   // PDB +0xa5c
    CInventoryDef();
};
CInventoryDef::CInventoryDef()
{
}