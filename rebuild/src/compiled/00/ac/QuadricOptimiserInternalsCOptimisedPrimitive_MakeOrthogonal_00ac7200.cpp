// QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal -- project a vector and preserve its length.
// Flags: /Gr
// Retail: 00ac7200,255 bytes,private const member with two vector references.
// Leaf: ?MakeOrthogonal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABV3@0@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_XYZ
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#include "engine/COptimisedPrimitive.h"
inline float FABLE_FASTCALL GFFloatAbs(float f){unsigned long bits=*(unsigned long*)&f;bits&=0x7fffffff;return *(float*)&bits;}
inline bool FABLE_FASTCALL GFIsNonZero(float f){return GFFloatAbs(f)>DELTA_NOTIONAL_ZERO;}
C3DVector QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal(const C3DVector& vect,const C3DVector& normal)const
{
 C3DVector result=vect;
 float vect_len=vect.GetMagnitude();
 if(GFIsNonZero(vect_len)){
  result-=normal*vect.Dot(normal);
  if(result.IsNonZero())result*=vect_len/result.GetMagnitude();
 }
 return result;
}