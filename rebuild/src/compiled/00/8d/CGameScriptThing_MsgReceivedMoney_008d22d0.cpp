// CGameScriptThing::MsgReceivedMoney -- scan entity events in (last,current] script frames
// Leaf: MsgReceivedMoney
// Retail 0x008d22d0. Native component lookup 004365B0, intelligent pointer 00A01B50, quest global013B89FC.
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CQuestManager.h"
#include "engine/CSpawnedFuncBase.h"
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
struct ComponentEntry{long Type;void* Component;};
struct ComponentMap{ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const long&) const;}; //004365B0
// Quarantined CThing donor has Components+48, retail has+44; lifecycle+91 confirmed by native.
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Metadata[0x45];unsigned char LifeFlags;};
struct IntelligentInfo{CThingNative* Object;long References;};
struct PointerView{void*VTable;IntelligentInfo* Info;CThingNative* GetPItem()const;}; //00A01B50
// Retail CQuestManager+88 PQuestCurrentlyRunning and CScriptBase+2c PActiveThread differ from donor STL container sizes.
typedef CSpawnedFuncBase SpawnedFunc;
struct ScriptBaseNative{unsigned char Containers[0x2c];SpawnedFunc* PActiveThread;};
struct ActiveQuest{unsigned char State[8];ScriptBaseNative* Script;};
struct QuestManagerNative{unsigned char State[0x88];ActiveQuest* PQuestCurrentlyRunning;};
extern QuestManagerNative* QuestManager; //013B89FC
__forceinline long LastFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->LastScriptFrame:0;}
__forceinline long CurrentFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->CurrentScriptFrame:0;}
// Event classes have no generated headers. PDB CEventBase is vptr+WorldFrame;
// event virtual slot0 is its deleting destructor, slot1 GetType (retail VTs audited).
struct EventBase{virtual ~EventBase();virtual int GetType()const=0;long WorldFrame;};
struct ExpressionEvent:EventBase{unsigned char PerformedBy[8];void* PExpressionDef;long DurationInFrames;};
struct MoneyEvent:EventBase{unsigned char ThingGivingItem[8];long Amount;};
struct AbilityEvent:EventBase{int Ability;bool Aggressive;};
struct EventNode{EventNode*Next;EventNode*Previous;EventBase*Data;void*Info;};
struct EventList{EventNode*Head;};
__forceinline EventList& Events(CTCEntityEvents*tc){return *reinterpret_cast<EventList*>(tc->Events);}
struct CGameScriptThingMethods:CGameScriptThing{bool MsgReceivedMoney(long& amount)const;};
bool CGameScriptThingMethods::MsgReceivedMoney(long& amount)const{
 const PointerView*p=reinterpret_cast<const PointerView*>(PThing);if(p->GetPItem()&&!(p->GetPItem()->LifeFlags&1)){
 long last=LastFrame(QuestManager->PQuestCurrentlyRunning);long current=CurrentFrame(QuestManager->PQuestCurrentlyRunning);
 CThingNative*thing=p->GetPItem();if(thing->UsingComponents[2]&4){CTCEntityEvents*events;{long key=66;ComponentMap*map=&thing->Components;ComponentEntry*it=map->LowerBound(key);ComponentEntry*end=map->Finish;events=static_cast<CTCEntityEvents*>((it==end||it->Type>66?end:it)->Component);}
 for(EventNode*it=Events(events).Head->Next;it!=Events(events).Head;it=it->Next){EventBase*event=it->Data;if(event->GetType()==9 && event->WorldFrame>last && event->WorldFrame<=current){amount=static_cast<MoneyEvent*>(event)->Amount;return true;}}}}
 return false;
}