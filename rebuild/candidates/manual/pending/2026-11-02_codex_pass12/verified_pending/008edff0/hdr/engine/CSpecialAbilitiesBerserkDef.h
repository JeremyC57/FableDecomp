#ifdef FABLE_RETAIL_WILL_BUTTON_TIMER
#ifndef FABLE_ENGINE_CSPECIALABILITIESBERSERKDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESBERSERKDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
// Original complete 0x1c132 in Ego_r; native timing loads confirm retail vector reductions.
class CSpecialAbilitiesBerserkDef : public CParentDefClassBase {
public:
    float SpeedMultiplier;
    long FramesToFadeOut;
    float LowerBerserkPercent;
    float TimeForThemeTransition;
    float BlurCenterFromFloor;
    float BerserkThreshold;
    float BerserkLength;
    float BerserkIntensity;
    float BerserkFade;
    float BerserkRange;
    float BerserkTwist;
    float TimeForRadialBlurToFadeOut;
    float CameraStartDist;
    float CameraEndDist;
    float CameraStartDeltaHeight;
    float CameraEndDeltaHeight;
    std::vector<float,std::allocator<float> > BerserkDuration;
    std::vector<float,std::allocator<float> > BerserkDamageToEnemyMultiplier;
    float BerserkDamageToEnemyMultiplierAdditionPerHit;
    std::vector<float,std::allocator<float> > BerserkDamageToMeMultiplier;
    std::vector<float,std::allocator<float> > MaxIncreaseInStrength;
    std::vector<float,std::allocator<float> > BerserkScale;
    float BerserkBlastRadius;
    std::vector<float,std::allocator<float> > CamDist;
    std::vector<float,std::allocator<float> > CamHeight;
    std::vector<float,std::allocator<float> > CamFOV;
    std::vector<float,std::allocator<float> > CamSwitchTime;
    std::vector<float,std::allocator<float> > RumbleLevel;
    std::vector<float,std::allocator<float> > RumbleSmoothness;
    std::vector<float,std::allocator<float> > RumbleAttack;
    std::vector<float,std::allocator<float> > RumbleDecay;
    float RumbleReleaseLevelDelta;
    float RumbleReleaseSmoothnessDelta;
    float RumbleReleaseAttackDelta;
    float RumbleReleaseDecayDelta;
    float CrazyActionRadius;
    float CrazyActionDamage;
    std::vector<float,std::allocator<float> > DecapitateMaxProb;
    float TargetNearestEnemyDistance;
    long WeaponTrailAttack;
    long WeaponTrailKnockDown;
    std::vector<float,std::allocator<float> > LevelUpTimings;
    std::vector<long,std::allocator<long> > TintColorR;
    std::vector<long,std::allocator<long> > TintColorG;
    std::vector<long,std::allocator<long> > TintColorB;
    bool CanBlockWhileBerserked;
    float CrazyIdleIntervalSecs;
};
#endif

#else

#endif
