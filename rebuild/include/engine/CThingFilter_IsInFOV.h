// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
#define FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
#include "engine/NPlayerTargeting.h"
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#ifdef _MSC_VER
#pragma intrinsic(cos,tan,sqrt)
#endif
// Original donor01d9e180/01f93a50 use the widened single-precision full turn.
inline double FABLE_FASTCALL GFCos(float n){return cos((double)n*6.28318548f);}
inline double FABLE_FASTCALL GFTan(float n){return tan((double)n*6.28318548f);}
inline float FABLE_FASTCALL GFRoot(float n){return (float)sqrt((double)n);}
extern const float DELTA_NOTIONAL_ZERO;
#endif
class CThingFilter_IsInFOV : public std::unary_function<CThing,bool> {
public:
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 CThingFilter_IsInFOV(const C3DVector&,const C3DVector&,float,float,bool);
 bool operator()(const NPlayerTargeting::CTargetPoint& test_point)const{return (*this)(test_point.TargetingPosition);}
 // Original FableWin01f93a80: early radius acceptance, then flattened direction test.
 bool operator()(const C3DVector& test_pos)const{
  C3DVector to_test_pos_direction=test_pos-FromPos;
  if(to_test_pos_direction.GetSquaredMagnitude()<0.01f)return true;
  if(!AngleTest3D)to_test_pos_direction.Z=0.0f;
  to_test_pos_direction.Normalise();
  float cos_angle=to_test_pos_direction.Dot(Direction);
  return cos_angle>CosFOV;
 }
#else
 bool operator()(const NPlayerTargeting::CTargetPoint&)const;
 bool operator()(const C3DVector&)const;
#endif
 bool operator()(const CThing&)const;
 bool operator()(const CThing*)const;
private:
 C3DVector FromPos,Direction;
 float CosFOV,FOV;
 bool AngleTest3D;
};
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
inline CThingFilter_IsInFOV::CThingFilter_IsInFOV(const C3DVector& from_pos,const C3DVector& facing_vector,float fov,float test_point_rad,bool angle_test_3d)
 :FromPos(from_pos),AngleTest3D(angle_test_3d)
{
 FOV=fov;
 float half_angle=fov*0.5f;
 CosFOV=(float)GFCos(half_angle);
 float displacement=0.0f;
 if(half_angle<0.25f)displacement=(float)(test_point_rad/GFTan(half_angle));
 Direction=facing_vector;
 if(!AngleTest3D)Direction.Z=0.0f;
 float sqr_mag=Direction.GetSquaredMagnitude();
 if(sqr_mag>DELTA_NOTIONAL_ZERO)Direction/=GFRoot(sqr_mag);
 FromPos-=Direction*displacement;
}
#endif
#endif
