// CApplyAdditionalExplosiveImpulseIfInsideRing::operator() -- push objects outward inside an explosion ring
// Flags:
// Retail: 0080db70,275 bytes,const thiscall,one CThing pointer argument.
// Leaf: ??RCApplyAdditionalExplosiveImpulseIfInsideRing@@QBEXPAVCThing@@@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CApplyAdditionalExplosiveImpulseIfInsideRing.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
void CApplyAdditionalExplosiveImpulseIfInsideRing::operator()(CThing* pnearby_thing) const
{
 CTCPhysicsBase& their_phys=*pnearby_thing->GetPhysicsTC();
 const C3DVector& their_pos=their_phys.GetPosition();
 C3DVector explosion_to_thing=their_pos-BasePosition;
 explosion_to_thing.Z=0.0f;
 float mag=explosion_to_thing.GetAccurateMagnitude();
 if(mag<DELTA_NOTIONAL_ZERO) explosion_to_thing=C3DVector(1.0f,0.0f,0.0f);
 else explosion_to_thing/=mag;
 if(mag>InnerRadius&&mag<OuterRadius){
  float dist_to_move=OuterRadius-mag;
  const float FRACTION_OF_DISTANCE_OUT_TO_MOVE_EACH_FRAME=0.25f;
  their_phys.ApplyControlledImpulse(explosion_to_thing*dist_to_move*FRACTION_OF_DISTANCE_OUT_TO_MOVE_EACH_FRAME);
  their_phys.SetAsMoving(true);
 }
}