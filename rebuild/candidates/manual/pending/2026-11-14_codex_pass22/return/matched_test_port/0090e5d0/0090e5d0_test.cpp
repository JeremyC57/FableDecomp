#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __declspec(x)
#endif
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CTHINGFILTER_IN2DRANGE_CTOR
#define FABLE_RETAIL_CTHINGFILTER_ISVALIDGOSSIPEE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCSOLIDTOTHING_ORIGINAL_MEMBERS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/ETCInterfaceType.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CBaseObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
struct TestState {
 int frames,fps,arrival,waitTerminate,waiting,moveProgress,moveFinal,lookResult,sheatheResult,unsheatheResult,interactionResult;
 int killRemove,killMove,killSheathe,initialPartner,initialDead,wantIncoming,carrying,initialSolid,randomSeed;
 int tokenNew,tokenDelete,baseNew,baseDelete,strings,requests,removed,waitCalls,looks,moves,sheathed,unsheathed,interacted,solidAdds,solidRemoved,constructed,destroyed,randomCalls;
 int kindTests,rangeTests,validTests,stage;
 bool preparing,terminated,solidCtorSeen,solidNameAlive,missingSolid;
};
static TestState state;
struct Context;static Context*Current;
class CBaseObjectPointer {public:CBaseObject*Owner;CBaseObjectPointer(CBaseObject*p):Owner(p){}};
static std::vector<CBaseObjectPointer*> Tokens;
static void Invalidate(CBaseObject*p){for(unsigned n=0;n<Tokens.size();++n)if(Tokens[n]->Owner==p)Tokens[n]->Owner=0;}
CBaseObject::~CBaseObject(){Invalidate(this);}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++state.baseNew;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);++state.baseDelete;}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return PData?PData->Owner:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){if(PData){bool found=false;for(unsigned n=0;n<Tokens.size();++n)if(Tokens[n]==PData){Tokens.erase(Tokens.begin()+n);found=true;break;}assert(found);delete PData;PData=0;++state.tokenDelete;}if(p){PData=new CBaseObjectPointer(p);Tokens.push_back(PData);++state.tokenNew;}}
class CCharString {
public:const char*Text;
 CCharString(const char*s,long n=-1):Text(s){assert(n==-1);++state.strings;if(strcmp(s,"CTCSolidToThing")==0){assert(state.missingSolid&&!state.solidCtorSeen);state.solidCtorSeen=true;state.solidNameAlive=true;}}
 CCharString(const CCharString&s):Text(s.Text){++state.strings;}
 ~CCharString(){assert(state.strings>0);--state.strings;if(strcmp(Text,"CTCSolidToThing")==0)state.solidNameAlive=false;}
};
class CTCBase;class CTCGossip;class CTCSolidToThing;
class CThing:public CBaseObject {
public:int Identity;unsigned long Flags;bool Dead,Valid;C3DVector Pos;CTCGossip*Gossip;CTCSolidToThing*Solid;
 CThing():Identity(0),Flags(31),Dead(false),Valid(true),Gossip(0),Solid(0){Pos.X=Pos.Y=Pos.Z=0.0f;}
 unsigned long GetTypeFlags()const{return Flags;}
 bool IsDead()const{return Dead;}
 const C3DVector&GetPos()const{return Pos;}
 template<class T>bool GetTC(T**);
 CTCBase*AddTC(const CCharString&,bool,const void*);
 void RemoveTC(ETCInterfaceType);
};
class CThingPhysical:public CThing{public:const C3DVector&PeekPos()const{return Pos;}};
class CThingCreatureBase:public CThingPhysical{};
class CThingAICreature:public CThingCreatureBase{public:static unsigned long StatGetTypeFlags(){return 31;}};
template<class S,class D>inline D GFDebugPDynamicCast(S p,D){return static_cast<D>(p);}
enum EReactionSpeechType { FixtureReactionZero=0 };
#include "engine/EMessageEventType.h"
#include "engine/CMessageEvent.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CThingFilter_IsValidGossipee.h"
#include "engine/CMessageEventManager.h"
#include "engine/CTCGossip.h"
#include "engine/CTCSolidToThing.h"
#include "engine/GFRandom.h"
class CWorld {public:CMessageEventManager*Manager;CMessageEventManager&GetMessageEventManager(){return *Manager;}static long FABLE_FASTCALL GetFrame(){return state.frames;}};
class CMainGameComponent {public:static long FABLE_FASTCALL GetConstantFPS(){return state.fps;}};
class CCreatureActionBase {public:int Kind;CCreatureActionBase(int k):Kind(k){++state.constructed;}virtual~CCreatureActionBase(){++state.destroyed;}};
class CCreatureAction_SheatheWeapons:public CCreatureActionBase{public:CCreatureAction_SheatheWeapons(CThingCreatureBase&);};
class CCreatureAction_UnsheatheWeapons:public CCreatureActionBase{public:CCreatureAction_UnsheatheWeapons(CThingCreatureBase&);};
class CTCCarrying {public:static bool IsThingCarryingWeapon(const CThing&);};
class CAIStateGroup_BanditGossip {
public:bool WasCarryingWeapon,Critical;
 CThingAICreature&GetCreature()const;
 CWorld&GetWorld()const;
 unsigned long*GetPWorldSeed()const;
 void Terminate(){assert(!state.terminated);state.terminated=true;}
 EAIActionReturn AIAction_DoNothing();
 EAIActionReturn AIAction_LookAtThing(EAIActionRepeat,CIntelligentPointer<const CThing>,long,float);
 EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat,CIntelligentPointer<const CThing>,float,bool&,EMovementType,bool,bool,bool,bool,float);
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&,bool);
 EAIActionReturn AIAction_InteractWith(EAIActionRepeat,CIntelligentPointer<CThingAICreature>,CCharString,int,bool);
 void Update();
};
struct Context {
 CThingAICreature Me,Other,Far,Wrong,Invalid;
 CWorld World;
 CMessageEventManager Manager;
 CTCGossip SelfGossip,OtherGossip;
 CTCSolidToThing*Solid;
 CAIStateGroup_BanditGossip Script;
 unsigned long Seed;
 Context():Manager(World),SelfGossip(Me),OtherGossip(Other),Solid(0),Seed((unsigned long)state.randomSeed){World.Manager=&Manager;Me.Identity=1;Other.Identity=2;Far.Identity=3;Wrong.Identity=4;Invalid.Identity=5;Me.Gossip=&SelfGossip;Other.Gossip=&OtherGossip;Far.Gossip=&OtherGossip;Wrong.Gossip=&OtherGossip;Invalid.Gossip=&OtherGossip;Far.Pos.X=32.0f;Other.Pos.X=4.0f;Wrong.Flags=0;Invalid.Valid=false;Other.Dead=state.initialDead!=0;SelfGossip.MyTurnToSpeak=false;OtherGossip.MyTurnToSpeak=true;Script.WasCarryingWeapon=true;Script.Critical=true;if(state.initialSolid){Solid=new CTCSolidToThing;Me.Solid=Solid;}if(state.initialPartner)SelfGossip.Gossipee=&Other;}
 ~Context(){delete Solid;}
};
CMessageEventManager::CMessageEventManager(const CWorld&w):World(w){}
CTCGossip::CTCGossip(CThing&t):CTCBase(t),MyTurnToSpeak(false){}
ETCInterfaceType CTCGossip::GetInterfaceType()const{return TCI_GOSSIP;}
CCharString CTCGossip::GetClassTypeName()const{return CCharString("CTCGossip");}
CMessageEvent*CMessageEventManager::AddMessage(EMessageEventType type,unsigned long life,const C3DVector&pos,CThing*originator,CThing*target,float sight,float hearing,long data){
 if(!state.preparing){assert(!state.terminated&&type==MESSAGE_EVENT_REQUEST_GOSSIP&&life==(unsigned long)(state.fps*4)&&originator==&Current->Me&&!target&&pos.X==0.0f&&pos.Y==0.0f&&pos.Z==0.0f&&sight==16.0f&&hearing==16.0f&&data==0);++state.requests;state.waiting=1;}
 CMessageEvent event;event.Type=type;event.Pos=pos;event.SightRadius=sight;event.HearingRadius=hearing;event.OriginatorUID=event.TargetUID=0;event.PTargetFaction=0;event.TimeStamp=state.frames;event.Lifespan=life;event.ExtraData=data;event.MessageID=0;event.ReactionSpeechType=FixtureReactionZero;event.POriginator=originator;event.PTarget=target;Messages.push_front(event);return &Messages.front();
}
void CMessageEventManager::RemoveMessage(const CMessageEvent*event){assert(!state.terminated&&event&&event->POriginator.Get()==&Current->Other);++state.removed;for(std::list<CMessageEvent>::iterator i=Messages.begin();i!=Messages.end();++i)if(&*i==event){Messages.erase(i);if(state.killRemove==1)Current->Other.Dead=true;else if(state.killRemove==2)Invalidate(&Current->Other);return;}assert(false);}
bool CMessageEventFilter_IsOfType::operator()(const CMessageEvent&event)const{assert(Type==MESSAGE_EVENT_REQUEST_GOSSIP);++state.kindTests;return event.Type==Type;}
bool CThingFilter_In2DRange::operator()(const CThing&thing)const{assert(Pos.X==0.0f&&Pos.Y==0.0f&&Pos.Z==0.0f&&Range==16.0f);++state.rangeTests;float x=Pos.X-thing.Pos.X,y=Pos.Y-thing.Pos.Y;return x*x+y*y<=Range*Range;}
CThingFilter_IsValidGossipee::CThingFilter_IsValidGossipee(const CThingAICreature&thing):Thing(thing),PFaction(0),PTCVillage(0){assert(&thing==&Current->Me);}
bool CThingFilter_IsValidGossipee::operator()(const CThingAICreature&thing)const{assert(&Thing==&Current->Me);++state.validTests;return &thing!=&Thing&&thing.Valid&&thing.Gossip;}
template<>bool CThing::GetTC<CTCGossip>(CTCGossip**p){if(Gossip){*p=Gossip;return true;}return false;}
template<>bool CThing::GetTC<CTCSolidToThing>(CTCSolidToThing**p){assert(this==&Current->Me);if(Solid){*p=Solid;return true;}state.missingSolid=true;return false;}
CTCBase*CThing::AddTC(const CCharString&name,bool flag,const void*param){assert(this==&Current->Me&&state.solidCtorSeen&&state.solidNameAlive&&strcmp(name.Text,"CTCSolidToThing")==0&&!flag&&!param&&!Solid);Current->Solid=new CTCSolidToThing;Solid=Current->Solid;return Solid;}
void CThing::RemoveTC(ETCInterfaceType type){assert(this==&Current->Me&&type==TCI_SOLID_TO_THING&&state.interacted==1&&Solid);++state.solidRemoved;delete Current->Solid;Current->Solid=0;Solid=0;}
void CTCSolidToThing::AddThingNotSolidTo(const CThing*thing){assert(this==Current->Me.Solid&&thing==&Current->Other&&!state.solidNameAlive);++state.solidAdds;NotSolidToThings.push_back(CIntelligentPointer<const CThing>(thing));}
CThingAICreature&CAIStateGroup_BanditGossip::GetCreature()const{if(state.missingSolid&&!Current->Me.Solid)assert(state.solidCtorSeen);return Current->Me;}
CWorld&CAIStateGroup_BanditGossip::GetWorld()const{return Current->World;}
unsigned long*CAIStateGroup_BanditGossip::GetPWorldSeed()const{assert(!state.terminated);return &Current->Seed;}
long FABLE_FASTCALL GFROR13(unsigned long value){unsigned long expected=(unsigned long)state.randomSeed*0x24a1ul+0x24dful;assert(value==expected&&state.randomSeed>=0&&state.randomSeed<=2&&!state.randomCalls);++state.randomCalls;unsigned int word=(unsigned int)value;unsigned int rotated=(word>>13)|(word<<19);assert(rotated<0x80000000u);return (long)rotated;}
EAIActionReturn CAIStateGroup_BanditGossip::AIAction_DoNothing(){assert(!state.terminated&&state.requests==1&&!state.looks);++state.waitCalls;++state.frames;if(state.arrival>=0&&state.frames>=state.arrival){Current->SelfGossip.Gossipee=&Current->Other;Current->Other.Dead=false;}return state.waitCalls==state.waitTerminate?AI_ACTION_TERMINATE_PROCESS:AI_ACTION_FINISHED;}
EAIActionReturn CAIStateGroup_BanditGossip::AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,long frames,float speed){assert(!state.terminated&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&thing.Get()==&Current->Other&&frames==17&&speed==0.0027777f&&!state.looks);++state.looks;return(EAIActionReturn)state.lookResult;}
EAIActionReturn CAIStateGroup_BanditGossip::AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,float distance,bool&nav,EMovementType movement,bool a,bool b,bool c,bool d,float last){assert(!state.terminated&&state.looks==1&&state.lookResult==AI_ACTION_FINISHED&&repeat==ACTION_REPEAT_CALLBACK&&thing.Get()==&Current->Other&&distance==2.0f&&movement==AI_MOVEMENT_WALKING&&a&&!b&&c&&d&&last==0.0f);nav=state.moves%2!=0;++state.moves;if(state.moves<=state.moveProgress)return AI_ACTION_IN_PROGRESS;if(state.killMove==1)Current->Other.Dead=true;else if(state.killMove==2)Invalidate(&Current->Other);return(EAIActionReturn)state.moveFinal;}
bool CTCCarrying::IsThingCarryingWeapon(const CThing&thing){assert(&thing==&Current->Me&&state.moves==state.moveProgress+1&&state.moveFinal==AI_ACTION_FINISHED);return state.carrying!=0;}
CCreatureAction_SheatheWeapons::CCreatureAction_SheatheWeapons(CThingCreatureBase&thing):CCreatureActionBase(1){assert(&thing==&Current->Me&&state.carrying&&!state.terminated);}
CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase&thing):CCreatureActionBase(2){assert(&thing==&Current->Me&&state.carrying&&state.interacted==1&&state.solidRemoved==1&&!state.terminated);}
EAIActionReturn CAIStateGroup_BanditGossip::AIAction_PerformActionAndWait(const CCreatureActionBase&action,bool wait){assert(!state.terminated&&!wait);if(action.Kind==1){++state.sheathed;if(state.killSheathe==1)Current->Other.Dead=true;else if(state.killSheathe==2)Invalidate(&Current->Other);return(EAIActionReturn)state.sheatheResult;}assert(action.Kind==2&&WasCarryingWeapon);++state.unsheathed;return(EAIActionReturn)state.unsheatheResult;}
EAIActionReturn CAIStateGroup_BanditGossip::AIAction_InteractWith(EAIActionRepeat repeat,CIntelligentPointer<CThingAICreature>other,CCharString interaction,int priority,bool submissive){assert(!state.terminated&&Critical&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&other.Get()==&Current->Other&&priority==100&&submissive==(state.removed==0)&&state.solidAdds==1&&state.randomCalls==1);static const char*names[]={"VILLAGER_INTERACTION_BANDIT_PUSHING","VILLAGER_INTERACTION_BANDIT_HOLDING","VILLAGER_INTERACTION_BANDIT_TELLING_OFF"};static const int choices[]={2,0,1};assert(strcmp(interaction.Text,names[choices[state.randomSeed]])==0);++state.interacted;return(EAIActionReturn)state.interactionResult;}
// Native IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >,007304C0..007304E4,36 bytes.
// Its original size compilation returns bool in AL; the complete real predicate stays visible for the hot wait-loop expansion.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline bool FABLE_FASTCALL IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >(const CIntelligentPointer<CThingAICreature>& thing)
{
    if(thing==0||thing->IsDead())return true;
    return false;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

void CAIStateGroup_BanditGossip::Update()
{
    WasCarryingWeapon=false;
    Critical=false;
    CIntelligentPointer<CThingAICreature> other_person=0;
    CTCGossip* gossip=0;
    CTCGossip* other_gossip=0;
    GetCreature().GetTC(&gossip);
    bool submissive;
    const CMessageEvent* event=GetWorld().GetMessageEventManager().FindMostRecentMessage(
        GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(),16.0f)),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature()))));
    if(event){
        other_person=event->GetPOriginator<CThingAICreature>();
        GetWorld().GetMessageEventManager().RemoveMessage(event);
        if(!IS_THING_ALIVE(other_person)){Terminate();return;}
        other_person->GetTC(&other_gossip);
        gossip->Gossipee=other_person;
        gossip->MyTurnToSpeak=true;
        other_gossip->Gossipee=&GetCreature();
        other_gossip->MyTurnToSpeak=false;
        submissive=false;
    }else{
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,CMainGameComponent::GetConstantFPS()*4,
            GetCreature().PeekPos(),&GetCreature(),0,16.0f,16.0f,0);
        long wait_till_frame=CWorld::GetFrame()+CMainGameComponent::GetConstantFPS()*4;
        while(IS_THING_DEAD(gossip->Gossipee)){
            if(AIAction_DoNothing()==AI_ACTION_TERMINATE_PROCESS||CWorld::GetFrame()>wait_till_frame){Terminate();return;}
        }
        other_person=gossip->Gossipee;
        other_person->GetTC(&other_gossip);
        submissive=true;
    }
    if(other_person.Get()==0){Terminate();return;}
    if(AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,other_person,17,0.0027777f)!=AI_ACTION_FINISHED){Terminate();return;}
    EAIActionReturn ret=AI_ACTION_FINISHED;
    bool nav_route_calculated;
    do{
        ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,other_person,2.0f,nav_route_calculated,AI_MOVEMENT_WALKING,true,false,true,true,0.0f);
    }while(ret==AI_ACTION_IN_PROGRESS);
    if(ret!=AI_ACTION_FINISHED){Terminate();return;}
    if(CTCCarrying::IsThingCarryingWeapon(GetCreature())){
        if(AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()),false)!=AI_ACTION_FINISHED){Terminate();return;}
        WasCarryingWeapon=true;
    }
    if(IS_THING_DEAD(other_person)){Terminate();return;}
    CTCSolidToThing* ptc_solid_to_thing=0;
    if(!GetCreature().GetTC(&ptc_solid_to_thing)){
        CCharString solid_name("CTCSolidToThing");
        ptc_solid_to_thing=static_cast<CTCSolidToThing*>(GetCreature().AddTC(solid_name,false,0));
    }
    ptc_solid_to_thing->AddThingNotSolidTo(other_person.Get());
    char* interactions[3]={"VILLAGER_INTERACTION_BANDIT_PUSHING","VILLAGER_INTERACTION_BANDIT_HOLDING","VILLAGER_INTERACTION_BANDIT_TELLING_OFF"};
    CCharString interaction(interactions[GFRandom(3,GetPWorldSeed())]);
    Critical=true;
    AIAction_InteractWith(ACTION_REPEAT_UNTIL_FINISHED,other_person,interaction,100,submissive);
    Critical=false;
    GetCreature().RemoveTC(TCI_SOLID_TO_THING);
    if(WasCarryingWeapon){
        if(AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()),false)!=AI_ACTION_FINISHED){Terminate();return;}
        WasCarryingWeapon=false;
    }
    Terminate();
}

