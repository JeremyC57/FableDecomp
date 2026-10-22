// CGlobalDef::Transfer(CPersistContext& context) -- persist the global definition: control schemes, weapon trails, sleep times, sound criteria, switch/track graphics, conversation cameras, narrators, dialogue layers, indicator icons and teleport effects
// Leaf: ?Transfer@CGlobalDef
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00472506, length 774 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers of the def module; the catalog's "UpdateShadowScene"/"FrameUpdate" labels are
//   propagated): long 0x00431102, float 0x00431061, CCharString 0x00431143, CEngineGraphic 0x00451E11,
//   map<long,CCharString> 0x0047280C, vector<CCharString> 0x004331F9,
//   TransferVectorOfSubComponents<NSpeechGainManager::CDialogueLayerDef> 0x00472A8D.
// Layout: Ego_r.pdb CGlobalDef (0x160) minus 4 (retail CDefClassBase is 0x3c bytes) up to TrackGraphic (+0xe6); the
//   STLport map and the two vectors are 12 bytes in retail (16 in the PDB), so everything after them sits a further
//   4 bytes lower per container: ConversationCameras +0xf4, PermittedAINarrators +0x100, DialogueLayers +0x10c,
//   OpinionDebugIndicatorIcon +0x118 .. DamageForMaximumBlood +0x14c. No generated engine header for the class.
// Pragma: #pragma optimize("s", on) -- a size TU: every member address goes through eax.
// Shape: one transfer per member in declaration order; the def base transfer is not called.
struct CCharString { void* m_pData; };
struct CEngineGraphic { unsigned char _body[0xe]; };
struct CConversationCameraMap { void* _M_header[3]; };   // STLport map<long, CCharString>, 12 bytes
struct CCharStringVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct CDialogueLayerDefVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };

struct CPersistContext {
    void Transfer(long& value);                         // 0x00431102
    void Transfer(float& value);                        // 0x00431061
    void Transfer(CCharString& value);                  // 0x00431143
    void Transfer(CEngineGraphic& value);               // 0x00451E11
    void Transfer(CConversationCameraMap& value);       // 0x0047280C
    void Transfer(CCharStringVector& value);            // 0x004331F9
    void Transfer(CDialogueLayerDefVector& value);      // 0x00472A8D
};

struct CDefClassBase {
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x3c - 4];
};

struct CGlobalDef : CDefClassBase {
    long  ControlSchemePC;                                        // +0x3c
    long  ControlSchemePCAggressive;                              // +0x40
    long  ControlSchemeXBOX;                                      // +0x44
    long  ControlSchemeXBOXAggressive;                            // +0x48
    long  PhysicsTexture;                                         // +0x4c
    long  InvalidThemeStandin;                                    // +0x50
    long  DefaultEngineThemeDef;                                  // +0x54
    float StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape; // +0x58
    long  GraphicWeaponTrail;                                     // +0x5c
    long  GraphicWeaponTrail2;                                    // +0x60
    long  GraphicWeaponTrailArrow;                                // +0x64
    long  WeaponTrailAlpha;                                       // +0x68
    float WeaponTrailAlphaFadeOffTime;                            // +0x6c
    long  WeaponTrailArrowAlpha;                                  // +0x70
    float WeaponTrailArrowAlphaFadeOffTime;                       // +0x74
    long  WeaponTrialNumFramesToFadeOut;                          // +0x78
    long  HealthPickupValue;                                      // +0x7c
    long  RespawnDelay;                                           // +0x80
    long  ScrabbleAngle;                                          // +0x84
    long  SlideAngle;                                             // +0x88
    long  MaxFollowers;                                           // +0x8c
    CCharString PickupGoldSoundCriteria;                          // +0x90
    CCharString PickupItemSoundCriteria;                          // +0x94
    CCharString OnClickSoundCriteria;                             // +0x98
    long  EditorSpangleCursor;                                    // +0x9c
    long  EditorCircleSprite;                                     // +0xa0
    long  EditorSecondaryCircleSprite;                            // +0xa4
    long  TestSpeech;                                             // +0xa8
    CCharString MimeSpeechIndex;                                  // +0xac
    long  AdultSleepTimeStart;                                    // +0xb0
    long  AdultSleepTimeEnd;                                      // +0xb4
    long  ChildSleepTimeStart;                                    // +0xb8
    long  ChildSleepTimeEnd;                                      // +0xbc
    long  ElderlySleepTimeStart;                                  // +0xc0
    long  ElderlySleepTimeEnd;                                    // +0xc4
    float CreatureGeneratorRegenDelaySeconds;                     // +0xc8
    long  Music;                                                  // +0xcc
    float AgeIncreasePerLevelUp;                                  // +0xd0
    long  E3BodgeFireflyGraphic;                                  // +0xd4
    CEngineGraphic SwitchGraphic;                                 // +0xd8
    CEngineGraphic TrackGraphic;                                  // +0xe6
    CConversationCameraMap ConversationCameras;                   // +0xf4
    CCharStringVector PermittedAINarrators;                       // +0x100
    CDialogueLayerDefVector DialogueLayers;                       // +0x10c
    long  OpinionDebugIndicatorIcon;                              // +0x118
    long  WifeLoveIndicatorIcon;                                  // +0x11c
    long  WifeEngagedIndicatorIcon;                               // +0x120
    long  WifeMarriedIndicatorIcon;                               // +0x124
    long  ExpressionIndicatorIcon;                                // +0x128
    long  FollowerIndicatorIcon;                                  // +0x12c
    float EmoteIconDrawDistance;                                  // +0x130
    float AngleToBeConsideredVertical;                            // +0x134
    float SoundGainLowerValueWhenInsideOutside;                   // +0x138
    long  TeleportOutParticleEffect;                              // +0x13c
    float TeleportFadeOutTimeInSeconds;                           // +0x140
    long  TeleportInParticleEffect;                               // +0x144
    float TeleportFadeInTimeInSeconds;                            // +0x148
    float DamageForMaximumBlood;                                  // +0x14c
    virtual void Transfer(CPersistContext& context);
};

