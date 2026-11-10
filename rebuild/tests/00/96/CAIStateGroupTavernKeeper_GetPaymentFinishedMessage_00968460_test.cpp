// Isolated original message/search callback fixture; selected method body follows unchanged.
#include <cstdio>
#include <cassert>
#include <list>
#include <functional>
#define FABLE_FASTCALL
#ifdef _MSC_VER
typedef unsigned __int64 ThingUID;
#else
typedef unsigned long long ThingUID;
#endif
enum EAIActionReturn {AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};
enum EAIActionRepeat {ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2};
enum EMessageEventType {MESSAGE_EVENT_PAYMENT_REQUEST=48,MESSAGE_EVENT_PAYMENT_CANCELLED=49};
class CThing {public:ThingUID UniqueID;bool Dead;bool IsDead()const{return Dead;}ThingUID GetUniqueID()const{return UniqueID;}};
class CThingAICreature:public CThing {};
template<class T> class CIntelligentPointer {public:T* p;mutable int reads;CIntelligentPointer():p(0),reads(0){}T* Get()const{++reads;return p;}T* operator->()const{return Get();}T& operator*()const{return *Get();}};
template<class T>bool operator!=(const CIntelligentPointer<T>& p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CMessageEvent {public:EMessageEventType Type;ThingUID OriginatorUID,TargetUID;int MessageID;};
class CMessageEventFilter_IsOfType:public std::unary_function<CMessageEvent,bool> {EMessageEventType Type;public:CMessageEventFilter_IsOfType(EMessageEventType t):Type(t){}bool operator()(const CMessageEvent& e)const{return Type==e.Type;}};
class CMessageEventFilter_IsTargettedAtThing:public std::unary_function<CMessageEvent,bool> {ThingUID UID;public:CMessageEventFilter_IsTargettedAtThing(const CThing& t):UID(t.GetUniqueID()){}bool operator()(const CMessageEvent& e)const{return UID==e.TargetUID;}};
class CMessageEventFilter_IsOriginatedByThing:public std::unary_function<CMessageEvent,bool> {ThingUID UID;public:CMessageEventFilter_IsOriginatedByThing(const CThing& t):UID(t.GetUniqueID()){}bool operator()(const CMessageEvent& e)const{return UID==e.OriginatorUID;}};
#ifndef FABLE_ENGINE_CFILTER_BIND3_H
#define FABLE_ENGINE_CFILTER_BIND3_H
// Original PDB fields are protected const references, with no functor copies.
template<class TF1,class TF2,class TF3> class CFilter_Bind3 {
protected:
 const TF1& F1;
 const TF2& F2;
 const TF3& F3;
public:
 CFilter_Bind3(const TF1& f1,const TF2& f2,const TF3& f3):F1(f1),F2(f2),F3(f3){}
 template<class T> bool operator()(const T& t)const{return F1(t)&&F2(t)&&F3(t);}
};
#if 1
// Original FableWin02c4bd5e/02c4bdb7 call the named factory, with three const references.
template<class A,class B,class C> inline CFilter_Bind3<A,B,C> FABLE_FASTCALL GFFilter_Bind3(const A& a,const B& b,const C& c){return CFilter_Bind3<A,B,C>(a,b,c);}
#endif
#endif

class CMessageEventManager {public:
 std::list<CMessageEvent> Messages;int removals;mutable int finds;CMessageEventManager():removals(0),finds(0){}
 template<class Filter>const CMessageEvent* FindMostRecentMessage(const Filter& filter)const{++finds;for(std::list<CMessageEvent>::const_iterator i=Messages.begin();i!=Messages.end();++i)if(filter(*i))return &*i;return 0;}
 void RemoveMessage(const CMessageEvent* e){for(std::list<CMessageEvent>::iterator i=Messages.begin();i!=Messages.end();++i)if(&*i==e){Messages.erase(i);++removals;return;}assert(false);}
};
class CWorld {public:CMessageEventManager manager;CMessageEventManager& GetMessageEventManager(){return manager;}const CMessageEventManager& GetMessageEventManager()const{return manager;}};
class CAIBrain {public:bool stopping;bool stopOnSuspend;int suspensions;CAIBrain():stopping(false),stopOnSuspend(false),suspensions(0){}bool SuspendSuspendableProcess(){++suspensions;if(stopOnSuspend)stopping=true;return true;}};
class CAIStateGroupBase {public:CAIBrain* PBrain;CThingAICreature* creature;CWorld* world;
 CThingAICreature& GetCreature()const{return *creature;}CWorld& GetWorld()const{return *world;}const CWorld& PeekWorld()const{return *world;}bool IsStopping()const{return PBrain->stopping;}
 template<class Filter>EAIActionReturn AIAction_WaitForMessageCallback(EAIActionRepeat,const Filter&,const CMessageEvent**);
};
class CAIStateGroup_TavernKeeper:public CAIStateGroupBase {public:CIntelligentPointer<CThingAICreature> PPayee;bool GetPaymentFinishedMessage();};
// Original donor02bc5110 and native0091a670: the real out-of-line callback loop.
template<class Filter>
#ifdef _MSC_VER
__declspec(noinline)
#endif
EAIActionReturn CAIStateGroupBase::AIAction_WaitForMessageCallback(EAIActionRepeat repeat,const Filter& filter,const CMessageEvent** ppevent)
{
 if(IsStopping())return AI_ACTION_TERMINATE_PROCESS;
 EAIActionReturn result=AI_ACTION_IN_PROGRESS;
 do{
  *ppevent=PeekWorld().GetMessageEventManager().FindMostRecentMessage(filter);
  if(*ppevent){result=AI_ACTION_FINISHED;break;}
  PBrain->SuspendSuspendableProcess();
  if(IsStopping()){result=AI_ACTION_TERMINATE_PROCESS;break;}
  result=AI_ACTION_IN_PROGRESS;
 }while(repeat==ACTION_REPEAT_UNTIL_FINISHED && result==AI_ACTION_IN_PROGRESS);
 return result;
}
bool CAIStateGroup_TavernKeeper::GetPaymentFinishedMessage()
{
 const CMessageEvent* pevent=0;
 if(IS_THING_ALIVE(PPayee)){
  if(AIAction_WaitForMessageCallback(ACTION_REPEAT_CALLBACK,
    GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_PAYMENT_REQUEST),
      CMessageEventFilter_IsOriginatedByThing(*PPayee),CMessageEventFilter_IsTargettedAtThing(GetCreature())),&pevent)==AI_ACTION_FINISHED){
   GetWorld().GetMessageEventManager().RemoveMessage(pevent);
   return true;
  }
 }
 return false;
}

