// CSpecialAbilitiesBerserkDef::operator=(const CSpecialAbilitiesBerserkDef&) -- memberwise copy of the def: CParentDefClassBase part, then every CSpecialAbilitiesBerserkDef member in declaration order; returns *this
// Leaf: ??4CSpecialAbilitiesBerserkDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045F854.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, std::vector<float>::operator= @ 0x0045EF2D, std::vector<long>::operator= @ 0x00454886.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CSpecialAbilitiesBerserkDef's
// vtable 0x012341FC is the 5-byte `jmp` at 0x0045F84F right before this body (rtti-vtables.tsv).
// Layout: PDB CSpecialAbilitiesBerserkDef members from +0x40 sit at retail +0x3c (retail CParentDefClassBase 0x3c bytes; retail
// STLport vectors/maps are 12 bytes where the PDB has 16). Names and order are the PDB's.
// Shape: compiler-generated memberwise assignment written out; scalars and small structs are copied inline,
// strings/containers through their out-of-line operator=.
#pragma optimize("s", on)

// NF48 (round 7): the member vectors are STLport 4.6.2 std::vector<long> (was CLongVector; operator= 0x00454886), std::vector<float> (was CFloatVector; operator= 0x0045EF2D) from the shared stlport/fable_containers.h
// (local stand-ins before); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares the out-of-line operator=.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)



class CSpecialAbilitiesBerserkDef : public CParentDefClassBase { public:// vtable 0x012341FC
    float        SpeedMultiplier;                                     // +0x3c
    long         FramesToFadeOut;                                     // +0x40
    float        LowerBerserkPercent;                                 // +0x44
    float        TimeForThemeTransition;                              // +0x48
    float        BlurCenterFromFloor;                                 // +0x4c
    float        BerserkThreshold;                                    // +0x50
    float        BerserkLength;                                       // +0x54
    float        BerserkIntensity;                                    // +0x58
    float        BerserkFade;                                         // +0x5c
    float        BerserkRange;                                        // +0x60
    float        BerserkTwist;                                        // +0x64
    float        TimeForRadialBlurToFadeOut;                          // +0x68
    float        CameraStartDist;                                     // +0x6c
    float        CameraEndDist;                                       // +0x70
    float        CameraStartDeltaHeight;                              // +0x74
    float        CameraEndDeltaHeight;                                // +0x78
    std::vector<float> BerserkDuration;                                     // +0x7c
    std::vector<float> BerserkDamageToEnemyMultiplier;                      // +0x88
    float        BerserkDamageToEnemyMultiplierAdditionPerHit;        // +0x94
    std::vector<float> BerserkDamageToMeMultiplier;                         // +0x98
    std::vector<float> MaxIncreaseInStrength;                               // +0xa4
    std::vector<float> BerserkScale;                                        // +0xb0
    float        BerserkBlastRadius;                                  // +0xbc
    std::vector<float> CamDist;                                             // +0xc0
    std::vector<float> CamHeight;                                           // +0xcc
    std::vector<float> CamFOV;                                              // +0xd8
    std::vector<float> CamSwitchTime;                                       // +0xe4
    std::vector<float> RumbleLevel;                                         // +0xf0
    std::vector<float> RumbleSmoothness;                                    // +0xfc
    std::vector<float> RumbleAttack;                                        // +0x108
    std::vector<float> RumbleDecay;                                         // +0x114
    float        RumbleReleaseLevelDelta;                             // +0x120
    float        RumbleReleaseSmoothnessDelta;                        // +0x124
    float        RumbleReleaseAttackDelta;                            // +0x128
    float        RumbleReleaseDecayDelta;                             // +0x12c
    float        CrazyActionRadius;                                   // +0x130
    float        CrazyActionDamage;                                   // +0x134
    std::vector<float> DecapitateMaxProb;                                   // +0x138
    float        TargetNearestEnemyDistance;                          // +0x144
    long         WeaponTrailAttack;                                   // +0x148
    long         WeaponTrailKnockDown;                                // +0x14c
    std::vector<float> LevelUpTimings;                                      // +0x150
    std::vector<long>  TintColorR;                                          // +0x15c
    std::vector<long>  TintColorG;                                          // +0x168
    std::vector<long>  TintColorB;                                          // +0x174
    bool         CanBlockWhileBerserked;                              // +0x180
    float        CrazyIdleIntervalSecs;                               // +0x184
    CSpecialAbilitiesBerserkDef& operator=(const CSpecialAbilitiesBerserkDef& other);
};

