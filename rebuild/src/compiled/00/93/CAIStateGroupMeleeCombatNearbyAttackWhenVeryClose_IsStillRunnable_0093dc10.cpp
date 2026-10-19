// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::IsStillRunnable(bool) -- require a runnable melee state and an enemy within the height and radius limits.
// Grade: RELOCATION_MATCH -- named squared-coordinate locals retain the retail x87 evaluation order (192 bytes).
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h"
#include <math.h>
struct C3DVector {
 float X,Y,Z;
 C3DVector() {}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z) {}
 C3DVector operator-(const C3DVector& b) const { return C3DVector(X-b.X,Y-b.Y,Z-b.Z); }
 float GetLengthSquared2D() const { float xx=X*X; float yy=Y*Y; return yy+xx; }
};

struct CThingCombatGeometry {
 unsigned char _beforePhysics[0x60]; CTCPhysicsBase* PhysicsTC;
 const C3DVector& GetPosition() const; // 004C73D0
};
struct CIntelligentTargetView {
 void* __vftable; CThingCombatGeometry* Object;
 CThingCombatGeometry* GetPItem() const; // 00A01B50
};
struct CMeleeNearbyBaseMethods : CAIStateGroupBase {
 CIntelligentTargetView Enemy;
 bool IsStillRunnable(bool); // 0093D800
};
struct CMeleeCloseMethods : CMeleeNearbyBaseMethods {
 bool IsStillRunnable(bool);
};
bool CMeleeCloseMethods::IsStillRunnable(bool initial) {
 if(CMeleeNearbyBaseMethods::IsStillRunnable(initial)) {
  CThingCombatGeometry* creature=(CThingCombatGeometry*)PBrain->Creature;
  CThingCombatGeometry* enemy=Enemy.GetPItem();
  const C3DVector& mine=creature->GetPosition();
  C3DVector delta=enemy->GetPosition()-mine;
  if(fabs(delta.Z)<2.0f) {
   CThingCombatGeometry* other=Enemy.GetPItem();
   CThingCombatGeometry* me=(CThingCombatGeometry*)PBrain->Creature;
   float radius=(me->PhysicsTC->Radius+other->PhysicsTC->Radius)*1.2f+2.0f;
   if(delta.GetLengthSquared2D()<radius*radius) return true;
  }
 }
 return false;
}