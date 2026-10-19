// CGameScriptThing::MsgOnCutSceneAnimEvent -- query entity events within current script frame interval
// Leaf: MsgOnCutSceneAnimEvent
#include "fable_string.h"
#include "engine/CBaseObject.h"
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CQuestManager.h"
#include "engine/CSpawnedFuncBase.h"
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
struct ComponentEntry{long Type;void* Component;};
struct ComponentMap{ComponentEntry *Start,*Finish,*Capacity;const ComponentEntry* LowerBound(const long&)const;}; //004365B0
// Quarantined CThing donor has Components+48, retail has+44; lifecycle+91 confirmed by native.
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Metadata[0x41];unsigned char LifeFlags;};
struct IntelligentInfo{CThingNative* Object;long References;};
struct CBaseIntelligentPointer{CBaseIntelligentPointer(); /* actual native default ctor 00A01B10 */ void*VTable;CBaseObjectPointer* Info;CBaseObject* GetPItem()const;}; //00A01B50, PDB QBEPAVCBaseObject
struct PointerView:CBaseIntelligentPointer{CThingNative* Get()const{return reinterpret_cast<CThingNative*>(GetPItem());}};
// Retail CQuestManager+88 PQuestCurrentlyRunning and CScriptBase+2c PActiveThread differ from donor STL container sizes.
typedef CSpawnedFuncBase SpawnedFunc;
struct ScriptBaseNative{unsigned char Containers[0x2c];SpawnedFunc* PActiveThread;};
struct ActiveQuest{unsigned char State[8];ScriptBaseNative* Script;};
struct QuestManagerNative{unsigned char State[0x88];ActiveQuest* PQuestCurrentlyRunning;};
extern QuestManagerNative* QuestManager; //013B89FC
__forceinline long LastFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->LastScriptFrame:0;}
__forceinline long CurrentFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->CurrentScriptFrame:0;}
// Event kind36; virtual slot1 GetType. String assignments0099EFE0(char*) and0099EFB0(CCharString&).
// PDB CEventCutSceneAnimEvent: WorldFrame+4, EventName+8. Native0122D70E contains empty string.
// Real event virtual order: destructor, GetType, OnAdd, GetLifeTime, IsReadyToDie.
// Native base0124F2C4 slots2=57A4FC(ret4),3=810E60,4=57A4FF(true).
namespace NEntityEvents {enum EEventType { HitThing=29,MoralityChanged=35,CutSceneAnimEvent=36 };}
struct EventBase { EventBase(); // genuine PDB01D93440
 virtual ~EventBase(); virtual NEntityEvents::EEventType GetType()const=0; virtual void OnAdd(const CTCEntityEvents&); virtual long GetLifeTime()const; virtual bool IsReadyToDie()const; long WorldFrame; };
struct CutSceneEvent:EventBase { CutSceneEvent(const CCharString&); // PDB01DB7140
 CCharString EventName; };
struct EventNode { EventNode* Next;EventNode* Previous;EventBase* Data;void* Info; };
struct EventList { EventNode* Head; };
struct CGameScriptThingMethods:CGameScriptThing{bool MsgOnCutSceneAnimEvent(CCharString& name)const;};
bool CGameScriptThingMethods::MsgOnCutSceneAnimEvent(CCharString& name)const {
 name="";
 const PointerView* p=reinterpret_cast<const PointerView*>(PThing);
 if(p->Get()&&!(p->Get()->LifeFlags&1)) {
 long last=LastFrame(QuestManager->PQuestCurrentlyRunning);
 long current=CurrentFrame(QuestManager->PQuestCurrentlyRunning);
 CThingNative* thing=p->Get();
 if(thing->UsingComponents[2]&4) {
 CTCEntityEvents* events;
 {long key=66;ComponentMap* map=&thing->Components;const ComponentEntry*it=map->LowerBound(key);const ComponentEntry*end=map->Finish;events=static_cast<CTCEntityEvents*>((it==end||it->Type>66?end:it)->Component);}
 for(EventNode* n=reinterpret_cast<EventList*>(events->Events)->Head->Next;n!=reinterpret_cast<EventList*>(events->Events)->Head;n=n->Next) {
 EventBase* e=n->Data;
 if(e->GetType()==36&&e->WorldFrame>last&&e->WorldFrame<=current) {
 name=static_cast<CutSceneEvent*>(e)->EventName;return true;
 }
 }
 }
 }
 return false;
}