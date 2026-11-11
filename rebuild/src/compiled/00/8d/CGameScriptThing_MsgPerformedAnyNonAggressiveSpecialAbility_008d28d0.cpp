// CGameScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility -- query entity events within current script frame interval
// Leaf: MsgPerformedAnyNonAggressiveSpecialAbility
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
#include "fable_compiler.h"
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
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct PointerView{void*VTable;IntelligentInfo* Info;}; //00A01B50
// Retail CQuestManager+88 PQuestCurrentlyRunning and CScriptBase+2c PActiveThread differ from donor STL container sizes.
typedef CSpawnedFuncBase SpawnedFunc;
struct ScriptBaseNative{unsigned char Containers[0x2c];SpawnedFunc* PActiveThread;};
struct ActiveQuest{unsigned char State[8];ScriptBaseNative* Script;};
struct QuestManagerNative{unsigned char State[0x88];ActiveQuest* PQuestCurrentlyRunning;};
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())
FABLE_FORCEINLINE long LastFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->LastScriptFrame:0;}
FABLE_FORCEINLINE long CurrentFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->CurrentScriptFrame:0;}
struct EntityEvent{virtual ~EntityEvent();virtual long GetType()const;long CreatedFrame;long Ability;bool Aggressive;};
struct EventNode{EventNode*Next;EventNode*Prev;EntityEvent*Data;void*Info;};
struct EventList{EventNode*Head;};
struct EntityEventsMethods:CTCEntityEvents{FABLE_FORCEINLINE EventNode* Head()const{return *reinterpret_cast<EventNode* const*>(Events);}};
struct CGameScriptThingMethods:CGameScriptThing{bool MsgPerformedAnyNonAggressiveSpecialAbility()const;};
bool CGameScriptThingMethods::MsgPerformedAnyNonAggressiveSpecialAbility()const{
 const PointerView* p=reinterpret_cast<const PointerView*>(PThing);if(((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem())&&!(((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem())->LifeFlags&1)){
 long last=LastFrame(reinterpret_cast<QuestManagerNative*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->PQuestCurrentlyRunning);long current=CurrentFrame(reinterpret_cast<QuestManagerNative*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->PQuestCurrentlyRunning);
 CThingNative* thing=((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem());if(thing->UsingComponents[2]&4){EntityEventsMethods* events;{long key=66;ComponentMap* map=&thing->Components;ComponentEntry*it=map->LowerBound(key);ComponentEntry*end=map->Finish;events=static_cast<EntityEventsMethods*>((it==end||it->Type>66?end:it)->Component);}for(EventNode*it=events->Head()->Next;it!=events->Head();it=it->Next){EntityEvent*event=it->Data;if(event->GetType()==26&&event->CreatedFrame>last&&event->CreatedFrame<=current&&!event->Aggressive)return true;}}}return false;
}