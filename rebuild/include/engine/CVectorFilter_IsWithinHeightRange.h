#ifndef FABLE_ENGINE_CVECTORFILTER_ISWITHINHEIGHTRANGE_H
#define FABLE_ENGINE_CVECTORFILTER_ISWITHINHEIGHTRANGE_H
#ifdef FABLE_RETAIL_CVECTORFILTER_ISWITHINHEIGHTRANGE_PASS30
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include <math.h>
// Ego_r and FableWin type records give Height and Range; FableWin 0x02C4FB80 evaluates their open band.
class CVectorFilter_IsWithinHeightRange : public std::unary_function<C3DVector,bool> {
public:
    CVectorFilter_IsWithinHeightRange(float height, float range) : Height(height), Range(range) {}
    bool operator()(const C3DVector& pos) const { return fabs(pos.Z - Height) < Range; }
private:
    float Height;
    float Range;
};
#endif
#endif
