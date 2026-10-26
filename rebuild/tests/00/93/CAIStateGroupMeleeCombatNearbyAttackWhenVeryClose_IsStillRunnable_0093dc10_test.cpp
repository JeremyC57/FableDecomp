#include <cstdio>
#include <limits>
#ifndef _MSC_VER
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_MELEECOMBATNEARBYATTACKWHENVERYCLOSE_H
class CThingAICreature;
class CAIBrain { public: CThingAICreature* Creature; };
class CAIStateGroupBase { public: CAIBrain* PBrain; };
class CTCPhysicsBase { public: float Radius; };
#endif
// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::IsStillRunnable(bool) -- require a runnable melee state and an enemy within the height and radius limits.
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

static bool permitted, expectedInitial;
static int baseCalls, targetCalls, positionCalls, errors;
static CMeleeCloseMethods* expectedState;
static CThingCombatGeometry* expectedCreature;
static CThingCombatGeometry* expectedEnemy;
static C3DVector creaturePosition,enemyPosition;
bool CMeleeNearbyBaseMethods::IsStillRunnable(bool initial) {
 ++baseCalls;if(this!=expectedState || initial!=expectedInitial) ++errors;return permitted;
}
CThingCombatGeometry* CIntelligentTargetView::GetPItem() const {
 ++targetCalls;if(this!=&expectedState->Enemy)++errors;return Object;
}
const C3DVector& CThingCombatGeometry::GetPosition() const {
 ++positionCalls;
 if(this==expectedCreature) { if(positionCalls!=1)++errors;return creaturePosition; }
 if(this!=expectedEnemy || positionCalls!=2)++errors;return enemyPosition;
}
static bool run(bool allowed,bool initial,float dx,float dy,float dz,float r1,float r2,bool accepted,bool heightPassed) {
 CThingCombatGeometry me,enemy;CTCPhysicsBase p1,p2;CAIBrain brain;CMeleeCloseMethods state;
 me.PhysicsTC=&p1;enemy.PhysicsTC=&p2;p1.Radius=r1;p2.Radius=r2;
 brain.Creature=(CThingAICreature*)&me;state.PBrain=&brain;state.Enemy.Object=&enemy;
 creaturePosition=C3DVector(0,0,0);enemyPosition=C3DVector(dx,dy,dz);
 permitted=allowed;expectedInitial=initial;expectedState=&state;expectedCreature=&me;expectedEnemy=&enemy;
 baseCalls=targetCalls=positionCalls=errors=0;
 bool result=state.IsStillRunnable(initial);
 return result==accepted && baseCalls==1 && targetCalls==(allowed?(heightPassed?2:1):0) && positionCalls==(allowed?2:0) && !errors;
}
int main() {
 float nan=std::numeric_limits<float>::quiet_NaN();
 if(!run(false,true,0,0,0,0,0,false,false) ||
    !run(true,false,0,0,0,0,0,true,true) ||
    !run(true,true,1,1,1.99f,0,0,true,true) ||
    !run(true,true,0,0,2,0,0,false,false) ||
    !run(true,true,0,0,-2,0,0,false,false) ||
    !run(true,true,0,0,-1.99f,0,0,true,true) ||
    !run(true,true,2,0,0,0,0,false,true) ||
    !run(true,true,0,2,0,0,0,false,true) ||
    !run(true,true,2.01f,0,0,0,0,false,true) ||
    !run(true,true,3,4,0,.5f,2,false,true) ||
    !run(true,true,2.9f,4,0,.5f,2,true,true) ||
    !run(true,true,nan,0,0,0,0,false,true) ||
    !run(true,true,0,0,nan,0,0,false,false)) {
  std::printf("0093dc10_TEST FAIL\n");return 1;
 }
 std::printf("0093dc10_TEST PASS\n");return 0;
}