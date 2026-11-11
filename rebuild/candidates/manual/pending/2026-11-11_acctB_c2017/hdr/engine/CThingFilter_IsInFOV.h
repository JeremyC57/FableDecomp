#ifndef FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
#define FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
#include "engine/NPlayerTargeting.h"
class CThingFilter_IsInFOV : public std::unary_function<CThing,bool> {
public:
#ifdef FABLE_RETAIL_CTHINGFILTER_ISINFOV_CTOR
 // Opt-in (chunk 2009, row 0x00745120): Ego_r.pdb / FableWin.pdb ??0CThingFilter_IsInFOV@@QAE@ABVC3DVector@@0MM_N@Z, out of line in retail
 // at 0x006A7150 (landed, ). Declaration only.
 CThingFilter_IsInFOV(const C3DVector& from_pos,const C3DVector& direction,float fov,float radius,bool angle_test_3d);   // 0x006A7150
#endif
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
