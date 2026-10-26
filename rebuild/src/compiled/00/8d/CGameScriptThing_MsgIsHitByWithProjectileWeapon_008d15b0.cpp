// CGameScriptThing::MsgIsHitByWithProjectileWeapon -- query entity events within current script frame interval
// Leaf: MsgIsHitByWithProjectileWeapon
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
struct CBaseIntelligentPointer{CBaseIntelligentPointer(); /* native00A01B10 */ void*VTable;CBaseObjectPointer* Info;CBaseObject* GetPItem()const;}; //00A01B50, PDB QBEPAVCBaseObject
struct PointerView:CBaseIntelligentPointer{CThingNative* Get()const{return reinterpret_cast<CThingNative*>(GetPItem());}};
// Retail CQuestManager+88 PQuestCurrentlyRunning and CScriptBase+2c PActiveThread differ from donor STL container sizes.
typedef CSpawnedFuncBase SpawnedFunc;
struct ScriptBaseNative{unsigned char Containers[0x2c];SpawnedFunc* PActiveThread;};
struct ActiveQuest{unsigned char State[8];ScriptBaseNative* Script;};
struct QuestManagerNative{unsigned char State[0x88];ActiveQuest* PQuestCurrentlyRunning;};
extern QuestManagerNative* QuestManager; //013B89FC
__forceinline long LastFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->LastScriptFrame:0;}
__forceinline long CurrentFrame(ActiveQuest* q){return q&&q->Script?q->Script->PActiveThread->CurrentScriptFrame:0;}
// Actual narrow header lacks the native out-of-line copy constructor. Include it as donor and expose the exact native signature below.
#define CCharString CCharStringDonor
#include "fable_string.h"
#undef CCharString
class CCharString { public:CCharStringData* storage_;CCharString();CCharString(const char*,long);CCharString(const CCharString&);~CCharString();}; //copy0099EC30, dtor0099EAE0
namespace NEntityEvents {enum EEventType { HitThing=29,MoralityChanged=35,CutSceneAnimEvent=36 };}
struct EventBase {EventBase();virtual ~EventBase();virtual NEntityEvents::EEventType GetType()const=0;virtual void OnAdd(const CTCEntityEvents&);virtual long GetLifeTime()const;virtual bool IsReadyToDie()const;long WorldFrame;};
class CHitParameters;
struct HitEventBase:EventBase {HitEventBase(const CHitParameters&);virtual ~HitEventBase();virtual long GetLifeTime()const;virtual bool IsFlourish()const;CHitParameters* HitParams;void*HitParamsInfo;CThingNative* GetPHitBy()const;float GetBuildupTime()const;}; // PDB008078C0 ctor,00807010 virtual IsFlourish; native00810F30 GetBuildupTime
struct HitByEvent:HitEventBase {HitByEvent(const CHitParameters&);virtual ~HitByEvent();virtual NEntityEvents::EEventType GetType()const;}; // PDB00815E20 genuine const-reference ctor

struct ScriptNameFilter {CCharString ScriptName;ScriptNameFilter(const CCharString& s):ScriptName(s){}};
struct ShotFilter {ShotFilter(){}};
struct CreatedBeforeOrOn{long Frame;CreatedBeforeOrOn(long f):Frame(f){}};
struct CreatedAfter{long Frame;CreatedAfter(long f):Frame(f){}};
struct Bind4 {const ShotFilter*First;const ScriptNameFilter*Second;const CreatedBeforeOrOn*Third;const CreatedAfter*Fourth;Bind4(const ShotFilter&a,const ScriptNameFilter&b,const CreatedBeforeOrOn&c,const CreatedAfter&d):First(&a),Second(&b),Third(&c),Fourth(&d){}};
struct EntityEventsMethods:CTCEntityEvents{bool GetEvent_008d4cd0(HitByEvent**,const Bind4&)const;}; //008D4CD0 PDB0090B9D0
struct CGameScriptThingMethods:CGameScriptThing{bool MsgIsHitByWithProjectileWeapon(const CCharString&,float&)const;};
bool CGameScriptThingMethods::MsgIsHitByWithProjectileWeapon(const CCharString& name,float& buildup)const {
 const PointerView* p=reinterpret_cast<const PointerView*>(PThing);
 if(p->Get()&&!(p->Get()->LifeFlags&1)) {
 long last=LastFrame(QuestManager->PQuestCurrentlyRunning);
 long current=CurrentFrame(QuestManager->PQuestCurrentlyRunning);
 CThingNative* thing=p->Get();
 if(thing->UsingComponents[2]&4) {
 EntityEventsMethods* events;
 {long key=66;ComponentMap* map=&thing->Components;const ComponentEntry*it=map->LowerBound(key);const ComponentEntry*end=map->Finish;events=static_cast<EntityEventsMethods*>((it==end||it->Type>66?end:it)->Component);}
 HitByEvent* event;
 if(events->GetEvent_008d4cd0(&event,Bind4(ShotFilter(),ScriptNameFilter(name),CreatedBeforeOrOn(current),CreatedAfter(last)))){buildup=event->GetBuildupTime();return true;}
 }
 }
 return false;
}