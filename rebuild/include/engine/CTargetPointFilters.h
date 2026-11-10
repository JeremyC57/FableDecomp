#ifndef FABLE_ENGINE_CTARGETPOINTFILTERS_H
#define FABLE_ENGINE_CTARGETPOINTFILTERS_H
#include "engine/NPlayerTargeting.h"
inline float FABLE_FASTCALL GFSqr(float value){return value*value;}
namespace {
class CTargetPointFilter_FurtherFromPosThan : public std::unary_function<NPlayerTargeting::CTargetPoint,bool> {
public:
 CTargetPointFilter_FurtherFromPosThan(const C3DVector& pos,float range):Pos(pos),Range(range){}
 bool operator()(const NPlayerTargeting::CTargetPoint& point)const {
  if((point.TargetingPosition-Pos).GetSquaredMagnitude()>GFSqr(Range))return true;
  return false;
 }
private:
 const C3DVector& Pos;
 float Range;
};
class CTargetPointFilter_IsLockedTarget : public std::unary_function<NPlayerTargeting::CTargetPoint,bool> {
public:
 CTargetPointFilter_IsLockedTarget(const CThing* target):PLockedTarget(target){}
 bool operator()(const NPlayerTargeting::CTargetPoint& point)const{return point.PThing==PLockedTarget;}
private:
 const CThing* PLockedTarget;
};
}
#endif
