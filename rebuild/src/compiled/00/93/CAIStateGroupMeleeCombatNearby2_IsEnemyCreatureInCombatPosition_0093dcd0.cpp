// CAIStateGroup_MeleeCombatNearby2::IsEnemyCreatureInCombatPosition() -- require combat range and a valid or nearby preferred position.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
struct C3DVector {
 float X,Y,Z;
 C3DVector() {}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z) {}
 C3DVector operator-(const C3DVector& b) const { return C3DVector(X-b.X,Y-b.Y,Z-b.Z); }
 float GetLengthSquared2D() const { return X*X+Y*Y; }
};
inline float GroundDistanceSquared(const C3DVector& a,const C3DVector& b) {
 return (a-b).GetLengthSquared2D();
}
struct CCreatureCombatView {
 bool IsRegisteredCombatant(); // 00662410 (propagated AddCombinationBlend label is incompatible with this call)
 const C3DVector& GetPosition() const; // 004C73D0
};
struct CCombatPositionMethods {
 bool CheckRingRangeToTarget(CThingAICreature*); // 006E94D0
 bool ValidateSequencePosition(CThingAICreature*); // 006E9700
 bool IsCreatureWithValidMeleeCombatSequence(CThingAICreature*); // 006E9450
 C3DVector GetPreferredPos(CThingAICreature*); // 006EA770
};
struct CCombatStateMethods : CAIStateGroupBase {
 CWorld* GetPWorld(); // 008FCFC0
 inline bool InRange() {
  if(((CCreatureCombatView*)PBrain->Creature)->IsRegisteredCombatant()) {
  CThingAICreature* creature=PBrain->Creature;
  CCombatPositionMethods* combat=(CCombatPositionMethods*)GetPWorld()->PThingCombatManager_ptr;
  if(combat->CheckRingRangeToTarget(creature)) return true;
  }
  return false;
 }
 bool IsEnemyCreatureInCombatPosition();
};
bool CCombatStateMethods::IsEnemyCreatureInCombatPosition() {
 bool inRange=InRange();
 CThingAICreature* creature=PBrain->Creature;
 CCombatPositionMethods* combat=(CCombatPositionMethods*)GetPWorld()->PThingCombatManager_ptr;
 if(combat->ValidateSequencePosition(creature)) return inRange;
 if(inRange) {
  creature=PBrain->Creature;
  combat=(CCombatPositionMethods*)GetPWorld()->PThingCombatManager_ptr;
  if(combat->IsCreatureWithValidMeleeCombatSequence(creature)) return true;
  creature=PBrain->Creature;
  combat=(CCombatPositionMethods*)GetPWorld()->PThingCombatManager_ptr;
  const C3DVector& position=((CCreatureCombatView*)creature)->GetPosition();
  if(GroundDistanceSquared(combat->GetPreferredPos(creature),position)<2.25f) return true;
 }
 return false;
}