#ifndef FABLE_ENGINE_CSPECIALABILITIESBERSERKDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESBERSERKDEF_H
// Retail class (Codex pass 12 row 0x008EDFF0, header wave 2026-11-03): RTTI base CParentDefClassBase (packed, 0x39), members in
// Ego_r.pdb order and spelling (ghidra_out/struct_layouts_egor.tsv, all 47 checked) with 12-byte STLport vectors; the
// CWillButtonHelperTimer ctor 0x008EDFF0 reads LevelUpTimings through CDefinitionManager::GetDef (graded RELOCATION_MATCH).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
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
