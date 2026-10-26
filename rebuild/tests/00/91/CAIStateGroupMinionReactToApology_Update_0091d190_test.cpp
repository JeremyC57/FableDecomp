#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTCENTITYEVENTS_H
#define FABLE_ENGINE_CPLAYERMANAGER_H
#define FABLE_ENGINE_CPLAYER_H
class CThingAICreature;
class CAIBrain { public: CThingAICreature* Creature; };
class CAIStateGroupBase { public: CAIBrain* PBrain; };
class CTCEntityEvents { public:};
class CPlayerManager { public:};
class CPlayer { public:};
#endif
// BEGIN EXACT CANDIDATE
// CAIStateGroup_MinionReactToApology::Update() -- speak accepted or rejected apology response then terminate
// Retail0x0091D190,270 bytes. GetApology91D0A0; player manager8FCF90,main player449970,hero487DC0;
// RTTI pointers1237f1c/123e7d8 slot0 deleting dtors48a4f0/4d6915, genuine virtual base destructor.
// intelligent pointer ctor/set A01B10/A01B90; speech981EA0; terminate8FCF40.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
class CCharString;
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*); };
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        bool present=(bits[type>>5]>>(type&31))&1;
        if(present) {
            long key=type; CInterfaceMap* map=&interfaces; CInterfaceMap::Entry* it=map->LowerBound(&key);
            *out=(T*)((it==map->last || it->type>type)?map->last:it)->component;
        } return present;
    }
};


struct CEventApologyReceived { void* vtable; long WorldFrame; bool Accepted; };
struct CEventFilter_None { CEventFilter_None() {} };
struct CTCEntityEventsMethods : CTCEntityEvents { bool GetApology(const CEventApologyReceived**,const CEventFilter_None&);
 const CEventApologyReceived* GetApology() { const CEventApologyReceived* event=0; GetApology(&event,CEventFilter_None()); return event; } };
struct CPlayerMethods : CPlayer { CThing* GetHeroThing(); };
struct CPlayerManagerMethods : CPlayerManager { CPlayerMethods* GetMainPlayer(); };
struct CBaseIntelligentPointer { void* Handle; CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer(); void SetPItem(CThing*); CThing* GetPItem() const; };
struct CIntelligentConstThing : CBaseIntelligentPointer { CIntelligentConstThing(CThing* p) { SetPItem(p); } CIntelligentConstThing(const CIntelligentConstThing& p) { SetPItem(p.GetPItem()); } };
struct CIntelligentCreature : CBaseIntelligentPointer { CIntelligentCreature(CThing* p) { SetPItem(p); } CIntelligentCreature(const CIntelligentCreature& p) { SetPItem(p.GetPItem()); } };
extern CCharString g_ApologyAccepted; // 0x13BB684
extern CCharString g_ApologyRejected; // 0x13BB688
struct CAIStateGroup_MinionReactToApology : CAIStateGroupBase {
 CPlayerManagerMethods* GetPPlayerManager(); void Terminate();
 long AIAction_SpeakAISpeechToTarget(long,CIntelligentCreature,CIntelligentConstThing,const CCharString&,bool,long);
 void Update();
};
void CAIStateGroup_MinionReactToApology::Update() {
 CTCEntityEventsMethods* events;
 bool hasEvents=((CThing*)PBrain->Creature)->GetInterface(&events,0x42);
 if(hasEvents) {
  const CEventApologyReceived* apology=events->GetApology();
  if(apology) {
   CThing* hero=GetPPlayerManager()->GetMainPlayer()->GetHeroThing();
   if(apology->Accepted) AIAction_SpeakAISpeechToTarget(1,(CThing*)PBrain->Creature,hero,g_ApologyAccepted,true,0x93);
   else AIAction_SpeakAISpeechToTarget(1,(CThing*)PBrain->Creature,hero,g_ApologyRejected,true,0x93);
  }
 }
 Terminate();
}
// END EXACT CANDIDATE
#ifdef _MSC_VER
#include "fable_string.h"
CCharString::CCharString():storage_(0) {}
CCharString::~CCharString() {}
#else
class CCharString { public: void* storage_; };
#endif
CCharString g_ApologyAccepted,g_ApologyRejected;
static int errors,lookups,eventCalls,players,speeches,terms,ctorCalls,dtorCalls,scenario;
static CThing creature,hero;
static CEventApologyReceived apology;
static CTCEntityEventsMethods events;
static CPlayerManagerMethods manager;
static CPlayerMethods player;
static void Check(bool b) { if(!b) ++errors; }
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* type) { ++lookups; Check(*type==0x42); return scenario==4?last:first; }
bool CTCEntityEventsMethods::GetApology(const CEventApologyReceived** out,const CEventFilter_None&) { ++eventCalls; Check(this==&events && *out==0); if(scenario!=1) *out=&apology; return scenario!=1; }
CPlayerManagerMethods* CAIStateGroup_MinionReactToApology::GetPPlayerManager() { ++players; return &manager; }
CPlayerMethods* CPlayerManagerMethods::GetMainPlayer() { Check(this==&manager); return &player; }
CThing* CPlayerMethods::GetHeroThing() { Check(this==&player); return &hero; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { ++ctorCalls; Handle=0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++dtorCalls; }

void CBaseIntelligentPointer::SetPItem(CThing* p) { Handle=p; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)Handle; }
long CAIStateGroup_MinionReactToApology::AIAction_SpeakAISpeechToTarget(long repeat,CIntelligentCreature speaker,CIntelligentConstThing target,const CCharString& speech,bool interrupt,long priority) {
 ++speeches; Check(repeat==1 && speaker.Handle==&creature && target.Handle==&hero && interrupt && priority==0x93);
 Check(&speech==(apology.Accepted?&g_ApologyAccepted:&g_ApologyRejected)); return 4;
}
void CAIStateGroup_MinionReactToApology::Terminate() { ++terms; }
int main() {
 CAIStateGroup_MinionReactToApology state; CAIBrain brain; state.PBrain=&brain; brain.Creature=(CThingAICreature*)&creature;
 CInterfaceMap::Entry entry[2]; entry[0].type=0x42; entry[0].component=&events; entry[1].component=&events;
 creature.interfaces.first=entry; creature.interfaces.last=entry+1;
 for(scenario=0;scenario<6;++scenario) {
  for(int i=0;i<9;++i) creature.bits[i]=0;
  if(scenario) creature.bits[0x42>>5]=1UL<<(0x42&31);
  apology.Accepted=(scenario%2)!=0; entry[0].type=scenario==5?0x43:0x42;
  lookups=eventCalls=players=speeches=terms=ctorCalls=dtorCalls=0;
  state.Update(); Check(terms==1); Check(lookups==(scenario!=0) && eventCalls==(scenario!=0));
  Check(players==(scenario>1) && speeches==(scenario>1)); Check(ctorCalls==dtorCalls);
  if(scenario>1) Check(ctorCalls>=2);
 }
 if(errors) { std::printf("0091d190_TEST FAIL %d\n",errors); return 1; }
 std::printf("0091d190_TEST PASS\n"); return 0;
}