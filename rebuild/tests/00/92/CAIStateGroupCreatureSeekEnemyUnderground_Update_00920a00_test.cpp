#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
struct C3DVector{float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}const C3DVector operator-(const C3DVector&o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}const C3DVector operator+(const C3DVector&o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}const C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}float GetMagnitude2D()const{return(float)sqrt(X*X+Y*Y);}float GetSquaredMagnitude2D()const{return X*X+Y*Y;}};
struct CTCPhysicsBase{C3DVector pos,old;CTCPhysicsBase():pos(0,0,0),old(0,0,0){}const C3DVector&GetPosition()const{return pos;}const C3DVector&GetLastPosition()const{return old;}};
struct CThing{bool dead;CTCPhysicsBase physics;CThing():dead(false){}bool IsDead()const{return dead;}const C3DVector&GetPos()const{return physics.pos;}const CTCPhysicsBase*GetPhysicsTC()const{return &physics;}};
struct CThingCreatureBase:CThing{};
struct CTCTargetingBase{CThingCreatureBase*enemy;CTCTargetingBase():enemy(0){}CThingCreatureBase*GetPBestEnemyCreatureForAttack()const{return enemy;}};
struct CTCPerceiveThing{float radius;CTCPerceiveThing():radius(10){}float GetSightRadius()const{return radius;}};
struct CThingAICreature:CThingCreatureBase{mutable int active;CTCTargetingBase targeting;CTCPerceiveThing perception;CThingAICreature():active(0){}bool IsPerformingAnyAction()const{if(active>0){--active;return true;}return false;}bool PeekTC(const CTCTargetingBase**p)const{*p=&targeting;return true;}bool PeekTC(const CTCPerceiveThing**p)const{*p=&perception;return true;}};
template<class T>struct CIntelligentPointer{T*p;CIntelligentPointer():p(0){}CIntelligentPointer(T*q):p(q){}T*operator->()const{return p;}bool operator!=(int)const{return p!=0;}};
template<class T>bool IS_THING_ALIVE(const T&t){if(t!=0){if(!t->IsDead())return true;}return false;}
inline float GFSqr(float x){return x*x;}
enum EAIActionReturn{AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};
struct CWorldMap{};struct CThingSearchTools{};
struct CAIStateGroupBase{CThingAICreature creature;CWorldMap map;CThingSearchTools tools;int term,actions;bool stopping;EAIActionReturn ret;CAIStateGroupBase():term(0),actions(0),stopping(false),ret(AI_ACTION_IN_PROGRESS){}CThingAICreature&GetCreature(){return creature;}const CWorldMap&PeekWorldMap()const{return map;}const CThingSearchTools&PeekThingSearchTools()const{return tools;}bool IsStopping()const{return stopping;}void Terminate(){++term;}EAIActionReturn AIAction_DoNothing(){++actions;return ret;}};
struct CAIStateGroup_CreatureSeekEnemyUnderground:CAIStateGroupBase{CIntelligentPointer<const CThing>PEnemy;C3DVector RisePos;CAIStateGroup_CreatureSeekEnemyUnderground():RisePos(-99,-99,-99){}bool IsStillRunnable(){if(IsStopping())return false;return true;}void Update();};
namespace NCreatureHiding{static std::vector<C3DVector>result;static C3DVector centre;static int calls;void GetValidPositions(std::vector<C3DVector>&out,const CWorldMap&,const CThingSearchTools&,const C3DVector&p,float radius,bool b,const void*shapes){if(radius!=2||b||shapes){puts("bad search arguments");exit(1);}out=result;centre=p;++calls;}}
static int randomValue;static int fixture_rand(){return randomValue;}
#define rand fixture_rand

