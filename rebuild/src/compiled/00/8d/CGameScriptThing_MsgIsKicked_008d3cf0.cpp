// CGameScriptThing::MsgIsKicked -- query entity events within current script frame interval
// Leaf: MsgIsKicked
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
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
extern QuestManagerNative* QuestManager; //013B89FC
FABLE_FORCEINLINE long LastFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->LastScriptFrame:0;}
FABLE_FORCEINLINE long CurrentFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->CurrentScriptFrame:0;}
struct EventType{long Type;EventType(long t):Type(t){}};
struct CreatedBeforeOrOn{long Frame;CreatedBeforeOrOn(long f):Frame(f){}};
struct CreatedAfter{long Frame;CreatedAfter(long f):Frame(f){}};
struct Bind3{const EventType*First;const CreatedAfter*Second;const CreatedBeforeOrOn*Third;Bind3(const EventType&a,const CreatedAfter&b,const CreatedBeforeOrOn&c):First(&a),Second(&b),Third(&c){}};
struct EntityEventsMethods:CTCEntityEvents{bool HasEvent(const Bind3&);}; //008D4F80
struct CGameScriptThingMethods:CGameScriptThing{bool MsgIsKicked()const;};
bool CGameScriptThingMethods::MsgIsKicked()const{
 const PointerView* p=reinterpret_cast<const PointerView*>(PThing);if(((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem())&&!(((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem())->LifeFlags&1)){
 long last=LastFrame(QuestManager->PQuestCurrentlyRunning);long current=CurrentFrame(QuestManager->PQuestCurrentlyRunning);
 CThingNative* thing=((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem());if(thing->UsingComponents[2]&4){EntityEventsMethods* events;{long key=66;ComponentMap* map=&thing->Components;ComponentEntry*it=map->LowerBound(key);ComponentEntry*end=map->Finish;events=static_cast<EntityEventsMethods*>((it==end||it->Type>66?end:it)->Component);}if(events->HasEvent(Bind3(EventType(41),CreatedAfter(last),CreatedBeforeOrOn(current))))return true;}}return false;
}