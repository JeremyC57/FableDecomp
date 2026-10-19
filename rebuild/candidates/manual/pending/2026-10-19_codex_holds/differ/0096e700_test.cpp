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

#include <cstdio>
static int errors,scenario,worldCalls,preferredCalls,occupiedCalls,positionCalls,enemyGets;
static NearbyThing actor,enemy;static CombatPhysics physics;static NearbyManager manager;static NearbyWorld world;
static C3DVector origin={0,0,1000},enemyPosition={0,0,-1000},preferred;
const float ReachScale=0.8333333134651184f,PreferredMargin=0.3f,ReachEpsilon=0.0001f,EnemyMargin=1.0f;
bool NearbyThing::IsRegisteredCombatant(){if(this!=&actor)++errors;return scenario!=0;}
const C3DVector& NearbyThing::GetPosition(){++positionCalls;if(this==&actor)return origin;if(this!=&enemy)++errors;return enemyPosition;}
NearbyThing* NearbyPointer::GetPItem()const{++enemyGets;return &enemy;}
NearbyWorld* NearbyMethods::GetPWorld(){++worldCalls;return &world;}
C3DVector NearbyManager::GetPreferredPos(NearbyThing* p){++preferredCalls;if(this!=&manager||p!=&actor||worldCalls!=1)++errors;return preferred;}
bool NearbyManager::IsPositionOccupied(NearbyThing* p){++occupiedCalls;if(this!=&manager||p!=&actor||worldCalls!=2)++errors;if(scenario==5)physics.Radius=10;return scenario==3;}
int main(){NearbyMethods g;CAIBrain brain;
#ifdef _MSC_VER
((CAIStateGroupBase*)&g)->PBrain=&brain;brain.Creature=(CThingAICreature*)&actor;
#else
g.PBrain=&brain;brain.Creature=&actor;
#endif
actor.Physics=&physics;
#ifdef _MSC_VER
world.PThingCombatManager_ptr=(CCombatManager*)&manager;
#else
world.PThingCombatManager_ptr=&manager;
#endif

for(scenario=0;scenario<6;++scenario){worldCalls=preferredCalls=occupiedCalls=positionCalls=enemyGets=0;physics.Radius=1;preferred.X=scenario==1?1:100;preferred.Y=0;preferred.Z=9999;enemyPosition.X=scenario>=4?5:1;bool expected=scenario==1||scenario==2||scenario==5;bool result=g.IsEnemyCreatureInCombatPosition();int later=scenario>=2;
if(result!=expected||worldCalls!=(scenario==0?0:scenario==1?1:2)||preferredCalls!=(scenario!=0)||occupiedCalls!=later||enemyGets!=later||positionCalls!=(scenario==0?0:scenario==1?1:3))++errors;}
if(errors){std::printf("0096e700_TEST FAIL %d\n",errors);return 1;}std::printf("0096e700_TEST PASS\n");return 0;}