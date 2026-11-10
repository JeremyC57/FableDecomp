// 00a42170 CPlane::Initialise -- construct a unit plane through three points.
// Leaf: ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z
// Retail:00a42170,269 bytes,thiscall.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP
#define FABLE_RETAIL_CPLANE_LINE
#include "engine/CPlane.h"
bool CPlane::Initialise(const C3DVector& point1,const C3DVector& point2,const C3DVector& point3)
{
 C3DVector cross1;
 C3DVector cross2;
 cross1=point2-point1;
 cross2=point3-point1;
 cross1.AccurateNormalise();
 cross2.AccurateNormalise();
 Normal=cross1^cross2;
 Normal.AccurateNormalise();
 Distance=point1.Dot(Normal);
 return true;
}