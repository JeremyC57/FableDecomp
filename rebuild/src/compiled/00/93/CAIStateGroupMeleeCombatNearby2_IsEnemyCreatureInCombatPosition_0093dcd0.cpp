// CAIStateGroup_MeleeCombatNearby2::IsEnemyCreatureInCombatPosition() -- require combat range and a valid or nearby preferred position.
// Leaf: ?IsEnemyCreatureInCombatPosition@CCombatStateMethods@@QAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared2D respelled GetSquaredMagnitude2D (the header / FableWin.pdb method, same computation).
#define CAIStateGroupBase CAIStateGroupBaseLayout   // engine/CAIStateGroupBase.h flattens the vptr
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
class CWorld;
class CAIStateGroupBase : public CAIStateGroupBaseLayout { public:
protected:
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/C3DVector.h"
inline float GroundDistanceSquared(const C3DVector& a,const C3DVector& b) {
 return (a-b).GetSquaredMagnitude2D();
}
#include "engine/CThing.h"
struct CCreatureCombatView : public CThing {
 bool IsRegisteredCombatant(); // 00662410 (propagated AddCombinationBlend label is incompatible with this call)
};
class CThingCreatureBase;
class CCombatManager { public:
 bool IsCreatureInAttackRingSector(const CThingCreatureBase& creature) const;   // 0x006E94D0 (Ego_r/FableWin ?IsCreatureInAttackRingSector@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z)
};
struct CCombatPositionMethods {
 bool ValidateSequencePosition(CThingAICreature*); // 006E9700
 bool IsCreatureWithValidMeleeCombatSequence(CThingAICreature*); // 006E9450
 C3DVector GetPreferredPos(CThingAICreature*); // 006EA770
};
struct CCombatStateMethods : CAIStateGroupBase {
 inline bool InRange() {
  if(((CCreatureCombatView*)PBrain->Creature)->IsRegisteredCombatant()) {
  CThingAICreature* creature=PBrain->Creature;
  CCombatManager* combat=(CCombatManager*)GetWorld().PThingCombatManager_ptr;
  if(combat->IsCreatureInAttackRingSector(*(const CThingCreatureBase*)creature)) return true;
  }
  return false;
 }
 bool IsEnemyCreatureInCombatPosition();
};
bool CCombatStateMethods::IsEnemyCreatureInCombatPosition() {
 bool inRange=InRange();
 CThingAICreature* creature=PBrain->Creature;
 CCombatPositionMethods* combat=(CCombatPositionMethods*)GetWorld().PThingCombatManager_ptr;
 if(combat->ValidateSequencePosition(creature)) return inRange;
 if(inRange) {
  creature=PBrain->Creature;
  combat=(CCombatPositionMethods*)GetWorld().PThingCombatManager_ptr;
  if(combat->IsCreatureWithValidMeleeCombatSequence(creature)) return true;
  creature=PBrain->Creature;
  combat=(CCombatPositionMethods*)GetWorld().PThingCombatManager_ptr;
  const C3DVector& position=((CCreatureCombatView*)creature)->GetPos();
  if(GroundDistanceSquared(combat->GetPreferredPos(creature),position)<2.25f) return true;
 }
 return false;
}