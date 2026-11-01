// Behavior-only fixture: exact target body, actual predicate/script objects.
// A separate event-state interpreter checks every termination boundary. Unused
// interfaces/layout tails are reduced; native dispatch/layout is not asserted.
#include <stdio.h>
#include <stdlib.h>
static int cases,trace[512],traceCount,termCount,termLimit,startFailures,starts,watchGoal,remindGoal,watches,reminds,trustFlag,filterCopies,filterDtors,liveFilter;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
static void event(int x){CHECK(traceCount<512);trace[traceCount++]=x;}
class CCPPointerInfo{public:unsigned long RefCount;};
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;CCountedPointer():Data(0),Info(0){}CCountedPointer(const CCountedPointer&o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}~CCountedPointer(){if(Info){CHECK(Info->RefCount>0);--Info->RefCount;}}};
class CScriptThing{public:CCountedPointer<CScriptThing>PImp;CScriptThing(){}CScriptThing(const CScriptThing&o):PImp(o.PImp){++filterCopies;}virtual~CScriptThing(){}};
class CScriptGameResourceObjectScriptedThingBase{public:bool Valid;CScriptGameResourceObjectScriptedThingBase():Valid(false){}bool IsValid()const{event(4);return Valid;}void Release(){CHECK(Valid);Valid=false;event(5);}};
enum EScriptAIPriority{};
namespace NScript{class CPredicateBase{public:virtual bool operator()()const=0;virtual CPredicateBase*Clone()const=0;virtual~CPredicateBase(){}};class CIsThingAlive:public CPredicateBase{public:CScriptThing Thing;CIsThingAlive(const CScriptThing&t):Thing(t){++liveFilter;}virtual bool operator()()const{return Thing.PImp.Data!=0;}virtual CPredicateBase*Clone()const{return 0;}virtual~CIsThingAlive(){CHECK(liveFilter==1);--liveFilter;++filterDtors;event(1);}};}
class CGameScriptInterfaceBase{public:virtual~CGameScriptInterfaceBase(){}virtual void NewScriptFrame()const=0;virtual bool StartScriptingEntity(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&,EScriptAIPriority)=0;};
static const CScriptThing*expectedMe;
class CEntityScriptBase{public:CGameScriptInterfaceBase*Game;CScriptThing Me;CEntityScriptBase(CGameScriptInterfaceBase&g,const CScriptThing&m):Game(&g),Me(m){}virtual~CEntityScriptBase(){}void SetPredicate(const NScript::CPredicateBase&p){CHECK(liveFilter==1&&p());const NScript::CIsThingAlive&f=static_cast<const NScript::CIsThingAlive&>(p);CHECK(f.Thing.PImp.Data==Me.PImp.Data&&f.Thing.PImp.Info==Me.PImp.Info);event(0);}bool IsActiveThreadTerminating()const{event(3);return ++termCount>=termLimit;}};
namespace NScript{class CV_MurderTwistFinalScript{public:enum EAssassinState{};EAssassinState AssassinState;bool DoesAssassinTrustHero(){event(7);return trustFlag!=0;}class CMurderTwistAssassin;};
class CV_MurderTwistFinalScript::CMurderTwistAssassin:public CEntityScriptBase{public:CV_MurderTwistFinalScript&ParentClass;CScriptGameResourceObjectScriptedThingBase MyEntityHandle;CMurderTwistAssassin(CV_MurderTwistFinalScript&p,CGameScriptInterfaceBase&g,const CScriptThing&m):CEntityScriptBase(g,m),ParentClass(p){}void DoWatchingForTrouble(){event(8);if(++watches>=watchGoal)ParentClass.AssassinState=(CV_MurderTwistFinalScript::EAssassinState)3;}void DoRemindingHeroToKillSheriff(){event(9);if(++reminds>=remindGoal)ParentClass.AssassinState=(CV_MurderTwistFinalScript::EAssassinState)2;}void AttackHero(){event(10);ParentClass.AssassinState=(CV_MurderTwistFinalScript::EAssassinState)4;}virtual void Main();};
void CV_MurderTwistFinalScript::CMurderTwistAssassin::Main(){
 {CIsThingAlive filter(Me);SetPredicate(filter);}
 Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 if(MyEntityHandle.IsValid())MyEntityHandle.Release();
 while(!Game->StartScriptingEntity(Me,MyEntityHandle,(EScriptAIPriority)4)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(IsActiveThreadTerminating())return;
 bool trusts=ParentClass.DoesAssassinTrustHero();
 while(ParentClass.AssassinState==0||ParentClass.AssassinState==1){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;if(IsActiveThreadTerminating())return;DoWatchingForTrouble();}
 if(IsActiveThreadTerminating())return;
 while(ParentClass.AssassinState==3){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;DoRemindingHeroToKillSheriff();}
 if(IsActiveThreadTerminating())return;
 if(ParentClass.AssassinState==2){if(IsActiveThreadTerminating())return;AttackHero();}
 if(IsActiveThreadTerminating())return;
 do{Game->NewScriptFrame();}while(!IsActiveThreadTerminating());
}
}
class CTestGame:public CGameScriptInterfaceBase{public:void NewScriptFrame()const{CHECK(!liveFilter);event(2);}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&h,EScriptAIPriority p){CHECK(&m==expectedMe&&(int)p==4);event(6);bool success=starts++>=startFailures;if(success)h.Valid=true;return success;}};
static int expectedTrace[512],expectedCount;static void expectedEvent(int x){CHECK(expectedCount<512);expectedTrace[expectedCount++]=x;}
// Interpreter phases: setup, acquisition, watch, reminder, attack, idle.
static int model(int initial,bool own){int phase=0,state=initial,terms=0,tries=0,watch=0,remind=0;expectedCount=0;
 for(;;){switch(phase){
 case 0:expectedEvent(0);expectedEvent(1);expectedEvent(2);expectedEvent(3);if(++terms>=termLimit)return state;expectedEvent(4);if(own)expectedEvent(5);phase=1;break;
 case 1:expectedEvent(6);if(tries++<startFailures){expectedEvent(2);expectedEvent(3);if(++terms>=termLimit)return state;}else phase=2;break;
 case 2:expectedEvent(3);if(++terms>=termLimit)return state;expectedEvent(7);phase=3;break;
 case 3:if(state==0||state==1){expectedEvent(2);expectedEvent(3);if(++terms>=termLimit)return state;expectedEvent(3);if(++terms>=termLimit)return state;expectedEvent(8);if(++watch>=watchGoal)state=3;}else{expectedEvent(3);if(++terms>=termLimit)return state;phase=4;}break;
 case 4:if(state==3){expectedEvent(2);expectedEvent(3);if(++terms>=termLimit)return state;expectedEvent(9);if(++remind>=remindGoal)state=2;}else{expectedEvent(3);if(++terms>=termLimit)return state;phase=5;}break;
 case 5:if(state==2){expectedEvent(3);if(++terms>=termLimit)return state;expectedEvent(10);state=4;}expectedEvent(3);if(++terms>=termLimit)return state;phase=6;break;
 case 6:expectedEvent(2);expectedEvent(3);if(++terms>=termLimit)return state;break;
 default:CHECK(false);
 }}
}
int main(){for(int initial=0;initial<5;++initial)for(int failures=0;failures<3;++failures)for(int w=1;w<=2;++w)for(int r=1;r<=2;++r)for(int trust=0;trust<2;++trust)for(int own=0;own<2;++own)for(int limit=1;limit<=17;++limit){
 termLimit=limit==17?40:limit;startFailures=failures;watchGoal=w;remindGoal=r;trustFlag=trust;int expectedState=model(initial,own!=0);
 NScript::CV_MurderTwistFinalScript parent;parent.AssassinState=(NScript::CV_MurderTwistFinalScript::EAssassinState)initial;CTestGame game;CScriptThing me,implementation;CCPPointerInfo info;info.RefCount=2;me.PImp.Data=&implementation;me.PImp.Info=&info;
 {NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin assassin(parent,game,me);assassin.MyEntityHandle.Valid=own!=0;expectedMe=&assassin.Me;traceCount=termCount=starts=watches=reminds=filterCopies=filterDtors=liveFilter=0;assassin.Main();CHECK(traceCount==expectedCount);for(int x=0;x<traceCount;++x)CHECK(trace[x]==expectedTrace[x]);CHECK((int)parent.AssassinState==expectedState&&filterCopies==1&&filterDtors==1&&liveFilter==0&&info.RefCount==3);}
 CHECK(info.RefCount==2);++cases;
 }
 CHECK(cases==4080);puts("00eb3f40_TEST PASS");return 0;}
