// Native virtual destructor, identified by deleting-dtor slot0 RTTI.
// Actual quarantined donor retained. Native packed parent39 vs donor3d;
// native vectors12 vs donor16. novtable views emit no production vtable.
#pragma optimize("s",on)
#define CSpecialAbilitiesBerserkDef CSpecialAbilitiesBerserkDefDonor
#include "engine/_quarantine/CSpecialAbilitiesBerserkDef.h"
#undef CSpecialAbilitiesBerserkDef
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#include <stdlib.h>
#ifdef _MSC_VER
typedef long S32;
#else
typedef int S32;
#define __declspec(x)
#endif
#pragma pack(push,1)
struct __declspec(novtable) CParentDefClassBase {
 virtual ~CParentDefClassBase(); //430300, genuine native slot0
 unsigned char DefClassBase[0x21];S32 InstantiationName_TablePos;
 unsigned char SubDefInfoMap[0x10];
};
#pragma pack(pop)
template<class T> struct NativeVector {T* Start;T* Finish;T* End;~NativeVector(){if(Start)free(Start);}};
struct __declspec(novtable) CSpecialAbilitiesBerserkDef:CParentDefClassBase {
    float         SpeedMultiplier;
    S32          FramesToFadeOut;
    float         LowerBerserkPercent;
    float         TimeForThemeTransition;
    float         BlurCenterFromFloor;
    float         BerserkThreshold;
    float         BerserkLength;
    float         BerserkIntensity;
    float         BerserkFade;
    float         BerserkRange;
    float         BerserkTwist;
    float         TimeForRadialBlurToFadeOut;
    float         CameraStartDist;
    float         CameraEndDist;
    float         CameraStartDeltaHeight;
    float         CameraEndDeltaHeight;
 NativeVector<float> BerserkDuration;
 NativeVector<float> BerserkDamageToEnemyMultiplier;
    float         BerserkDamageToEnemyMultiplierAdditionPerHit;
 NativeVector<float> BerserkDamageToMeMultiplier;
 NativeVector<float> MaxIncreaseInStrength;
 NativeVector<float> BerserkScale;
    float         BerserkBlastRadius;
 NativeVector<float> CamDist;
 NativeVector<float> CamHeight;
 NativeVector<float> CamFOV;
 NativeVector<float> CamSwitchTime;
 NativeVector<float> RumbleLevel;
 NativeVector<float> RumbleSmoothness;
 NativeVector<float> RumbleAttack;
 NativeVector<float> RumbleDecay;
    float         RumbleReleaseLevelDelta;
    float         RumbleReleaseSmoothnessDelta;
    float         RumbleReleaseAttackDelta;
    float         RumbleReleaseDecayDelta;
    float         CrazyActionRadius;
    float         CrazyActionDamage;
 NativeVector<float> DecapitateMaxProb;
    float         TargetNearestEnemyDistance;
    S32          WeaponTrailAttack;
    S32          WeaponTrailKnockDown;
 NativeVector<float> LevelUpTimings;
 NativeVector<S32> TintColorR;
 NativeVector<S32> TintColorG;
 NativeVector<S32> TintColorB;
    bool          CanBlockWhileBerserked;
    float         CrazyIdleIntervalSecs;
 virtual ~CSpecialAbilitiesBerserkDef();
};
CSpecialAbilitiesBerserkDef::~CSpecialAbilitiesBerserkDef() {}
