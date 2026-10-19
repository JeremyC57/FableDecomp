// Grade: HELD(bytes match retail) -- ReachEpsilon bound to two different float pools (0.3 at 0x1238170, 1e-4 at 0x129ba3c); PreferredMargin/ReachEpsilon swapped | Codex source 00/96/CAIStateGroupMeleeCombatNearby_IsEnemyCreatureInCombatPosition_0096e700.cpp, flags , pass pattern 0096e700_TEST PASS
// CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition() -- check preferred position or an unoccupied nearby enemy position.
// Retail0096e700 length297. Shared group/base/brain; native physics60 and physics radius34.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_MeleeCombatNearby.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#else
#define __forceinline inline
struct CAIBrain{char prefix[24];void* Creature;};
struct CAIStateGroupBase{void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_MeleeCombatNearby:CAIStateGroupBase{void* _pad_0x14[2];};
#endif
struct C3DVector{float X,Y,Z;};
extern const float ReachScale,PreferredMargin,ReachEpsilon,EnemyMargin; //012925cc,01238170,0129ba3c,0122ded8
struct CombatPhysics{char prefix[0x34];float Radius;};
struct NearbyThing{char prefix[0x60];CombatPhysics* Physics;bool IsRegisteredCombatant();const C3DVector& GetPosition();}; //00662410,004c73d0
struct NearbyPointer{NearbyThing* GetPItem()const;}; //00a01b50
struct NearbyManager{C3DVector GetPreferredPos(NearbyThing*);bool IsPositionOccupied(NearbyThing*);}; //006ea770,006e9700
#ifndef _MSC_VER
struct CWorld{char prefix[0x4c];void* PThingCombatManager_ptr;};
#endif
struct NearbyWorld:CWorld{__forceinline NearbyManager* GetCombatManager(){return (NearbyManager*)PThingCombatManager_ptr;}};
struct NearbyMethods:CAIStateGroup_MeleeCombatNearby{
__forceinline NearbyThing* Creature(){return (NearbyThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
NearbyWorld* GetPWorld(); //008fcfc0
bool IsEnemyCreatureInCombatPosition();};
bool NearbyMethods::IsEnemyCreatureInCombatPosition(){
if(Creature()->IsRegisteredCombatant()){
NearbyThing* actor=Creature();NearbyWorld* world=GetPWorld();C3DVector preferred=world->GetCombatManager()->GetPreferredPos(actor);
const C3DVector& position=Creature()->GetPosition();
float dx=preferred.X-position.X,dy=preferred.Y-position.Y;
float reach=(Creature()->Physics->Radius+Creature()->Physics->Radius)*ReachScale+PreferredMargin+ReachEpsilon;
if(dx*dx+dy*dy<reach*reach)return true;
const C3DVector& target=((NearbyPointer*)_pad_0x14)->GetPItem()->GetPosition();
const C3DVector& current=Creature()->GetPosition();
C3DVector delta;delta.X=target.X-current.X;delta.Y=target.Y-current.Y;delta.Z=0;
NearbyThing* owner=Creature();NearbyManager* manager=GetPWorld()->GetCombatManager();
if(!manager->IsPositionOccupied(owner)){
float limit=(Creature()->Physics->Radius+Creature()->Physics->Radius)*ReachScale+EnemyMargin+ReachEpsilon;
if(delta.X*delta.X+delta.Y*delta.Y<limit*limit)return true;}
}
return false;
}