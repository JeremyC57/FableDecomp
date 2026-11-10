// 006afaf0 CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject -- project velocity off the horizontal collision normal.
// Leaf: ?ImpulseRespondToCollisonWithImpassableObject@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@ABV1@AAVC3DVector@@@Z
// Flags: none
// Retail:006afaf0,249 bytes.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPASSABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ISNONZERO_DIRECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsCollideResponse.h"
void CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject(const CPhysicsCollideResponse& response_info,const CTCPhysicsBase& collided_with,C3DVector& velocity)const
{
 C3DVector normal(response_info.CollideNormal);
 if(normal.ToC2DVector().IsNonZero()){
  normal.Z=0;
  normal.Normalise();
 }
 float dot=(normal.X*velocity.X+normal.Y*velocity.Y)+normal.Z*velocity.Z;
 velocity-=normal*dot;
}