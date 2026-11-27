#ifndef FABLE_ENGINE_CWALL_H
#define FABLE_ENGINE_CWALL_H
#ifdef FABLE_RETAIL_SB_CORRIDOR_PASS29
#include "engine/C2DVector.h"
// Layout: Ego_r and Ego_d place Point1, Point2 and ToInternal at offsets 0, 8 and 16.
class CWall {
public:
    CWall(C2DVector point1, C2DVector point2, C2DVector to_internal)
    {
        Point1 = point1;
        Point2 = point2;
        ToInternal = to_internal;
    }
    C2DVector Point1, Point2, ToInternal;
};
#endif
#endif
