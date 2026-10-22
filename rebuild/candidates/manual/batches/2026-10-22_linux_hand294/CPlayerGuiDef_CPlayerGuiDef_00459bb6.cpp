// CPlayerGuiDef::CPlayerGuiDef() -- construct the player-GUI definition: the CParentDefClassBase base, this vtable, then every non-POD member (35 STLport vectors zeroed inline, 7 CCharStrings and 3 maps constructed out of line)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00459BB6, length 622 bytes (no EH frame: the def TU has no unwinding).
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370; CCharString::CCharString() 0x0099E4B0 (x7);
// map<CCharString, long> ctor 0x00430345 (ScriptSprites, MiniMapGraphics); map<CCharString, EGameAction> ctor 0x00454AEF
// (GameActionValues). Vtable 0x012352DC.
// Layout: Ego_r.pdb CPlayerGuiDef (0xb50) with the retail sizes: the CParentDefClassBase base is 0x3c (PDB 0x40, as the
// landed CCardPositionsDef ctor 0x004F9D15 shows), and every STLport vector and map is 12 bytes (PDB 16), so retail
// is 0xab4 bytes; every computed offset agrees with the constructor's stores (StealthEyeGraphicFrames +0x150 ..
// RegionGraphFileName +0xa94).
// Shape: the members are constructed in declaration order from one zero register (edi); the vector default
// constructor stores _M_start, _M_finish, _M_end_of_storage through `lea eax,[esi+off]`.
#pragma optimize("s", on)

struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
class CCharString {
public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
};
struct CQuickAccessItemButtonGuiDef;
struct CAbilityButtonGuiDef;
template <class T> struct allocator {                  // STLport allocator<T>: empty
    allocator() { }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {   // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> struct _Vector_base {               // STLport _Vector_base<T, allocator<T> >
    T*                    _M_start;
    T*                    _M_finish;
    _STLP_alloc_proxy<T>  _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> struct vector : _Vector_base<T> {   // STLport vector<T>, 12 bytes
    vector() : _Vector_base<T>(allocator<T>()) { }
};
struct CScriptSpriteMap {                              // map<CCharString, long>, 12 bytes
    void* _M_header;
    long  _M_node_count;
    long  _M_key_compare;
    CScriptSpriteMap();                                // 0x00430345
};
struct CGameActionMap {                                // map<CCharString, EGameAction>, 12 bytes
    void* _M_header;
    long  _M_node_count;
    long  _M_key_compare;
    CGameActionMap();                                  // 0x00454AEF
};
struct CParentDefClassBase {                           // retail 0x3c bytes
    virtual void v00();
    unsigned char _body[0x38];
    CParentDefClassBase();                             // 0x00430370
};

struct CPlayerGuiDef : CParentDefClassBase {
    long                         InventoryNoMovementSound;                   // +0x03c
    float                        SecondsToFadeWholeGui;                      // +0x040
    float                        AlphaValueToFadeWholeGui;                   // +0x044
    CRGBColour                   AmbientColour;                              // +0x048
    CRGBColour                   DiffuseColour;                              // +0x04c
    CRGBColour                   BacklightColour;                            // +0x050
    C3DVector                    DiffuseVector;                              // +0x054
    C2DVector                    GuildSealPos;                               // +0x060
    long                         GuildSealFlashCycles;                       // +0x068
    float                        GuildSealFlashCycleSeconds;                 // +0x06c
    float                        GuildSealReminderTimeSeconds;               // +0x070
    C2DVector                    HealthBarPos;                               // +0x074
    long                         HealthBarLeftGraphic;                       // +0x07c
    C2DVector                    HealthBarMiddleOffset;                      // +0x080
    float                        HealthBarMiddleLength;                      // +0x088
    long                         HealthBarMiddleGraphic;                     // +0x08c
    long                         HealthBarRightGraphic;                      // +0x090
    C2DVector                    HealthBarHealthOffset;                      // +0x094
    long                         HealthBarHealthGraphic;                     // +0x09c
    long                         HealthBarHealthEvilGraphic;                 // +0x0a0
    long                         HealthBarHealthGoodGraphic;                 // +0x0a4
    C2DVector                    HealthBarHealthEffectOffset;                // +0x0a8
    C2DVector                    HealthBarHealthIncreaseEffectOffset;        // +0x0b0
    long                         HealthBarHealthDefaultEffect;               // +0x0b8
    long                         HealthBarHealthIncreaseEffect;              // +0x0bc
    long                         HealthBarHealthLevelUpEffect;               // +0x0c0
    long                         HealthBarHealthAdrenalineEffect;            // +0x0c4
    long                         HealthBarHealthBerserkEffect;               // +0x0c8
    long                         HealthBarHealthLowHealthEffect;             // +0x0cc
    float                        HealthBarHealthLowPercentage;               // +0x0d0
    C2DVector                    HealthBarHealthInnerOffset;                 // +0x0d4
    long                         HealthBarHealthInnerGraphic;                // +0x0dc
    long                         HealthBarHealthEndGraphic;                  // +0x0e0
    C2DVector                    HealthBarResurrectionOffset;                // +0x0e4
    CRGBColour                   HealthBarResurrectionColour;                // +0x0ec
    C2DVector                    HealthBarStaminaBackOffset;                 // +0x0f0
    C2DVector                    HealthBarStaminaOffset;                     // +0x0f8
    long                         HealthBarStaminaGraphic;                    // +0x100
    C2DVector                    HealthBarStaminaInnerOffset;                // +0x104
    long                         HealthBarStaminaInnerGraphic;               // +0x10c
    long                         HealthBarStaminaEndGraphic;                 // +0x110
    C2DVector                    HealthBarStaminaEffectOffset;               // +0x114
    C2DVector                    HealthBarStaminaIncreaseEffectOffset;       // +0x11c
    float                        HealthBarStaminaIncreaseEffectWidth;        // +0x124
    float                        HealthBarStaminaIncreaseEffectSegmentWidth; // +0x128
    long                         HealthBarStaminaDefaultEffect;              // +0x12c
    long                         HealthBarStaminaIncreaseEffect;             // +0x130
    long                         HealthBarStaminaIncreaseSegmentEffect;      // +0x134
    C2DVector                    StealthEyePos;                              // +0x138
    long                         StealthEyeFriendGraphic;                    // +0x140
    long                         StealthEyeNeutralGraphic;                   // +0x144
    long                         StealthEyeEnemyGraphic;                     // +0x148
    long                         StealthEyeQuestionMarkGraphic;              // +0x14c
    vector<long>                 StealthEyeGraphicFrames;                    // +0x150
    C2DVector                    StealthEyeTextOffset;                       // +0x15c
    float                        StealthEyeSecondsToOpen;                    // +0x164
    float                        StealthEyeSecondsToThrob;                   // +0x168
    float                        StealthEyeThrobScale;                       // +0x16c
    long                         DPadRingsGraphic;                           // +0x170
    C2DVector                    DPadRingsPos;                               // +0x174
    long                         DPadRingsTopGraphic;                        // +0x17c
    C2DVector                    DPadRingsTopOffset;                         // +0x180
    long                         DPadRingsExtendGraphic;                     // +0x188
    vector<long>                 DPadRingsChargeGraphics;                    // +0x18c
    vector<C2DVector>            DPadRingsChargeOffsets;                     // +0x198
    vector<float>                DPadRingsChargeAngles;                      // +0x1a4
    long                         DPadRingsFavouriteGraphic;                  // +0x1b0
    long                         DPadRingsTopFavouriteGraphic;               // +0x1b4
    long                         DPadRingsTopAggressiveGraphic;              // +0x1b8
    long                         DPadGuildSealLevelUpGraphic;                // +0x1bc
    C2DVector                    DPadGuildSealTooltipOffset;                 // +0x1c0
    C2DVector                    DPadMiddleOffset;                           // +0x1c8
    long                         DPadArrowUpGraphic;                         // +0x1d0
    long                         DPadArrowUpDepGraphic;                      // +0x1d4
    long                         DPadArrowRightGraphic;                      // +0x1d8
    long                         DPadArrowRightDepGraphic;                   // +0x1dc
    long                         DPadArrowDownGraphic;                       // +0x1e0
    long                         DPadArrowDownDepGraphic;                    // +0x1e4
    long                         DPadArrowLeftGraphic;                       // +0x1e8
    long                         DPadArrowLeftDepGraphic;                    // +0x1ec
    long                         DPadGuildSealRecallGraphic;                 // +0x1f0
    long                         GuildSealLevelUpGraphic;                    // +0x1f4
    long                         GuildSealQuestGraphic;                      // +0x1f8
    long                         GuildSealRecallGraphic;                     // +0x1fc
    C2DVector                    GuildSealLevelUpOffset;                     // +0x200
    C2DVector                    GuildSealMainLevelUpOffset;                 // +0x208
    long                         ABXYRingsGraphic;                           // +0x210
    C2DVector                    ABXYRingsPos;                               // +0x214
    long                         ABXYRingsAlpha;                             // +0x21c
    long                         ABXYRingsTopGraphic;                        // +0x220
    C2DVector                    ABXYRingsTopOffset;                         // +0x224
    long                         ABXYMiddleGraphic;                          // +0x22c
    C2DVector                    ABXYMiddleOffset;                           // +0x230
    long                         ABXYMiddleUpGraphic;                        // +0x238
    long                         ABXYMiddleRightGraphic;                     // +0x23c
    long                         ABXYMiddleDownGraphic;                      // +0x240
    long                         ABXYMiddleLeftGraphic;                      // +0x244
    long                         ABXYRingsFavouriteGraphic;                  // +0x248
    long                         ABXYRingsTopFavouriteGraphic;               // +0x24c
    long                         WhiteButtonGraphic;                         // +0x250
    C2DVector                    WhiteButtonPos;                             // +0x254
    C2DVector                    WhiteButtonIconOffset;                      // +0x25c
    C2DVector                    WhiteButtonTooltipOffset;                   // +0x264
    long                         BlackButtonGraphic;                         // +0x26c
    C2DVector                    BlackButtonPos;                             // +0x270
    C2DVector                    BlackButtonIconOffset;                      // +0x278
    C2DVector                    BlackButtonTooltipOffset;                   // +0x280
    long                         TextBoxTLGraphic;                           // +0x288
    long                         TextBoxTMGraphic;                           // +0x28c
    long                         TextBoxTRGraphic;                           // +0x290
    long                         TextBoxMLGraphic;                           // +0x294
    long                         TextBoxMRGraphic;                           // +0x298
    long                         TextBoxBLGraphic;                           // +0x29c
    long                         TextBoxBMGraphic;                           // +0x2a0
    long                         TextBoxBRGraphic;                           // +0x2a4
    long                         TextBoxBackGraphic;                         // +0x2a8
    float                        TextBoxMaxWidth;                            // +0x2ac
    float                        TextBoxMinWidth;                            // +0x2b0
    float                        TextBoxMaxHeight;                           // +0x2b4
    float                        TextBoxMinHeight;                           // +0x2b8
    float                        TextBoxBorderWidth;                         // +0x2bc
    float                        TextBoxBorderHeight;                        // +0x2c0
    float                        TextBoxCutsceneSecondsBeforeSkip;           // +0x2c4
    float                        StatBoxMaxWidth;                            // +0x2c8
    long                         StatBoxMiddleGraphic;                       // +0x2cc
    C2DVector                    StatBoxLeftOffset;                          // +0x2d0
    C2DVector                    StatBoxRightOffset;                         // +0x2d8
    long                         StatBoxStartGraphic;                        // +0x2e0
    long                         StatBoxEndGraphic;                          // +0x2e4
    long                         StatBoxGoldGraphic;                         // +0x2e8
    long                         StatBoxGoldGainEffect;                      // +0x2ec
    long                         StatBoxGoldLossEffect;                      // +0x2f0
    C2DVector                    ClockPos;                                   // +0x2f4
    long                         ClockFaceGraphic;                           // +0x2fc
    long                         ClockHandGraphic;                           // +0x300
    C2DVector                    ClockRingPos;                               // +0x304
    long                         ClockRingGraphic;                           // +0x30c
    C2DVector                    ModePos;                                    // +0x310
    C2DVector                    ModeOffset;                                 // +0x318
    C2DVector                    ModeInnerOffset;                            // +0x320
    long                         ModeBorderGraphic;                          // +0x328
    long                         ModeLockOffGraphic;                         // +0x32c
    long                         ModeLockOnGraphic;                          // +0x330
    long                         ModeSneakOffGraphic;                        // +0x334
    long                         ModeSneakOnGraphic;                         // +0x338
    long                         ModeSafetyOffGraphic;                       // +0x33c
    long                         ModeSafetyOnGraphic;                        // +0x340
    C2DVector                    ModeTextOffset;                             // +0x344
    C2DVector                    CombatPos;                                  // +0x34c
    C2DVector                    CombatLockedOffset;                         // +0x354
    vector<long>                 CombatLockedGraphics;                       // +0x35c
    C2DVector                    CombatRingOffset;                           // +0x368
    long                         CombatRingGraphic;                          // +0x370
    C2DVector                    CombatCurrentOffset;                        // +0x374
    C2DVector                    CombatCurrentTextOffset;                    // +0x37c
    vector<C2DVector>            CombatCountOffsets;                         // +0x384
    long                         CombatCountEmptyGraphic;                    // +0x390
    vector<long>                 CombatCountGraphics;                        // +0x394
    vector<long>                 CombatRingGraphics;                         // +0x3a0
    long                         CombatRingBlackGraphic;                     // +0x3ac
    long                         CombatRingWhiteGraphic;                     // +0x3b0
    long                         CombatCrossGraphic;                         // +0x3b4
    C2DVector                    CombatCrossOffset;                          // +0x3b8
    C2DVector                    CombatMiddleOffset;                         // +0x3c0
    float                        CombatMiddleMinWidth;                       // +0x3c8
    long                         CombatMiddleGraphic;                        // +0x3cc
    long                         CombatEndGraphic;                           // +0x3d0
    C2DVector                    ExperiencePos;                              // +0x3d4
    long                         ExperienceGraphic;                          // +0x3dc
    long                         ExperienceGeneralGraphic;                   // +0x3e0
    long                         ExperienceSkillGraphic;                     // +0x3e4
    long                         ExperienceStrengthGraphic;                  // +0x3e8
    long                         ExperienceWillGraphic;                      // +0x3ec
    C2DVector                    ExperienceTextOffset;                       // +0x3f0
    CRGBColour                   ExperienceTextColour;                       // +0x3f8
    C2DVector                    ExperienceMiddleOffset;                     // +0x3fc
    float                        ExperienceMiddleMinWidth;                   // +0x404
    long                         ExperienceMiddleGraphic;                    // +0x408
    long                         ExperienceEndGraphic;                       // +0x40c
    C2DVector                    CrimePos;                                   // +0x410
    C2DVector                    CrimeTextOffset;                            // +0x418
    CRGBColour                   CrimeTextColour;                            // +0x420
    CRGBColour                   CrimeTextAlertColour;                       // +0x424
    long                         CrimeWarningGraphic;                        // +0x428
    long                         CrimeCriminalGraphic;                       // +0x42c
    long                         CrimeBarGraphic;                            // +0x430
    C2DVector                    CrimeBarIconOffset;                         // +0x434
    C2DVector                    CrimeBarBorderOffset;                       // +0x43c
    vector<C2DVector>            CrosshairOffsets;                           // +0x444
    vector<long>                 CrosshairGraphics;                          // +0x450
    vector<long>                 CrosshairGreenGraphics;                     // +0x45c
    long                         MiniGameFishingLeftGraphic;                 // +0x468
    long                         MiniGameFishingRightGraphic;                // +0x46c
    long                         MiniGameBettingGraphic;                     // +0x470
    long                         MiniGameDiggingGraphic;                     // +0x474
    long                         MiniGamePickpocketGraphic;                  // +0x478
    long                         MiniGamePicklockGraphic;                    // +0x47c
    long                         MiniGameStealGraphic;                       // +0x480
    long                         MiniGameTrophyGotGraphic;                   // +0x484
    long                         MiniGameTrophyLeftGraphic;                  // +0x488
    long                         SmackEffect;                                // +0x48c
    long                         SmackAlphaFadePerFrame;                     // +0x490
    long                         SmackAlphaStartValue;                       // +0x494
    long                         SmackAlphaEndValue;                         // +0x498
    float                        SmackScaleFactorPerFrame;                   // +0x49c
    float                        SmackTimeToRotateSeconds;                   // +0x4a0
    C2DVector                    MoneyPos;                                   // +0x4a4
    C2DVector                    MoneyTextOffset;                            // +0x4ac
    C2DVector                    MoneyTextRightOffset;                       // +0x4b4
    float                        MoneyWidth;                                 // +0x4bc
    CRGBColour                   MoneyTextColour;                            // +0x4c0
    CRGBColour                   MoneyIncreaseTextColour;                    // +0x4c4
    CRGBColour                   MoneyDecreaseTextColour;                    // +0x4c8
    C2DVector                    MoneyChangeStartOffset;                     // +0x4cc
    C2DVector                    MoneyChangeEndOffset;                       // +0x4d4
    float                        MoneyChangeTextSpeed;                       // +0x4dc
    CCharString                  StatBarsFontName;                           // +0x4e0
    float                        SecondsForStatsBarsToAppear;                // +0x4e4
    float                        SecondsBeforeStatsBackBarMoves;             // +0x4e8
    float                        SecondsToShowStatsBars;                     // +0x4ec
    float                        SecondsForStatsBarsToDisappear;             // +0x4f0
    C2DVector                    StatUpdatePos;                              // +0x4f4
    C2DVector                    StatUpdateMoralityGoodPos;                  // +0x4fc
    C2DVector                    StatUpdateMoralityEvilPos;                  // +0x504
    C2DVector                    StatUpdateRenownPos;                        // +0x50c
    C2DVector                    StatUpdateTextOffset;                       // +0x514
    CRGBColour                   StatUpdateTextColour;                       // +0x51c
    float                        StatUpdateDisplayDuration;                  // +0x520
    float                        StatUpdateWaitingDuration;                  // +0x524
    C2DVector                    StatUpdateEffectOffset;                     // +0x528
    long                         StatUpdateMoralityGoodGraphic;              // +0x530
    long                         StatUpdateMoralityEvilGraphic;              // +0x534
    long                         StatUpdateMoralityGoodEffect;               // +0x538
    long                         StatUpdateMoralityEvilEffect;               // +0x53c
    vector<CRGBColour>           StatUpdateColours;                          // +0x540
    vector<long>                 StatUpdateGraphics;                         // +0x54c
    vector<long>                 StatUpdateEffects;                          // +0x558
    C2DVector                    AutoPickupPos;                              // +0x564
    C2DVector                    QuestInfoPos;                               // +0x56c
    float                        QuestInfoBarOffsetX;                        // +0x574
    long                         QuestInfoGraphicWidth;                      // +0x578
    long                         QuestInfoGraphicHeight;                     // +0x57c
    float                        QuestInfoGraphicMinHeight;                  // +0x580
    float                        QuestInfoSecondsToScroll;                   // +0x584
    float                        QuestInfoSecondsToAppear;                   // +0x588
    CRGBColour                   QuestInfoTurncoatColour;                    // +0x58c
    long                         QuestInfoTickGraphic;                       // +0x590
    C2DVector                    OracleSymbolPos;                            // +0x594
    C2DVector                    OracleSymbolOffset;                         // +0x59c
    C2DVector                    TextBoxTextAreaTLPos;                       // +0x5a4
    C2DVector                    TextBoxTextAreaBRPos;                       // +0x5ac
    CRGBColour                   TextBoxTextColour;                          // +0x5b4
    CRGBColour                   TextBoxTextGameInfoColour;                  // +0x5b8
    CRGBColour                   TextBoxTextGameActionColour;                // +0x5bc
    C2DVector                    TextBoxButtonGraphicOffset;                 // +0x5c0
    float                        TextBoxButtonOffsetY;                       // +0x5c8
    float                        TextBoxButtonInBoxOffsetY;                  // +0x5cc
    float                        TextBoxButtonGapWidth;                      // +0x5d0
    CCharString                  TextBoxButtonTextFontName;                  // +0x5d4
    long                         TextBoxConfirmGraphic;                      // +0x5d8
    long                         TextBoxCancelGraphic;                       // +0x5dc
    long                         TextBoxThirdGraphic;                        // +0x5e0
    long                         TextBoxSkipCutsceneGraphic;                 // +0x5e4
    CCharString                  TextBoxButtonASoundCriteria;                // +0x5e8
    CCharString                  TextBoxButtonBSoundCriteria;                // +0x5ec
    long                         TextBoxHugeButtonGraphic;                   // +0x5f0
    long                         TextBoxHugeMouseButtonGraphic;              // +0x5f4
    long                         TextBoxHugeMouseButtonLeftGraphic;          // +0x5f8
    C2DVector                    TextBoxHugeButtonOffset;                    // +0x5fc
    C2DVector                    TextBoxHugeButtonLeftOffset;                // +0x604
    C2DVector                    TextBoxFishingReelTextPos;                  // +0x60c
    C2DVector                    TextBoxFishingReelGraphicOffset;            // +0x614
    C2DVector                    TextBoxBettingGraphicPos;                   // +0x61c
    C2DVector                    TextBoxBettingUpOffset;                     // +0x624
    C2DVector                    TextBoxBettingDownOffset;                   // +0x62c
    C2DVector                    TextBoxBettingLeftOffset;                   // +0x634
    C2DVector                    TextBoxBettingRightOffset;                  // +0x63c
    CCharString                  ReceiveItemsSoundCriteria;                  // +0x644
    long                         ControllerAGraphic;                         // +0x648
    long                         ControllerBGraphic;                         // +0x64c
    long                         ControllerXGraphic;                         // +0x650
    long                         ControllerYGraphic;                         // +0x654
    long                         ControllerDPadGraphic;                      // +0x658
    long                         ControllerDPadDownGraphic;                  // +0x65c
    long                         ControllerDPadLeftGraphic;                  // +0x660
    long                         ControllerDPadRightGraphic;                 // +0x664
    long                         ControllerDPadUpGraphic;                    // +0x668
    long                         ControllerThumbLeftGraphic;                 // +0x66c
    long                         ControllerThumbRightGraphic;                // +0x670
    long                         ControllerThumbClickLeftGraphic;            // +0x674
    long                         ControllerThumbClickRightGraphic;           // +0x678
    long                         ControllerBackGraphic;                      // +0x67c
    long                         ControllerStartGraphic;                     // +0x680
    long                         ControllerTriggerLeftGraphic;               // +0x684
    long                         ControllerTriggerRightGraphic;              // +0x688
    long                         ControllerTriggerBlackGraphic;              // +0x68c
    long                         ControllerTriggerWhiteGraphic;              // +0x690
    C2DVector                    ControllerGraphicOffset;                    // +0x694
    C2DVector                    ControllerGraphicTextOffset;                // +0x69c
    C2DVector                    ControllerClickGraphicTextOffset;           // +0x6a4
    C2DVector                    ControllerTriggerGraphicTextOffset;         // +0x6ac
    C2DVector                    MenuConfirmPos;                             // +0x6b4
    C2DVector                    MenuConfirmTextPos;                         // +0x6bc
    long                         MenuConfirmGraphic;                         // +0x6c4
    C2DVector                    MenuCancelPos;                              // +0x6c8
    C2DVector                    MenuCancelTextPos;                          // +0x6d0
    long                         MenuCancelGraphic;                          // +0x6d8
    long                         QuickAccessDPadGraphic;                     // +0x6dc
    long                         QuickAccessMenuGraphic;                     // +0x6e0
    long                         QuickAccessLampEffect;                      // +0x6e4
    long                         QuickAccessGuildSealEffect;                 // +0x6e8
    long                         QuickAccessMainGuildSealEffect;             // +0x6ec
    C2DVector                    QuickAccessEffectOffset;                    // +0x6f0
    C2DVector                    QuickAccessAbilityCrossOffset;              // +0x6f8
    long                         HotBarSingleGraphic;                        // +0x700
    long                         HotBarMainGraphic;                          // +0x704
    vector<long>                 HotBarEndGraphics;                          // +0x708
    C2DVector                    HotBarOffset;                               // +0x714
    C2DVector                    HotBarEndOffset;                            // +0x71c
    long                         HotBarItemWidth;                            // +0x724
    vector<CQuickAccessItemButtonGuiDef> QuickAccessItemButtons;                     // +0x728
    vector<CQuickAccessItemButtonGuiDef> ContextSensitiveItemButtons;                // +0x734
    long                         TargetingCursorSectionEnemyEmptyGraphic;    // +0x740
    long                         CompassGraphic;                             // +0x744
    C2DVector                    CompassPos;                                 // +0x748
    long                         CompassShadowGraphic;                       // +0x750
    C2DVector                    CompassShadowOffset;                        // +0x754
    long                         AbilityButtonTalkGraphic;                   // +0x75c
    long                         AbilityButtonActivateGraphic;               // +0x760
    long                         AbilityButtonBlockGraphic;                  // +0x764
    long                         AbilityButtonUnarmedGraphic;                // +0x768
    long                         AbilityButtonMeleeGraphic;                  // +0x76c
    long                         AbilityButtonRangedGraphic;                 // +0x770
    long                         AbilityButtonSheatheAxeGraphic;             // +0x774
    long                         AbilityButtonSheatheBowGraphic;             // +0x778
    long                         AbilityButtonSheatheMaceGraphic;            // +0x77c
    long                         AbilityButtonSheatheStickGraphic;           // +0x780
    long                         AbilityButtonSheatheSwordGraphic;           // +0x784
    C2DVector                    AbilityButtonFlourishEffectPos;             // +0x788
    long                         AbilityButtonFlourishGraphic;               // +0x790
    C2DVector                    AbilityButtonFlourishGraphicOffset;         // +0x794
    long                         ABXYButtonMiddleGraphic;                    // +0x79c
    C2DVector                    ABXYAbilityLevelInitialOffset;              // +0x7a0
    C2DVector                    ABXYAbilityLevelOffset;                     // +0x7a8
    long                         ABXYAbilityUnavailableGraphic;              // +0x7b0
    long                         ABXYAbilityUncastableGraphic;               // +0x7b4
    long                         ABXYAbilitySwitchGraphic;                   // +0x7b8
    long                         ABXYAbilityShieldOffGraphic;                // +0x7bc
    long                         ABXYRunGraphic;                             // +0x7c0
    long                         ABXYTargetingEnterGraphic;                  // +0x7c4
    long                         ABXYTargetingCancelGraphic;                 // +0x7c8
    float                        ABXYButtonFadeDurationSeconds;              // +0x7cc
    long                         TargetingBorderLeftGraphic;                 // +0x7d0
    C2DVector                    TargetingBorderLeftPos;                     // +0x7d4
    long                         TargetingBorderRightGraphic;                // +0x7dc
    C2DVector                    TargetingBorderRightPos;                    // +0x7e0
    long                         InteractButtonBorderGraphic;                // +0x7e8
    C2DVector                    InteractButtonBorderOffset;                 // +0x7ec
    C2DVector                    InteractButtonTooltipOffset;                // +0x7f4
    vector<CAbilityButtonGuiDef> AbilityButtons;                             // +0x7fc
    vector<CAbilityButtonGuiDef> SpecialAbilityButtons;                      // +0x808
    C2DVector                    DigitPos;                                   // +0x814
    C2DVector                    DigitHeroPos;                               // +0x81c
    C2DVector                    DigitHeroOffset;                            // +0x824
    vector<long>                 DigitGraphics;                              // +0x82c
    C2DVector                    RegionDisplayPos;                           // +0x838
    C2DVector                    InfoDisplayPos;                             // +0x840
    long                         HeroSprintBarLeftGraphic;                   // +0x848
    long                         HeroSprintBarLeftGlowGraphic;               // +0x84c
    long                         HeroSprintBarMiddleGraphic;                 // +0x850
    long                         HeroSprintBarMiddleGlowGraphic;             // +0x854
    long                         HeroSprintBarRightGraphic;                  // +0x858
    long                         HeroSprintBarRightGlowGraphic;              // +0x85c
    long                         HeroSprintBarInnerGraphic;                  // +0x860
    C2DVector                    HeroSprintBarInnerGraphicOffset;            // +0x864
    vector<CRGBColour>           HighlightStartColour;                       // +0x86c
    vector<CRGBColour>           HighlightEndColour;                         // +0x878
    vector<float>                HighlightStartWidthWorldSpace;              // +0x884
    vector<float>                HighlightEndWidthWorldSpace;                // +0x890
    vector<float>                HighlightStartWidthScreenSpace;             // +0x89c
    vector<float>                HighlightEndWidthScreenSpace;               // +0x8a8
    vector<long>                 HighlightFadeSteps;                         // +0x8b4
    float                        NumberOfSecondsForContainerItemToReachFullSize;// +0x8c0
    C2DVector                    ContainerItemDisplayAreaTLPos;              // +0x8c4
    C2DVector                    ContainerItemDisplayAreaBRPos;              // +0x8cc
    float                        NumberOfSecondsForContainerItemToRotate;    // +0x8d4
    float                        NumberOfSecondsForContainerItemToStayAtFullSize;// +0x8d8
    CRGBColour                   ContainerItemDisplayBackgroundColour;       // +0x8dc
    C2DVector                    ScreenMessageTextPos;                       // +0x8e0
    CRGBColour                   ScreenMessageTextColour;                    // +0x8e8
    C2DVector                    ScreenMessageTextDropShadowOffset;          // +0x8ec
    CRGBColour                   ScreenMessageTextDropShadowColour;          // +0x8f4
    CCharString                  ScreenMessageFont;                          // +0x8f8
    C2DVector                    ScreenMessageNextMessageTextOffset;         // +0x8fc
    long                         MaximumNumberOfScreenMessages;              // +0x904
    float                        NumberOfSecondsScreenMessageStaysOnScreen;  // +0x908
    long                         NumberOfCharactersScreenMessageAddsPerFrame;// +0x90c
    vector<CCharString>          TutorialText;                               // +0x910
    vector<CCharString>          TutorialLogBookText;                        // +0x91c
    vector<long>                 TutorialGuildSealGraphics;                  // +0x928
    CScriptSpriteMap             ScriptSprites;                              // +0x934
    CGameActionMap               GameActionValues;                           // +0x940
    CScriptSpriteMap             MiniMapGraphics;                            // +0x94c
    vector<long>                 MiniMapMarkerGraphics;                      // +0x958
    C2DVector                    MiniMapScreenOrigin;                        // +0x964
    C2DVector                    MiniMapScreenSize;                          // +0x96c
    long                         MiniMapScreenRadius;                        // +0x974
    long                         MiniMapTextureWidth;                        // +0x978
    long                         MiniMapMarkerTextureWidth;                  // +0x97c
    float                        MiniMapWorldResolution;                     // +0x980
    CRGBColour                   MiniMapPassableColour;                      // +0x984
    CRGBColour                   MiniMapImpassableColour;                    // +0x988
    CRGBColour                   MiniMapWaterColour;                         // +0x98c
    CRGBColour                   MiniMapMaplessColour;                       // +0x990
    CRGBColour                   MiniMapImpassableWaterColour;               // +0x994
    unsigned long                MaxMinimapZoneColours;                      // +0x998
    unsigned long                MaxMinimapThemeColours;                     // +0x99c
    vector<CRGBColour>           MinimapZoneColours;                         // +0x9a0
    vector<CRGBColour>           MinimapThemeColours;                        // +0x9ac
    C2DVector                    RegionChangeTLOffset;                       // +0x9b8
    C2DVector                    RegionChangeTMOffset;                       // +0x9c0
    C2DVector                    RegionChangeTROffset;                       // +0x9c8
    C2DVector                    RegionChangeBLOffset;                       // +0x9d0
    C2DVector                    RegionChangeBMOffset;                       // +0x9d8
    C2DVector                    RegionChangeBROffset;                       // +0x9e0
    long                         RegionChangeTLGraphic;                      // +0x9e8
    long                         RegionChangeTMGraphic;                      // +0x9ec
    long                         RegionChangeTRGraphic;                      // +0x9f0
    long                         RegionChangeBLGraphic;                      // +0x9f4
    long                         RegionChangeBMGraphic;                      // +0x9f8
    long                         RegionChangeBRGraphic;                      // +0x9fc
    C2DVector                    RegionChangeBorderLeftOffset;               // +0xa00
    C2DVector                    RegionChangeBorderRightOffset;              // +0xa08
    long                         RegionChangeBorderLeftGraphic;              // +0xa10
    long                         RegionChangeBorderRightGraphic;             // +0xa14
    long                         RegionChangeGuildSealBorderGraphic;         // +0xa18
    C2DVector                    RegionChangeGuildSealBorderOffset;          // +0xa1c
    vector<long>                 RegionChangeGuildSealGraphics;              // +0xa24
    long                         LoadProgressBarLeftGraphic;                 // +0xa30
    long                         LoadProgressBarMidGraphic;                  // +0xa34
    long                         LoadProgressBarRightGraphic;                // +0xa38
    long                         LoadProgressBarInnerGraphic;                // +0xa3c
    vector<long>                 LoadProgressBackdropGraphics;               // +0xa40
    C2DVector                    TavernGameBlockTLOffset;                    // +0xa4c
    C2DVector                    TavernGameBlockTMOffset;                    // +0xa54
    C2DVector                    TavernGameBlockTROffset;                    // +0xa5c
    C2DVector                    TavernGameBlockBLOffset;                    // +0xa64
    C2DVector                    TavernGameBlockBMOffset;                    // +0xa6c
    C2DVector                    TavernGameBlockBROffset;                    // +0xa74
    long                         TavernGameBlockTLGraphic;                   // +0xa7c
    long                         TavernGameBlockTMGraphic;                   // +0xa80
    long                         TavernGameBlockTRGraphic;                   // +0xa84
    long                         TavernGameBlockBLGraphic;                   // +0xa88
    long                         TavernGameBlockBMGraphic;                   // +0xa8c
    long                         TavernGameBlockBRGraphic;                   // +0xa90
    CCharString                  RegionGraphFileName;                        // +0xa94
    float                        MouseCursorScaleX;                          // +0xa98
    float                        MouseCursorScaleY;                          // +0xa9c
    long                         MouseCursorGraphic;                         // +0xaa0
    long                         MouseLMBGraphic;                            // +0xaa4
    long                         MouseMMBGraphic;                            // +0xaa8
    long                         MouseRMBGraphic;                            // +0xaac
    long                         MouseOverlayGraphic;                        // +0xab0
    virtual void v00();
    CPlayerGuiDef();
};

CPlayerGuiDef::CPlayerGuiDef()
{
}
