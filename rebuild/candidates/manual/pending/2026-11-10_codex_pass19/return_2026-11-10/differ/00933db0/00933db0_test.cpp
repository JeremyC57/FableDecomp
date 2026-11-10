#include <cstdio>
#include <cstdlib>
static void check(bool b){if(!b)std::abort();}
enum EAIActionReturn{AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};enum EAIActionRepeat{ACTION_REPEAT_CALLBACK=2};enum EMovementType{AI_MOVEMENT_RUNNING=5};
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
struct CTCPhysicsBase{float radius;float GetRadius()const{return radius;}};
struct CThing{bool dead;C3DVector pos;CTCPhysicsBase physics;CThing():dead(false){}bool IsDead()const{return dead;}const C3DVector& GetPos()const{return pos;}CTCPhysicsBase* GetPhysicsTC(){return &physics;}};
struct CThingCreatureBase:CThing{};struct CThingAICreature:CThingCreatureBase{};
template<class T>struct CIntelligentPointer{T* ptr;CIntelligentPointer(T* p=0):ptr(p){}template<class U>CIntelligentPointer(CIntelligentPointer<U>& p):ptr(p.ptr){}T* Get()const{return ptr;}T* operator->()const{return ptr;}};
template<class T>bool operator==(const CIntelligentPointer<T>& p,int){return p.Get()==0;}template<class T>bool operator!=(const CIntelligentPointer<T>& p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T& p){return p!=0&&!p->IsDead();}template<class T>bool IS_THING_DEAD(const T& p){return p==0||p->IsDead();}
static bool GFWorld2DDistanceSmallerThan(const C3DVector& a,const C3DVector& b,float d,bool squared){float x=a.X-b.X,y=a.Y-b.Y;return x*x+y*y<(squared?d:d*d);}
struct CCreatureActionBase{CThingCreatureBase* owner;CThingCreatureBase* target;CCreatureActionBase(CThingCreatureBase& o,CThingCreatureBase* t):owner(&o),target(t){}virtual ~CCreatureActionBase(){}};
struct CCreatureAction_KickCreature:CCreatureActionBase{CCreatureAction_KickCreature(CThingCreatureBase& o,CThingCreatureBase* t):CCreatureActionBase(o,t){}};
struct CAIStateGroup_MinionKidAnnoy{
 CThingAICreature* creature;CIntelligentPointer<CThingCreatureBase> PTarget;int moveCalls,kicks,terminated,retries,maxKicks;bool dieOnMove;EAIActionReturn moveResult,kickResult;
 CThingAICreature& GetCreature()const{return *creature;}
 EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing> target,float radius,bool& nav,EMovementType movement,bool a,bool b,bool c,bool d,float speed){
  check(repeat==ACTION_REPEAT_CALLBACK&&target.ptr==PTarget.ptr&&radius==creature->physics.radius+PTarget->physics.radius&&movement==AI_MOVEMENT_RUNNING&&a&&!b&&c&&d&&speed==0);
  nav=true;++moveCalls;check(moveCalls<20);if(moveCalls%(retries+1))return AI_ACTION_IN_PROGRESS;
  if(dieOnMove)PTarget->dead=true;return moveResult;
 }
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action,bool b){check(!b&&action.owner==creature&&action.target==PTarget.ptr);++kicks;if(kicks==maxKicks)PTarget->dead=true;return kickResult;}
 void Terminate(){++terminated;}
 void Update();
};
void CAIStateGroup_MinionKidAnnoy::Update()
{
 const float MAX_RANGE=9.0f;
 EAIActionReturn ret;
 bool nav_route_calculated;
 do {
  ret=AI_ACTION_FAILED;
  do {
   if(IS_THING_DEAD(PTarget)||!GFWorld2DDistanceSmallerThan(GetCreature().GetPos(),PTarget->GetPos(),MAX_RANGE,false)){Terminate();return;}
   const CTCPhysicsBase& physics=*GetCreature().GetPhysicsTC();
   float attack_range=PTarget->GetPhysicsTC()->GetRadius();
   attack_range+=physics.GetRadius();
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTarget,attack_range,nav_route_calculated,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS);
  if(ret!=AI_ACTION_FINISHED){Terminate();return;}
  if(IS_THING_ALIVE(PTarget)) {
   ret=AIAction_PerformActionAndWait(CCreatureAction_KickCreature(GetCreature(),PTarget.Get()),false);
   if(ret!=AI_ACTION_FINISHED){Terminate();return;}
  }
 }while(true);
}

int main(){
 CThingAICreature creature;CThingCreatureBase target;CAIStateGroup_MinionKidAnnoy state;state.creature=&creature;
 creature.pos=C3DVector(0,0,100);creature.physics.radius=1.25f;target.physics.radius=2.5f;
 const float distances[4]={0,8.75f,9,9.25f};int cases=0;
 for(int flags=0;flags<8;++flags)for(int dist=0;dist<4;++dist)for(int retries=0;retries<3;++retries)for(int move=0;move<4;++move)for(int kick=0;kick<4;++kick)for(int limit=1;limit<=2;++limit){
  state.PTarget.ptr=(flags&1)?&target:0;target.dead=(flags&2)!=0;target.pos=C3DVector(distances[dist],0,-100);
  state.moveCalls=state.kicks=state.terminated=0;state.retries=retries;state.maxKicks=limit;state.dieOnMove=(flags&4)!=0;state.moveResult=EAIActionReturn(move);state.kickResult=EAIActionReturn(kick);
  bool allowed=(flags&1)&&!(flags&2)&&distances[dist]<9;
  int expectedKicks=(allowed&&move==2&&!(flags&4))?(kick==2?limit:1):0;
  int expectedMoves=allowed?(retries+1)*(expectedKicks?expectedKicks:1):0;
  state.Update();check(state.terminated==1&&state.moveCalls==expectedMoves&&state.kicks==expectedKicks);++cases;
 }
 check(cases==3072);std::puts("00933db0_TEST PASS");return 0;
}
