#ifndef FABLE_ENGINE_CFISHINGDEF_H
#define FABLE_ENGINE_CFISHINGDEF_H
#ifdef FABLE_RETAIL_CFISHINGDEF_PASS22
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
class CFishingDef:public CSubDefClassBase {
public:
 std::vector<long> FishTypes;
 float FishingSpotProximity,FishingSpotMinDepth,FishingSpotMinHeightDiff,FishingSpotMaxHeightDiff;
 long PointsPerFishingLevel;
 float SlowFishingSpeed,NormalFishingSpeed,FastFishingSpeed,FastestFishingSpeed;
 float FishWeightRainMult,FishWeightSnowMult,FishWeightMistMult;
 float BaseWaitPeriodSeconds,MaxRandomWaitPeriodSeconds,HookWindowSeconds;
 long BaseHookPresses,MaxRandomHookPresses;
 float StrainIncreasePerPress;
};
FABLE_STATIC_ASSERT(offsetof(CFishingDef,FishingSpotMinDepth)==0x38);
FABLE_STATIC_ASSERT(sizeof(CFishingDef)==0x7c);
#endif
#endif
// Retail opt-in (Codex pass22): full both-PDB CFishingDef,existing retail base and12-byte vector; no new packing.
