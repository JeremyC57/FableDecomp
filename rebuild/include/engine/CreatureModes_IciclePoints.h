#ifndef FABLE_ENGINE_CREATUREMODES_ICICLEPOINTS_H
#define FABLE_ENGINE_CREATUREMODES_ICICLEPOINTS_H
#ifdef FABLE_RETAIL_ICICLE_ASSAULT_PASS30
#include "engine/NCreatureMode_CModeIceTrollGroundSmashIcicleAssault.h"
namespace {
class CIsUsableIciclePoint {
    typedef NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData CIcicleData;
public:
    CIsUsableIciclePoint(const std::vector<CIcicleData>& ici, float icicles_limit)
        : Icicles(ici), IciclesLimit(icicles_limit) {}
    bool operator()(const C3DVector&) const;
private:
    const std::vector<CIcicleData>& Icicles;
    float IciclesLimit;
};
class CGetBestIciclePointForTarget {
public:
    CGetBestIciclePointForTarget(const CThing& target, float prediction_frames);
    bool GetBestPosition(C3DVector& pos) const;
    void operator()(const C3DVector&);
private:
    C3DVector TargetPos;
    float BestPosScore;
    C3DVector BestPos;
};
}
#endif
#endif
