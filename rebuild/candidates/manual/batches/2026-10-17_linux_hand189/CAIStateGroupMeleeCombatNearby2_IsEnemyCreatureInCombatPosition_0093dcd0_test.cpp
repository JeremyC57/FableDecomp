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

#include <stdio.h>
#include <string.h>
static CWorld world;
static CCombatPositionMethods combat;
static C3DVector position(10.0f,20.0f,3.0f), preferred(10.0f,20.0f,99.0f);
static CThingAICreature* expectedCreature;
static bool registeredCombatant, ringRange, validated, validSequence;
static int rangeCalls,validateCalls,sequenceCalls,preferredCalls,badArgument;
bool CCreatureCombatView::IsRegisteredCombatant() { if((void*)this!=(void*)expectedCreature) ++badArgument; return registeredCombatant; }
const C3DVector& CCreatureCombatView::GetPosition() const { if((const void*)this!=(void*)expectedCreature) ++badArgument; return position; }
CWorld* CCombatStateMethods::GetPWorld() { return &world; }
bool CCombatPositionMethods::CheckRingRangeToTarget(CThingAICreature* p) { ++rangeCalls; if(this!=&combat||p!=expectedCreature) ++badArgument; return ringRange; }
bool CCombatPositionMethods::ValidateSequencePosition(CThingAICreature* p) { ++validateCalls; if(this!=&combat||p!=expectedCreature) ++badArgument; return validated; }
bool CCombatPositionMethods::IsCreatureWithValidMeleeCombatSequence(CThingAICreature* p) { ++sequenceCalls; if(this!=&combat||p!=expectedCreature) ++badArgument; return validSequence; }
C3DVector CCombatPositionMethods::GetPreferredPos(CThingAICreature* p) { ++preferredCalls; if(this!=&combat||p!=expectedCreature) ++badArgument; return preferred; }
#define CHECK(c) do { if(!(c)) { printf("check failed at %d\n",__LINE__); return 1; } } while(0)
int main() {
 CCombatStateMethods state; CAIBrain brain; unsigned char creature[4];
 memset(&state,0,sizeof(state));memset(&brain,0,sizeof(brain));
 state.PBrain=&brain;brain.Creature=expectedCreature=(CThingAICreature*)creature;
 world.PThingCombatManager_ptr=(CCombatManager*)&combat;
 registeredCombatant=false;ringRange=true;validated=false;validSequence=true;
 CHECK(!state.IsEnemyCreatureInCombatPosition());
 CHECK(rangeCalls==0&&validateCalls==1&&sequenceCalls==0&&preferredCalls==0);
 registeredCombatant=true;ringRange=false;validated=true;
 CHECK(!state.IsEnemyCreatureInCombatPosition());CHECK(sequenceCalls==0);
 ringRange=true;
 CHECK(state.IsEnemyCreatureInCombatPosition());CHECK(sequenceCalls==0&&preferredCalls==0);
 validated=false;validSequence=true;
 CHECK(state.IsEnemyCreatureInCombatPosition());CHECK(sequenceCalls==1&&preferredCalls==0);
 validSequence=false;
 CHECK(state.IsEnemyCreatureInCombatPosition());CHECK(preferredCalls==1);
 preferred.X=11.49f;CHECK(state.IsEnemyCreatureInCombatPosition());
 preferred.X=11.5f;CHECK(!state.IsEnemyCreatureInCombatPosition());
 preferred.X=12.0f;CHECK(!state.IsEnemyCreatureInCombatPosition());
 preferred.X=10.0f;preferred.Y=21.5f;CHECK(!state.IsEnemyCreatureInCombatPosition());
 preferred.X=11.0f;preferred.Y=21.0f;CHECK(state.IsEnemyCreatureInCombatPosition());
 CHECK(badArgument==0);
 puts("0093dcd0_TEST PASS");return 0;
}
