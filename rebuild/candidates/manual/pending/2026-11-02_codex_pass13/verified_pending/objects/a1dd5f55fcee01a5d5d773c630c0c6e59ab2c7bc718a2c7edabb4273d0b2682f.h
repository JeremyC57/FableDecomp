#ifndef FABLE_ENGINE_CINTERPOLATIONINFO_H
#define FABLE_ENGINE_CINTERPOLATIONINFO_H
#include "rebuild_abi.h"

// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CInterpolationInfo { public: class CInterpolationInfoSet {
public: float GTPredictedTimeSinceLastRenderFrame;
public: float WFInterpolate;
};
public: CInterpolationInfo::CInterpolationInfoSet Current;
public: CInterpolationInfo::CInterpolationInfoSet Last;
public: CInterpolationInfo::CInterpolationInfoSet PausedCurrent;
public: CInterpolationInfo::CInterpolationInfoSet PausedLast;
public: CInterpolationInfo::CInterpolationInfoSet BulletTimeCurrent;
public: CInterpolationInfo::CInterpolationInfoSet BulletTimeLast;
public: long WFServerCurrentTime;
};
FABLE_STATIC_ASSERT(sizeof(CInterpolationInfo)==52);
#endif