CSpecialAbilitiesBerserkDef& CSpecialAbilitiesBerserkDef::operator=(const CSpecialAbilitiesBerserkDef& other)
{
    CParentDefClassBase::operator=(other);
    SpeedMultiplier = other.SpeedMultiplier;
    FramesToFadeOut = other.FramesToFadeOut;
    LowerBerserkPercent = other.LowerBerserkPercent;
    TimeForThemeTransition = other.TimeForThemeTransition;
    BlurCenterFromFloor = other.BlurCenterFromFloor;
    BerserkThreshold = other.BerserkThreshold;
    BerserkLength = other.BerserkLength;
    BerserkIntensity = other.BerserkIntensity;
    BerserkFade = other.BerserkFade;
    BerserkRange = other.BerserkRange;
    BerserkTwist = other.BerserkTwist;
    TimeForRadialBlurToFadeOut = other.TimeForRadialBlurToFadeOut;
    CameraStartDist = other.CameraStartDist;
    CameraEndDist = other.CameraEndDist;
    CameraStartDeltaHeight = other.CameraStartDeltaHeight;
    CameraEndDeltaHeight = other.CameraEndDeltaHeight;
    BerserkDuration = other.BerserkDuration;
    BerserkDamageToEnemyMultiplier = other.BerserkDamageToEnemyMultiplier;
    BerserkDamageToEnemyMultiplierAdditionPerHit = other.BerserkDamageToEnemyMultiplierAdditionPerHit;
    BerserkDamageToMeMultiplier = other.BerserkDamageToMeMultiplier;
    MaxIncreaseInStrength = other.MaxIncreaseInStrength;
    BerserkScale = other.BerserkScale;
    BerserkBlastRadius = other.BerserkBlastRadius;
    CamDist = other.CamDist;
    CamHeight = other.CamHeight;
    CamFOV = other.CamFOV;
    CamSwitchTime = other.CamSwitchTime;
    RumbleLevel = other.RumbleLevel;
    RumbleSmoothness = other.RumbleSmoothness;
    RumbleAttack = other.RumbleAttack;
    RumbleDecay = other.RumbleDecay;
    RumbleReleaseLevelDelta = other.RumbleReleaseLevelDelta;
    RumbleReleaseSmoothnessDelta = other.RumbleReleaseSmoothnessDelta;
    RumbleReleaseAttackDelta = other.RumbleReleaseAttackDelta;
    RumbleReleaseDecayDelta = other.RumbleReleaseDecayDelta;
    CrazyActionRadius = other.CrazyActionRadius;
    CrazyActionDamage = other.CrazyActionDamage;
    DecapitateMaxProb = other.DecapitateMaxProb;
    TargetNearestEnemyDistance = other.TargetNearestEnemyDistance;
    WeaponTrailAttack = other.WeaponTrailAttack;
    WeaponTrailKnockDown = other.WeaponTrailKnockDown;
    LevelUpTimings = other.LevelUpTimings;
    TintColorR = other.TintColorR;
    TintColorG = other.TintColorG;
    TintColorB = other.TintColorB;
    CanBlockWhileBerserked = other.CanBlockWhileBerserked;
    CrazyIdleIntervalSecs = other.CrazyIdleIntervalSecs;
    return *this;
}