void CGlobalDef::Transfer(CPersistContext& context)
{
    context.Transfer(ControlSchemePC);
    context.Transfer(ControlSchemePCAggressive);
    context.Transfer(ControlSchemeXBOX);
    context.Transfer(ControlSchemeXBOXAggressive);
    context.Transfer(PhysicsTexture);
    context.Transfer(InvalidThemeStandin);
    context.Transfer(DefaultEngineThemeDef);
    context.Transfer(StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape);
    context.Transfer(GraphicWeaponTrail);
    context.Transfer(GraphicWeaponTrail2);
    context.Transfer(GraphicWeaponTrailArrow);
    context.Transfer(WeaponTrailAlpha);
    context.Transfer(WeaponTrailAlphaFadeOffTime);
    context.Transfer(WeaponTrailArrowAlpha);
    context.Transfer(WeaponTrailArrowAlphaFadeOffTime);
    context.Transfer(WeaponTrialNumFramesToFadeOut);
    context.Transfer(HealthPickupValue);
    context.Transfer(RespawnDelay);
    context.Transfer(ScrabbleAngle);
    context.Transfer(SlideAngle);
    context.Transfer(MaxFollowers);
    context.Transfer(PickupGoldSoundCriteria);
    context.Transfer(PickupItemSoundCriteria);
    context.Transfer(OnClickSoundCriteria);
    context.Transfer(EditorSpangleCursor);
    context.Transfer(EditorCircleSprite);
    context.Transfer(EditorSecondaryCircleSprite);
    context.Transfer(TestSpeech);
    context.Transfer(MimeSpeechIndex);
    context.Transfer(AdultSleepTimeStart);
    context.Transfer(AdultSleepTimeEnd);
    context.Transfer(ChildSleepTimeStart);
    context.Transfer(ChildSleepTimeEnd);
    context.Transfer(ElderlySleepTimeStart);
    context.Transfer(ElderlySleepTimeEnd);
    context.Transfer(CreatureGeneratorRegenDelaySeconds);
    context.Transfer(Music);
    context.Transfer(AgeIncreasePerLevelUp);
    context.Transfer(E3BodgeFireflyGraphic);
    context.Transfer(SwitchGraphic);
    context.Transfer(TrackGraphic);
    context.Transfer(ConversationCameras);
    context.Transfer(PermittedAINarrators);
    context.Transfer(DialogueLayers);
    context.Transfer(OpinionDebugIndicatorIcon);
    context.Transfer(WifeLoveIndicatorIcon);
    context.Transfer(WifeEngagedIndicatorIcon);
    context.Transfer(WifeMarriedIndicatorIcon);
    context.Transfer(ExpressionIndicatorIcon);
    context.Transfer(FollowerIndicatorIcon);
    context.Transfer(EmoteIconDrawDistance);
    context.Transfer(AngleToBeConsideredVertical);
    context.Transfer(SoundGainLowerValueWhenInsideOutside);
    context.Transfer(TeleportOutParticleEffect);
    context.Transfer(TeleportFadeOutTimeInSeconds);
    context.Transfer(TeleportInParticleEffect);
    context.Transfer(TeleportFadeInTimeInSeconds);
    context.Transfer(DamageForMaximumBlood);
}