template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class CompareType>class CVectorMap {public:typedef std::pair<K,V>value_type;typedef value_type*iterator;value_type Backing[33];int Count;iterator begin(){return Backing;}iterator end(){return Backing+Count;}iterator LowerBound(const K&);};
template<class K,class V,class CompareType>
__declspec(noinline) typename CVectorMap<K,V,CompareType>::iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}

int main(){unsigned cases=0;int coverage[10]={0,0,0,0,0,0,0,0,0,0};
 static const int results[]={2,3,1,0};
 for(int seed=0;seed<4352;++seed){assert(Tokens.empty());memset(&state,0,sizeof(state));state.frames=100;state.fps=5;state.wantIncoming=seed%2;state.killRemove=seed/2%3;state.initialPartner=seed/5%2;state.initialDead=seed/7%2;state.arrival=seed/9%3==0?120:seed/9%3==1?121:-1;state.waitTerminate=seed/13%3==0?1:seed/13%3==1?3:0;state.lookResult=seed/17%5;state.moveProgress=seed/19%3;state.moveFinal=results[seed/23%4];state.carrying=seed/29%2;state.sheatheResult=results[seed/31%4];state.unsheatheResult=results[seed/37%4];state.killMove=seed/41%3;state.killSheathe=seed/43%3;state.interactionResult=seed/47%5;state.initialSolid=seed/53%2;state.randomSeed=seed/59%3;
  if(seed>=4096){int edge=seed-4096;state.wantIncoming=edge%2;state.killRemove=0;state.initialPartner=1;state.initialDead=0;state.arrival=120;state.waitTerminate=0;state.lookResult=2;state.moveFinal=2;state.carrying=edge/2%2;state.sheatheResult=2;state.unsheatheResult=results[edge/4%4];state.killMove=0;state.killSheathe=0;state.randomSeed=edge/16%3;}
  {Context context;Current=&context;state.preparing=true;C3DVector zero;zero.X=zero.Y=zero.Z=0.0f;
   if(state.wantIncoming)context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Other,0,16,16,0);
   context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Invalid,0,16,16,0);
   context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Far,0,16,16,0);
   context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Wrong,0,16,16,0);
   context.Manager.AddMessage((EMessageEventType)0,20,zero,&context.Other,0,16,16,0);
   state.preparing=false;context.Script.Update();assert(state.terminated&&!context.Script.Critical&&state.strings==0&&state.constructed==state.destroyed);
   assert(state.removed==state.wantIncoming&&state.requests==1-state.wantIncoming&&state.kindTests>=4&&state.rangeTests>=2&&state.validTests>=1);
   if(state.removed&&(!state.initialDead&&!state.killRemove)){assert(context.SelfGossip.MyTurnToSpeak&&!context.OtherGossip.MyTurnToSpeak&&context.OtherGossip.Gossipee.Get()==&context.Me);++coverage[0];}
   if(state.removed&&(state.initialDead||state.killRemove)){assert(!state.looks&&!state.moves&&!state.interacted&&!context.Script.WasCarryingWeapon);++coverage[1];}
   if(!state.removed){if(state.looks){assert((state.initialPartner&&!state.initialDead)||state.arrival==120);assert(context.SelfGossip.Gossipee.Get()==&context.Other||state.killMove==2||state.killSheathe==2);++coverage[2];}else{assert(state.waitCalls>0&&state.frames<=121);++coverage[3];}}
   if(state.moves)assert(state.lookResult==AI_ACTION_FINISHED&&state.moves==state.moveProgress+1);
   if(state.interacted){assert(state.solidRemoved==1&&state.randomCalls==1);++coverage[4];if(state.carrying){assert(state.sheathed==1&&state.sheatheResult==AI_ACTION_FINISHED&&state.unsheathed==1);assert(context.Script.WasCarryingWeapon==(state.unsheatheResult!=AI_ACTION_FINISHED));++coverage[5];}else assert(!state.sheathed&&!state.unsheathed&&!context.Script.WasCarryingWeapon);if(!state.initialSolid)assert(state.solidCtorSeen);else assert(!state.solidCtorSeen);++coverage[7+state.randomSeed];}
   else{assert(!state.solidRemoved&&!state.randomCalls);if(state.sheathed&&state.sheatheResult==AI_ACTION_FINISHED)assert(context.Script.WasCarryingWeapon);else assert(!context.Script.WasCarryingWeapon);if(state.killMove==2||state.killSheathe==2)++coverage[6];}
  }
  assert(Tokens.empty()&&state.tokenNew==state.tokenDelete&&state.baseNew==state.baseDelete&&state.strings==0);++cases;
 }

 unsigned searches=0;for(int count=0;count<=32;++count){CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >map;map.Count=count;for(int n=0;n<count;++n){map.Backing[n].first=(ETCInterfaceType)(n/2*3);map.Backing[n].second=0;}for(int key=0;key<100;++key){ETCInterfaceType k=(ETCInterfaceType)key;int expected=0;while(expected<count&&map.Backing[expected].first<k)++expected;assert(map.LowerBound(k)==map.Backing+expected);++searches;}}assert(searches==3300u);
 memset(&state,0,sizeof(state));
 {CThingAICreature creature;CIntelligentPointer<CThingAICreature> empty=0;assert(IS_THING_DEAD(empty)&&!IS_THING_ALIVE(empty));CIntelligentPointer<CThingAICreature> first=&creature;assert(!IS_THING_DEAD(first)&&IS_THING_ALIVE(first));{const CIntelligentPointer<CThingAICreature>&reference=first;CIntelligentPointer<CThingAICreature> second(reference);assert(second.Get()==&creature&&first.Get()==&creature);creature.Dead=true;assert(IS_THING_DEAD(second)&&!IS_THING_ALIVE(second));creature.Dead=false;Invalidate(&creature);assert(!second.Get()&&!first.Get()&&IS_THING_DEAD(second));}assert(IS_THING_DEAD(first));}
 assert(Tokens.empty()&&state.tokenNew==state.tokenDelete&&state.baseNew==state.baseDelete);
 assert(cases==4352u);for(int n=0;n<10;++n)assert(coverage[n]>0);puts("0090e5d0_TEST PASS");return 0;}
