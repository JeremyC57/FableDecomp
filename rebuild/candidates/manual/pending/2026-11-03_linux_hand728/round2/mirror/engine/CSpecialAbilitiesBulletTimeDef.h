#ifndef FABLE_ENGINE_CSPECIALABILITIESBULLETTIMEDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESBULLETTIMEDEF_H
// Retail class (Codex pass 12 row 0x008EDFF0, header wave 2026-11-03): RTTI base CParentDefClassBase (packed, 0x39), members in
// Ego_r.pdb order and spelling (ghidra_out/struct_layouts_egor.tsv, all 42 checked) with 12-byte STLport vectors; the
// CWillButtonHelperTimer ctor 0x008EDFF0 reads LevelUpTimings through CDefinitionManager::GetDef (graded RELOCATION_MATCH).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
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
