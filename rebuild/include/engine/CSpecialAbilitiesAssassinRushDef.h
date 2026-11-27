#ifndef FABLE_RETAIL_ASSASSIN_DEF_H
#define FABLE_RETAIL_ASSASSIN_DEF_H
#include "engine/CParentDefClassBase.h"
class CSpecialAbilitiesAssassinRushDef : public CParentDefClassBase {
public:
    float         DistanceAwayFromtargetToAssassinRushTo;    // +0x3c
    float         SafeCollideRadiusPercentage;               // +0x40
    float         DistFromFloor;                             // +0x44
    float         AssassinStartSeconds;                      // +0x48
    float         AssassinEndSeconds;                        // +0x4c
    float         AssassinThreshold;                         // +0x50
    float         AssassinLength;                            // +0x54
    float         AssassinIntensity;                         // +0x58
    float         AssassinFade;                              // +0x5c
    float         AssassinRange;                             // +0x60
    float         AssassinTwist;                             // +0x64
    float         RunSpeed;                                  // +0x68
    float         TrailDelaySecs;                            // +0x6c
    float         SlowTimeLength;                            // +0x70
    long          AssassinAlpha;                             // +0x74
    long          SlowTimeMultiplier;                        // +0x78
    float         RushDelay;                                 // +0x7c
    std::vector<float> DashDistance;
};
FABLE_STATIC_ASSERT(sizeof(CSpecialAbilitiesAssassinRushDef)==0x8c);
#endif
