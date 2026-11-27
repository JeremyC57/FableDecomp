#ifndef FABLE_ENGINE_CTHINGFILTER_ISWITHINAREA_H
#define FABLE_ENGINE_CTHINGFILTER_ISWITHINAREA_H
#ifdef FABLE_RETAIL_EXPLOSION_HITS_PASS29
#include "engine/C3DVector.h"
class CThing;
class CThingFilter_IsWithinArea {
    const C3DVector& StartPos;
    const C3DVector& EndPos;
    float Radius;
    bool IncludeThingRad;
public:
    CThingFilter_IsWithinArea(const C3DVector& a,const C3DVector& b,float r,bool include):StartPos(a),EndPos(b),Radius(r),IncludeThingRad(include){}
    bool operator()(const CThing&)const;
};
#endif
#endif
