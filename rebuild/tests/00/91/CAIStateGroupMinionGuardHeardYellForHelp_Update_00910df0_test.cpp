// Self-contained C++03 fixture; both original selected bodies are copied unchanged.
#include <cstdio>
#include <cstdlib>
#include <vector>
static long cases=0;
#define CHECK(x)do{if(!(x)){std::fprintf(stderr,"case %ld line %d: %s\n",cases,__LINE__,#x);std::exit(1);}}while(0)
static std::vector<int> trace;
enum EAIActionReturn{AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};enum EAIActionRepeat{ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2};enum EMovementType{AI_MOVEMENT_RUNNING=5};enum EMessageEventType{YELL=11};
class CThing{ public:mutable int refs;bool dead;unsigned long flags;CThing():refs(0),dead(false),flags(15){}bool IsDead()const{return dead;}unsigned long GetTypeFlags()const{return flags;}};
class CThingCreatureBase:public CThing{ public:static unsigned long StatGetTypeFlags(){return 15;}};class CThingPlayerCreature:public CThingCreatureBase{ public:};class CThingAICreature:public CThingCreatureBase{ public:float fov;CThingAICreature():fov(180){}float GetFOV()const{trace.push_back(25);return fov;}};
template<class T>class CIntelligentPointer{ public:T*p;CIntelligentPointer(T*q=0):p(q){if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;}template<class U>CIntelligentPointer(CIntelligentPointer<U>&o):p(o.Get()){if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;}CIntelligentPointer&operator=(const CIntelligentPointer&o){if(o.p)++o.p->refs;if(p)--p->refs;p=o.p;return*this;}T*Get()const{return p;}T*operator->()const{return p;}};
template<class T>bool operator!=(const CIntelligentPointer<T>&p,int){return p.Get()!=0;}template<class T>bool operator==(const CIntelligentPointer<T>&p,int){return p.Get()==0;}
template<class T>bool IS_THING_DEAD(const T&p){return p==0||p->IsDead();}template<class T>bool IS_THING_ALIVE(const T&p){return p!=0&&!p->IsDead();}
template<class T,class U>U*GFDebugPDynamicCast(T*p,U*){return static_cast<U*>(p);}
class CMessageEvent{ public:CIntelligentPointer<CThing>POriginator;template<class T>T*GetPOriginator()const{T*ret=0;if(POriginator!=0){unsigned long things_flags=POriginator->GetTypeFlags();unsigned long cast_to_flags=T::StatGetTypeFlags();unsigned long xored=things_flags^cast_to_flags;if(!(xored&cast_to_flags))ret=GFDebugPDynamicCast(POriginator.Get(),(T*)0);}return ret;}};
class CMessageEventFilter_IsOfType{ public:EMessageEventType Type;CMessageEventFilter_IsOfType(EMessageEventType t):Type(t){}};class CMessageEventFilter_IsOriginatorAlive{ public:};class CMessageEventFilter_CanBeSeenOrHeard{ public:const CThingAICreature&Creature;CMessageEventFilter_CanBeSeenOrHeard(const CThingAICreature&c):Creature(c){}};
template<class A,class B,class C>class CFilter_Bind3{ public:const A&F1;const B&F2;const C&F3;CFilter_Bind3(const A&a,const B&b,const C&c):F1(a),F2(b),F3(c){}};template<class A,class B,class C>CFilter_Bind3<A,B,C>GFFilter_Bind3(const A&a,const B&b,const C&c){return CFilter_Bind3<A,B,C>(a,b,c);}
static CThingAICreature*expected_guard;static CThingCreatureBase*expected_target;static CThingPlayerCreature*expected_hero;
class CMessageEventManager{ public:CMessageEvent*event;bool removed;CMessageEventManager():event(0),removed(false){}template<class F>const CMessageEvent*FindMostRecentMessage(const F&f)const{trace.push_back(10);CHECK(f.F1.Type==YELL&&&f.F3.Creature==expected_guard);return event;}void RemoveMessage(const CMessageEvent*e){trace.push_back(20);CHECK(e==event&&!removed);if(expected_target)CHECK(expected_target->refs==2);const_cast<CMessageEvent*>(e)->POriginator=CIntelligentPointer<CThing>();removed=true;}};
class CWorld{ public:CMessageEventManager manager;CMessageEventManager&GetMessageEventManager(){return manager;}const CMessageEventManager&GetMessageEventManager()const{return manager;}};
class CPlayer{ public:CThingPlayerCreature*hero;CThingPlayerCreature*GetPControlledCreature()const{trace.push_back(50);return hero;}};class CPlayerManager{ public:CPlayer player;CPlayer&GetMainPlayer(){return player;}};
class CCreatureActionBase{ public:virtual~CCreatureActionBase(){}};class CCreatureAction_DeclarePlayerACriminal:public CCreatureActionBase{ public:CThingCreatureBase&guard;CThingPlayerCreature&hero;CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase&g,CThingPlayerCreature&h):guard(g),hero(h){CHECK(&g==expected_guard&&&h==expected_hero);trace.push_back(60);}~CCreatureAction_DeclarePlayerACriminal(){trace.push_back(80);}};
struct CAIStateGroup_MinionGuardHeardYellForHelp{CThingAICreature&guard;CWorld&world;CPlayerManager&players;int look_result,repeat_count,terminal,die_at,move_calls,terminations;CAIStateGroup_MinionGuardHeardYellForHelp(CThingAICreature&g,CWorld&w,CPlayerManager&p):guard(g),world(w),players(p),look_result(2),repeat_count(0),terminal(2),die_at(-2),move_calls(0),terminations(0){}CThingAICreature&GetCreature()const{return guard;}const CWorld&PeekWorld()const{return world;}CWorld&GetWorld()const{return world;}CPlayerManager&GetPlayerManager(){return players;}void Terminate(){trace.push_back(90);++terminations;}EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,long frames,float speed){trace.push_back(30);CHECK(repeat==ACTION_REPEAT_UNTIL_FINISHED&&frames==35&&speed==guard.fov/360.0f);CHECK(thing.Get()==expected_target);if(expected_target){CHECK(thing->refs==2);if(die_at==-1)const_cast<CThing*>(thing.Get())->dead=true;}return(EAIActionReturn)look_result;}EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,float radius,bool&nav,EMovementType movement,bool a,bool b,bool c,bool d,float e){trace.push_back(40);CHECK(repeat==ACTION_REPEAT_CALLBACK&&thing.Get()==expected_target&&thing.Get()&&!thing->dead&&thing->refs==2);CHECK(radius==3&&movement==AI_MOVEMENT_RUNNING&&a&&!b&&c&&d&&e==0);CHECK(nav==(move_calls!=0));nav=true;if(move_calls==die_at)const_cast<CThing*>(thing.Get())->dead=true;int result=move_calls<repeat_count?4:terminal;++move_calls;return(EAIActionReturn)result;}EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&action,bool wait){CHECK(!wait);const CCreatureAction_DeclarePlayerACriminal&v=static_cast<const CCreatureAction_DeclarePlayerACriminal&>(action);CHECK(&v.guard==expected_guard&&&v.hero==expected_hero);trace.push_back(70);return AI_ACTION_FAILED;}const CMessageEvent*FindYellForHelpEvent();void Update();};