int main(){
 for(int live=0;live<3;++live)for(int stop=0;stop<3;++stop)for(int mask=0;mask<16;++mask)for(int high=0;high<2;++high){
  CWorld world;CAIBrain brain;CThingAICreature self,payee;CAIStateGroup_TavernKeeper keeper;
  ThingUID highWord=ThingUID(high)*65536*65536;self.UniqueID=highWord+19;payee.UniqueID=highWord+23;self.Dead=false;payee.Dead=live==2;
  keeper.creature=&self;keeper.PPayee.p=live==0?0:&payee;keeper.world=&world;keeper.PBrain=&brain;brain.stopping=stop==1;brain.stopOnSuspend=stop==2;
  int first=-1;
  for(int i=0;i<4;++i){CMessageEvent e;e.Type=(mask&(1<<i))?MESSAGE_EVENT_PAYMENT_REQUEST:MESSAGE_EVENT_PAYMENT_CANCELLED;e.TargetUID=(i==1)?self.UniqueID+1:self.UniqueID;e.OriginatorUID=(i==2)?payee.UniqueID+1:payee.UniqueID;e.MessageID=i;world.manager.Messages.push_back(e);if(first<0&&e.Type==MESSAGE_EVENT_PAYMENT_REQUEST&&e.TargetUID==self.UniqueID&&e.OriginatorUID==payee.UniqueID)first=i;}
  bool expected=live==1&&stop!=1&&first>=0;bool got=keeper.GetPaymentFinishedMessage();assert(got==expected);
  assert(world.manager.removals==(expected?1:0));assert(world.manager.Messages.size()==(unsigned int)(expected?3:4));
  if(expected){for(std::list<CMessageEvent>::const_iterator i=world.manager.Messages.begin();i!=world.manager.Messages.end();++i)assert(i->MessageID!=first);}
  assert(world.manager.finds==(live==1&&stop!=1?1:0));assert(brain.suspensions==(live==1&&stop!=1&&first<0?1:0));assert(keeper.PPayee.reads==(live==0?1:live==2?2:3));
 }
 std::puts("00968460_TEST PASS");return 0;
}