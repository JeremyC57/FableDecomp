// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::IsStillRunnable(bool) -- require a runnable melee state and an enemy within the height and radius limits.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Grade: RELOCATION_MATCH -- named squared-coordinate locals retain the retail x87 evaluation order (192 bytes).
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h"
#include <math.h>
class C3DVector { public:
 float X,Y,Z;
 C3DVector() {}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z) {}
 C3DVector operator-(const C3DVector& b) const { return C3DVector(X-b.X,Y-b.Y,Z-b.Z); }
 float GetLengthSquared2D() const { float xx=X*X; float yy=Y*Y; return yy+xx; }
};

class CThing { public:
    const C3DVector& GetPos() const; // 004C73D0
};
struct CThingCombatGeometry : public CThing {
 unsigned char _beforePhysics[0x60]; CTCPhysicsBase* PhysicsTC;
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentTargetView {
 void* __vftable; CThingCombatGeometry* Object;
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
  CThingCombatGeometry* enemy=((CThingCombatGeometry*)((CBaseIntelligentPointer*)&Enemy)->GetPItem());
  const C3DVector& mine=creature->GetPos();
  C3DVector delta=enemy->GetPos()-mine;
  if(fabs(delta.Z)<2.0f) {
   CThingCombatGeometry* other=((CThingCombatGeometry*)((CBaseIntelligentPointer*)&Enemy)->GetPItem());
   CThingCombatGeometry* me=(CThingCombatGeometry*)PBrain->Creature;
   float radius=(me->PhysicsTC->Radius+other->PhysicsTC->Radius)*1.2f+2.0f;
   if(delta.GetLengthSquared2D()<radius*radius) return true;
  }
 }
 return false;
}