const CMessageEvent* CAIStateGroup_MinionGuardHeardYellForHelp::FindYellForHelpEvent()
{
 return PeekWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(CMessageEventFilter_IsOfType((EMessageEventType)11),CMessageEventFilter_IsOriginatorAlive(),CMessageEventFilter_CanBeSeenOrHeard(GetCreature())));
}
void CAIStateGroup_MinionGuardHeardYellForHelp::Update()
{
 const CMessageEvent* pevent=FindYellForHelpEvent();
 if(pevent) {
  CIntelligentPointer<const CThingCreatureBase> poriginator(pevent->GetPOriginator<CThingCreatureBase>());
  GetWorld().GetMessageEventManager().RemoveMessage(pevent);
  if(AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,poriginator,35,GetCreature().GetFOV()/360.0f)!=AI_ACTION_FINISHED) {
   Terminate();return;
  }
  EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
  bool nav_route_calculated=false;
  while(ret==AI_ACTION_IN_PROGRESS) {
   if(IS_THING_DEAD(poriginator)){Terminate();return;}
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,poriginator,3.0f,nav_route_calculated,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f);
  }
  if(ret!=AI_ACTION_FINISHED){Terminate();return;}
  CThingPlayerCreature* phero=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
  if(IS_THING_ALIVE(phero))AIAction_PerformActionAndWait(CCreatureAction_DeclarePlayerACriminal(GetCreature(),*phero),false);
 }
 Terminate();
}
int main(){
 for(int ev=0;ev<6;++ev)for(int look=0;look<5;++look)for(int repeats=0;repeats<4;++repeats)for(int terminal=0;terminal<4;++terminal)for(int death=-2;death<4;++death)for(int hero_state=0;hero_state<3;++hero_state)for(int fov=0;fov<3;++fov){
  ++cases;trace.clear();CThingAICreature guard;guard.fov=fov==0?90.0f:fov==1?180.0f:360.0f;CThingCreatureBase originator;originator.flags=ev==2?7:ev==5?31:15;originator.dead=ev==3;CThingPlayerCreature hero;hero.dead=hero_state==1;CMessageEvent event;CWorld world;CPlayerManager players;players.player.hero=hero_state?&hero:0;world.manager.event=ev?&event:0;event.POriginator=CIntelligentPointer<CThing>(ev>=2?&originator:0);
  expected_guard=&guard;expected_target=ev>=3?&originator:0;expected_hero=hero_state?&hero:0;
  CAIStateGroup_MinionGuardHeardYellForHelp state(guard,world,players);state.look_result=look;state.repeat_count=repeats;state.terminal=terminal;state.die_at=death;
  std::vector<int>wanted;wanted.push_back(10);bool dead=originator.dead;int moves=0;
  if(ev){wanted.push_back(20);wanted.push_back(25);wanted.push_back(30);if(expected_target&&death==-1)dead=true;
   if(look==2&&expected_target&&!dead){
    for(int k=0;k<=repeats;++k){if(dead)break;wanted.push_back(40);++moves;if(k==death)dead=true;if(k==repeats){if(terminal==2){wanted.push_back(50);if(hero_state==2){wanted.push_back(60);wanted.push_back(70);wanted.push_back(80);}}break;}}
   }
  }
  wanted.push_back(90);state.Update();CHECK(trace==wanted);CHECK(state.terminations==1&&state.move_calls==moves);CHECK(world.manager.removed==(ev!=0));CHECK(originator.refs==0&&originator.dead==dead);CHECK(event.POriginator.Get()==0);CHECK(guard.refs==0&&hero.refs==0);
 }
 CHECK(cases==25920);std::puts("00910df0_TEST PASS");return 0;
}