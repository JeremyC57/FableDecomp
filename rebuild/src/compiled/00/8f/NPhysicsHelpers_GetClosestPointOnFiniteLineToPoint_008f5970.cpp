// NPhysicsHelpers::GetClosestPointOnFiniteLineToPoint -- project onto a finite segment.
// Flags: /Gr
// Retail: 008f5970,465 bytes,fastcall returning a vector by value.
// Leaf: ?GetClosestPointOnFiniteLineToPoint@NPhysicsHelpers@@YI?AVC3DVector@@ABV2@00@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/C3DVector.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
inline float FABLE_FASTCALL GFSqr(float n){return n*n;}
namespace NPhysicsHelpers {
C3DVector FABLE_FASTCALL GetClosestPointOnFiniteLineToPoint(const C3DVector& start_pos,const C3DVector& end_pos,const C3DVector& test_pos)
{
 C3DVector point_to_use=start_pos;
 C3DVector line=end_pos-start_pos;
 float line_mag=line.GetAccurateMagnitude();
 if(line_mag>DELTA_NOTIONAL_ZERO){
  line/=line_mag;
  C3DVector start_to_thing=test_pos-start_pos;
  point_to_use=start_pos+line*start_to_thing.Dot(line);
  float sqr_dist_start=(point_to_use-start_pos).GetSquaredMagnitude();
  float sqr_dist_end=(point_to_use-end_pos).GetSquaredMagnitude();
  if(!(sqr_dist_start<GFSqr(line_mag)&&sqr_dist_end<GFSqr(line_mag))){
   if(sqr_dist_start<sqr_dist_end)point_to_use=start_pos;
   else point_to_use=end_pos;
  }
 }
 return point_to_use;
}
}