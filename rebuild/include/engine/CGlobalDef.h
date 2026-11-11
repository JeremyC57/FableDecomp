#ifndef FABLE_ENGINE_CGLOBALDEF_H
#define FABLE_ENGINE_CGLOBALDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/CEngineGraphic.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
namespace NSpeechGainManager{class CDialogueLayerDef;}
// Layout: complete original Ego148a7; native container sizes yield336 bytes.
class CGlobalDef:public CParentDefClassBase {
public: virtual void Copy(const CDefClassBase * p0);
public: virtual unsigned long GetSizeofClass();
public: static CDefClassBase * FABLE_FASTCALL Alloc();
public: static CCharString FABLE_FASTCALL StatGetTypeName();
public: long ControlSchemePC;
public: long ControlSchemePCAggressive;
public: long ControlSchemeXBOX;
public: long ControlSchemeXBOXAggressive;
public: long PhysicsTexture;
public: long InvalidThemeStandin;
public: long DefaultEngineThemeDef;
public: float StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape;
public: long GraphicWeaponTrail;
public: long GraphicWeaponTrail2;
public: long GraphicWeaponTrailArrow;
public: long WeaponTrailAlpha;
public: float WeaponTrailAlphaFadeOffTime;
public: long WeaponTrailArrowAlpha;
public: float WeaponTrailArrowAlphaFadeOffTime;
public: long WeaponTrialNumFramesToFadeOut;
public: long HealthPickupValue;
public: long RespawnDelay;
public: long ScrabbleAngle;
public: long SlideAngle;
public: long MaxFollowers;
public: CCharString PickupGoldSoundCriteria;
public: CCharString PickupItemSoundCriteria;
public: CCharString OnClickSoundCriteria;
public: long EditorSpangleCursor;
public: long EditorCircleSprite;
public: long EditorSecondaryCircleSprite;
public: long TestSpeech;
public: CCharString MimeSpeechIndex;
public: long AdultSleepTimeStart;
public: long AdultSleepTimeEnd;
public: long ChildSleepTimeStart;
public: long ChildSleepTimeEnd;
public: long ElderlySleepTimeStart;
public: long ElderlySleepTimeEnd;
public: float CreatureGeneratorRegenDelaySeconds;
public: long Music;
public: float AgeIncreasePerLevelUp;
public: long E3BodgeFireflyGraphic;
public: CEngineGraphic SwitchGraphic;
public: CEngineGraphic TrackGraphic;
public: std::map<long,CCharString,std::less<long>,std::allocator<std::pair<long const ,CCharString> > > ConversationCameras;
public: std::vector<CCharString,std::allocator<CCharString> > PermittedAINarrators;
public: std::vector<NSpeechGainManager::CDialogueLayerDef,std::allocator<NSpeechGainManager::CDialogueLayerDef> > DialogueLayers;
public: long OpinionDebugIndicatorIcon;
public: long WifeLoveIndicatorIcon;
public: long WifeEngagedIndicatorIcon;
public: long WifeMarriedIndicatorIcon;
public: long ExpressionIndicatorIcon;
public: long FollowerIndicatorIcon;
public: float EmoteIconDrawDistance;
public: float AngleToBeConsideredVertical;
public: float SoundGainLowerValueWhenInsideOutside;
public: long TeleportOutParticleEffect;
public: float TeleportFadeOutTimeInSeconds;
public: long TeleportInParticleEffect;
public: float TeleportFadeInTimeInSeconds;
public: float DamageForMaximumBlood;
public: virtual void Transfer(CPersistContext & p0);
public: CGlobalDef(const CGlobalDef & p0);
public: CGlobalDef();
public: virtual ~CGlobalDef();
public: CGlobalDef & operator=(const CGlobalDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CGlobalDef)==336);
FABLE_STATIC_ASSERT(offsetof(CGlobalDef,EmoteIconDrawDistance)==0x130);
#endif
