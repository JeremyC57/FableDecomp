#ifndef FABLE_ENGINE_CENGINESCENERENDERSTATS_H
#define FABLE_ENGINE_CENGINESCENERENDERSTATS_H

#include "rebuild_abi.h"

// Ego_r CEngineSceneRenderStats, size 28. FableWin updates these members
// in the debug renderer.
class CEngineSceneRenderStats {
public:
    unsigned long StaticPrimitivesRendered;
    unsigned long MovingPrimitivesRendered;
    unsigned long StaticPrimitivesTouched;
    unsigned long MovingPrimitivesTouched;
    unsigned long VisibleCells;
    unsigned long BoundingBoxTests;
    unsigned long BoundingSphereTests;
};

FABLE_STATIC_ASSERT(sizeof(CEngineSceneRenderStats) == 28);

#endif
