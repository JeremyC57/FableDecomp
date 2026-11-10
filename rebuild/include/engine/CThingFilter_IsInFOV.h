#ifndef FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
#define FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
#include "engine/NPlayerTargeting.h"
class CThingFilter_IsInFOV : public std::unary_function<CThing,bool> {
public:
 bool operator()(const NPlayerTargeting::CTargetPoint&)const;
 bool operator()(const C3DVector&)const;
 bool operator()(const CThing&)const;
 bool operator()(const CThing*)const;
private:
 C3DVector FromPos,Direction;
 float CosFOV,FOV;
 bool AngleTest3D;
};
#endif
