// CGlobalDef::operator=(const CGlobalDef&) -- memberwise copy of the def: CParentDefClassBase part, then every CGlobalDef member in declaration order; returns *this
// Leaf: ??4CGlobalDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465EB3.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, CCharString::operator= @ 0x0099EFB0, CConversationCameraMap::operator= @ 0x00466152, CStringVector::operator= @ 0x00432EE9, CDialogueLayerVector::operator= @ 0x00466281.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CGlobalDef's
// vtable 0x01234C8C is the 5-byte `jmp` at 0x00465EAE right before this body (rtti-vtables.tsv).
// Layout: PDB CGlobalDef members from +0x40 sit at retail +0x3c (retail CParentDefClassBase 0x3c bytes; retail
// STLport vectors/maps are 12 bytes where the PDB has 16). Names and order are the PDB's.
// Shape: compiler-generated memberwise assignment written out; scalars and small structs are copied inline,
// strings/containers through their out-of-line operator=.
#pragma optimize("s", on)

#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x38];      // +0x04 CDefClassBase part, InstantiationName, SubDefInfoMap
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CCharString {                    // 4-byte string handle
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

#pragma pack(push, 2)
struct CEngineGraphic {                 // 14 bytes, copied as 3 dwords + a word
    unsigned long BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
};
#pragma pack(pop)

struct CConversationCameraMap {         // retail map<long,CCharString> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CConversationCameraMap& operator=(const CConversationCameraMap& other);   // 0x00466152
};

struct CStringVector {                  // retail std::vector<CCharString> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CStringVector& operator=(const CStringVector& other);   // 0x00432EE9
};

struct CDialogueLayerVector {           // retail std::vector<NSpeechGainManager::CDialogueLayerDef> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CDialogueLayerVector& operator=(const CDialogueLayerVector& other);   // 0x00466281
};

struct CGlobalDef : CParentDefClassBase {// vtable 0x01234C8C
    long                   ControlSchemePC;                                     // +0x3c
    long                   ControlSchemePCAggressive;                           // +0x40
    long                   ControlSchemeXBOX;                                   // +0x44
    long                   ControlSchemeXBOXAggressive;                         // +0x48
    long                   PhysicsTexture;                                      // +0x4c
    long                   InvalidThemeStandin;                                 // +0x50
    long                   DefaultEngineThemeDef;                               // +0x54
    float                  StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape; // +0x58
    long                   GraphicWeaponTrail;                                  // +0x5c
    long                   GraphicWeaponTrail2;                                 // +0x60
    long                   GraphicWeaponTrailArrow;                             // +0x64
    long                   WeaponTrailAlpha;                                    // +0x68
    float                  WeaponTrailAlphaFadeOffTime;                         // +0x6c
    long                   WeaponTrailArrowAlpha;                               // +0x70
    float                  WeaponTrailArrowAlphaFadeOffTime;                    // +0x74
    long                   WeaponTrialNumFramesToFadeOut;                       // +0x78
    long                   HealthPickupValue;                                   // +0x7c
    long                   RespawnDelay;                                        // +0x80
    long                   ScrabbleAngle;                                       // +0x84
    long                   SlideAngle;                                          // +0x88
    long                   MaxFollowers;                                        // +0x8c
    CCharString            PickupGoldSoundCriteria;                             // +0x90
    CCharString            PickupItemSoundCriteria;                             // +0x94
    CCharString            OnClickSoundCriteria;                                // +0x98
    long                   EditorSpangleCursor;                                 // +0x9c
    long                   EditorCircleSprite;                                  // +0xa0
    long                   EditorSecondaryCircleSprite;                         // +0xa4
    long                   TestSpeech;                                          // +0xa8
    CCharString            MimeSpeechIndex;                                     // +0xac
    long                   AdultSleepTimeStart;                                 // +0xb0
    long                   AdultSleepTimeEnd;                                   // +0xb4
    long                   ChildSleepTimeStart;                                 // +0xb8
    long                   ChildSleepTimeEnd;                                   // +0xbc
    long                   ElderlySleepTimeStart;                               // +0xc0
    long                   ElderlySleepTimeEnd;                                 // +0xc4
    float                  CreatureGeneratorRegenDelaySeconds;                  // +0xc8
    long                   Music;                                               // +0xcc
    float                  AgeIncreasePerLevelUp;                               // +0xd0
    long                   E3BodgeFireflyGraphic;                               // +0xd4
    CEngineGraphic         SwitchGraphic;                                       // +0xd8
    CEngineGraphic         TrackGraphic;                                        // +0xe6
    CConversationCameraMap ConversationCameras;                                 // +0xf4
    CStringVector          PermittedAINarrators;                                // +0x100
    CDialogueLayerVector   DialogueLayers;                                      // +0x10c
    long                   OpinionDebugIndicatorIcon;                           // +0x118
    long                   WifeLoveIndicatorIcon;                               // +0x11c
    long                   WifeEngagedIndicatorIcon;                            // +0x120
    long                   WifeMarriedIndicatorIcon;                            // +0x124
    long                   ExpressionIndicatorIcon;                             // +0x128
    long                   FollowerIndicatorIcon;                               // +0x12c
    float                  EmoteIconDrawDistance;                               // +0x130
    float                  AngleToBeConsideredVertical;                         // +0x134
    float                  SoundGainLowerValueWhenInsideOutside;                // +0x138
    long                   TeleportOutParticleEffect;                           // +0x13c
    float                  TeleportFadeOutTimeInSeconds;                        // +0x140
    long                   TeleportInParticleEffect;                            // +0x144
    float                  TeleportFadeInTimeInSeconds;                         // +0x148
    float                  DamageForMaximumBlood;                               // +0x14c
    CGlobalDef& operator=(const CGlobalDef& other);
};

CGlobalDef& CGlobalDef::operator=(const CGlobalDef& other)
{
    CParentDefClassBase::operator=(other);
    ControlSchemePC = other.ControlSchemePC;
    ControlSchemePCAggressive = other.ControlSchemePCAggressive;
    ControlSchemeXBOX = other.ControlSchemeXBOX;
    ControlSchemeXBOXAggressive = other.ControlSchemeXBOXAggressive;
    PhysicsTexture = other.PhysicsTexture;
    InvalidThemeStandin = other.InvalidThemeStandin;
    DefaultEngineThemeDef = other.DefaultEngineThemeDef;
    StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape = other.StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape;
    GraphicWeaponTrail = other.GraphicWeaponTrail;
    GraphicWeaponTrail2 = other.GraphicWeaponTrail2;
    GraphicWeaponTrailArrow = other.GraphicWeaponTrailArrow;
    WeaponTrailAlpha = other.WeaponTrailAlpha;
    WeaponTrailAlphaFadeOffTime = other.WeaponTrailAlphaFadeOffTime;
    WeaponTrailArrowAlpha = other.WeaponTrailArrowAlpha;
    WeaponTrailArrowAlphaFadeOffTime = other.WeaponTrailArrowAlphaFadeOffTime;
    WeaponTrialNumFramesToFadeOut = other.WeaponTrialNumFramesToFadeOut;
    HealthPickupValue = other.HealthPickupValue;
    RespawnDelay = other.RespawnDelay;
    ScrabbleAngle = other.ScrabbleAngle;
    SlideAngle = other.SlideAngle;
    MaxFollowers = other.MaxFollowers;
    PickupGoldSoundCriteria = other.PickupGoldSoundCriteria;
    PickupItemSoundCriteria = other.PickupItemSoundCriteria;
    OnClickSoundCriteria = other.OnClickSoundCriteria;
    EditorSpangleCursor = other.EditorSpangleCursor;
    EditorCircleSprite = other.EditorCircleSprite;
    EditorSecondaryCircleSprite = other.EditorSecondaryCircleSprite;
    TestSpeech = other.TestSpeech;
    MimeSpeechIndex = other.MimeSpeechIndex;
    AdultSleepTimeStart = other.AdultSleepTimeStart;
    AdultSleepTimeEnd = other.AdultSleepTimeEnd;
    ChildSleepTimeStart = other.ChildSleepTimeStart;
    ChildSleepTimeEnd = other.ChildSleepTimeEnd;
    ElderlySleepTimeStart = other.ElderlySleepTimeStart;
    ElderlySleepTimeEnd = other.ElderlySleepTimeEnd;
    CreatureGeneratorRegenDelaySeconds = other.CreatureGeneratorRegenDelaySeconds;
    Music = other.Music;
    AgeIncreasePerLevelUp = other.AgeIncreasePerLevelUp;
    E3BodgeFireflyGraphic = other.E3BodgeFireflyGraphic;
    SwitchGraphic = other.SwitchGraphic;
    TrackGraphic = other.TrackGraphic;
    ConversationCameras = other.ConversationCameras;
    PermittedAINarrators = other.PermittedAINarrators;
    DialogueLayers = other.DialogueLayers;
    OpinionDebugIndicatorIcon = other.OpinionDebugIndicatorIcon;
    WifeLoveIndicatorIcon = other.WifeLoveIndicatorIcon;
    WifeEngagedIndicatorIcon = other.WifeEngagedIndicatorIcon;
    WifeMarriedIndicatorIcon = other.WifeMarriedIndicatorIcon;
    ExpressionIndicatorIcon = other.ExpressionIndicatorIcon;
    FollowerIndicatorIcon = other.FollowerIndicatorIcon;
    EmoteIconDrawDistance = other.EmoteIconDrawDistance;
    AngleToBeConsideredVertical = other.AngleToBeConsideredVertical;
    SoundGainLowerValueWhenInsideOutside = other.SoundGainLowerValueWhenInsideOutside;
    TeleportOutParticleEffect = other.TeleportOutParticleEffect;
    TeleportFadeOutTimeInSeconds = other.TeleportFadeOutTimeInSeconds;
    TeleportInParticleEffect = other.TeleportInParticleEffect;
    TeleportFadeInTimeInSeconds = other.TeleportFadeInTimeInSeconds;
    DamageForMaximumBlood = other.DamageForMaximumBlood;
    return *this;
}