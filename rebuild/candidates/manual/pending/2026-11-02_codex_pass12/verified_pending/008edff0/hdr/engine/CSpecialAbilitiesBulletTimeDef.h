#ifdef FABLE_RETAIL_WILL_BUTTON_TIMER
#ifndef FABLE_ENGINE_CSPECIALABILITIESBULLETTIMEDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESBULLETTIMEDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
// Original complete 0x1c145 in Ego_r; native timing loads confirm retail vector reductions.
class CSpecialAbilitiesBulletTimeDef : public CParentDefClassBase {
public:
    float SlowTimeDuration[4];
    long BulletTimeSlowdownMultiplier[4];
    float LevelUpTimings[3];
    float CamDist[4];
    float CamHeight[4];
    float CamFOV[4];
    float CamSwitchTime[4];
    float CastCamDist;
    float CastCamHeight;
    float CastCamFOV;
    float CastCamSwitchTime;
    CDefString ChargeSound;
    CDefString PowerUp1Sound;
    CDefString PowerUp2Sound;
    CDefString PowerUp3Sound;
    CDefString InitSound;
    CDefString LoopSound;
    CDefString EndSound;
    C3DVector RadialBlurThresholdCol;
    float RadialBlurLength;
    C3DVector RadialBlurIntensityCol;
    float RadialBlurFade;
    float RadialBlurRange;
    float RadialBlurTwist;
    float RadialBlurFadeTimeSecs;
    C3DVector DamageRadialBlurThresholdCol;
    float DamageRadialBlurLength;
    C3DVector DamageRadialBlurIntensityCol;
    float DamageRadialBlurFade;
    float DamageRadialBlurRange;
    float DamageRadialBlurTwist;
    float DamageRadialBlurFadeTimeSecs;
    float HeroCombatSlowness;
    std::vector<float,std::allocator<float> > ScreenFilterAlpha;
    std::vector<float,std::allocator<float> > ScreenFilterSaturation;
    std::vector<float,std::allocator<float> > ScreenFilterContrast;
    std::vector<float,std::allocator<float> > ScreenFilterBrightness;
    std::vector<float,std::allocator<float> > ScreenFilterTintR;
    std::vector<float,std::allocator<float> > ScreenFilterTintG;
    std::vector<float,std::allocator<float> > ScreenFilterTintB;
    float ScreenFilterFadeInSecs;
    float ScreenFilterFadeOutSecs;
};
#endif

#else

#endif