void CAIStateGroup_CreatureSeekEnemyUnderground::Update()
{
 const float RADIUS_FOR_UNDEAD_CREATION=2.0f;
 const float MINIMUM_DISTANCE_FROM_HERO_FOR_UNDEAD_CREATION=1.5f;
 const float CREATION_POINT_OFFSET_MULTIPLIER=15.0f;
 while(GetCreature().IsPerformingAnyAction()) {
  EAIActionReturn ret=AIAction_DoNothing();
  if(ret!=AI_ACTION_IN_PROGRESS||!IsStillRunnable()){Terminate();return;}
 }
 if(!IS_THING_ALIVE(PEnemy)) {
  const CTCTargetingBase* ptc_targeting;
  GetCreature().PeekTC(&ptc_targeting);
  CThingCreatureBase* p_enemy=ptc_targeting->GetPBestEnemyCreatureForAttack();
  if(IS_THING_ALIVE(p_enemy))PEnemy=CIntelligentPointer<const CThing>(p_enemy);
 }
 if(!IsStillRunnable()){Terminate();return;}
 const CTCPerceiveThing* ptc_perception=0;
 GetCreature().PeekTC(&ptc_perception);
 if(IS_THING_ALIVE(PEnemy)) {
  if((PEnemy->GetPos()-GetCreature().GetPos()).GetMagnitude2D()<ptc_perception->GetSightRadius()*1.5f) {
   C3DVector creationOffset=(PEnemy->GetPhysicsTC()->GetPosition()-PEnemy->GetPhysicsTC()->GetLastPosition())*CREATION_POINT_OFFSET_MULTIPLIER;
   std::vector<C3DVector> positions;
   C3DVector creationCentrePoint=PEnemy->GetPos()+creationOffset;
   NCreatureHiding::GetValidPositions(positions,PeekWorldMap(),PeekThingSearchTools(),creationCentrePoint,RADIUS_FOR_UNDEAD_CREATION,false,0);
   std::vector<C3DVector> valid_positions;
   long numPositions=positions.size();
   valid_positions.reserve(numPositions);
   long i;
   for(i=0;i<numPositions;++i) {
    if((positions[i]-PEnemy->GetPos()).GetSquaredMagnitude2D()>=GFSqr(MINIMUM_DISTANCE_FROM_HERO_FOR_UNDEAD_CREATION))valid_positions.push_back(positions[i]);
   }
   if(valid_positions.size()>0){i=rand()%(long)valid_positions.size();RisePos=valid_positions[i];}
   Terminate();return;
  }
 }
 AIAction_DoNothing();
 RisePos=GetCreature().GetPos();
}
#undef rand
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
static bool equal(const C3DVector&a,const C3DVector&b){return fabs(a.X-b.X)<0.00003&&fabs(a.Y-b.Y)<0.00003&&fabs(a.Z-b.Z)<0.00003;}
static void reset(){NCreatureHiding::calls=0;NCreatureHiding::result.clear();randomValue=0;}
int main(){
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;m.creature.physics.pos=C3DVector(1,2,3);m.Update();check(m.actions==1&&m.term==0&&equal(m.RisePos,m.creature.GetPos()),"no enemy idles and rises in place");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;m.creature.active=1;m.ret=AI_ACTION_FINISHED;m.Update();check(m.term==1&&m.actions==1&&m.RisePos.X==-99,"action completion terminates");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;m.creature.active=1;m.stopping=true;m.Update();check(m.term==1&&m.actions==1&&NCreatureHiding::calls==0,"action stopping terminates");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;m.stopping=true;m.Update();check(m.term==1&&m.actions==0,"stopping with no action");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;CThingCreatureBase e;e.dead=true;m.creature.targeting.enemy=&e;m.Update();check(m.PEnemy.p==0&&m.term==0&&m.actions==1,"dead target ignored");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;CThingCreatureBase e;e.physics.pos=C3DVector(20,0,0);m.creature.targeting.enemy=&e;m.Update();check(m.PEnemy.p==&e&&m.actions==1&&NCreatureHiding::calls==0,"far enemy remembered");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;CThingCreatureBase e;e.physics.pos=C3DVector(15,0,0);m.PEnemy=CIntelligentPointer<const CThing>(&e);m.Update();check(m.actions==1&&NCreatureHiding::calls==0,"sight range strict boundary");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;CThingCreatureBase e;e.physics.pos=C3DVector(1,2,3);e.physics.old=C3DVector(0.9f,1.8f,2.7f);m.creature.targeting.enemy=&e;NCreatureHiding::result.push_back(C3DVector(1,2,10));NCreatureHiding::result.push_back(C3DVector(2.5f,2,3));NCreatureHiding::result.push_back(C3DVector(5,6,7));randomValue=3;m.Update();check(m.term==1&&m.actions==0&&equal(m.RisePos,C3DVector(5,6,7)),"filter XY-only, accept threshold, choose modulo");check(equal(NCreatureHiding::centre,C3DVector(2.5f,5,7.5f)),"creation centre extrapolates old position");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;CThingCreatureBase e;CThingCreatureBase stale;stale.dead=true;m.PEnemy=CIntelligentPointer<const CThing>(&stale);m.creature.targeting.enemy=&e;m.Update();check(m.PEnemy.p==&e&&m.term==1&&m.RisePos.X==-99,"replace dead enemy, empty search preserves rise point");}
 reset();{CAIStateGroup_CreatureSeekEnemyUnderground m;CThingCreatureBase e;m.PEnemy=CIntelligentPointer<const CThing>(&e);m.creature.active=2;m.Update();check(m.actions==2&&m.term==1,"running actions waited before search");}
 puts("00920a00_TEST PASS");return 0;
}