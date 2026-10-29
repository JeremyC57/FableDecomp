// CGameScriptThing::MsgIsHitByWithFlourish -- query entity events within current script frame interval
// Leaf: MsgIsHitByWithFlourish
#include "engine/CBaseObject.h"
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CQuestManager.h"
#include "engine/CSpawnedFuncBase.h"
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish,*Capacity;const ComponentEntry* LowerBound(const ETCInterfaceType& key) const;};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //004365B0
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
struct FlourishFilter {};
struct CreatedBeforeOrOn{long Frame;CreatedBeforeOrOn(long f):Frame(f){}};
struct CreatedAfter{long Frame;CreatedAfter(long f):Frame(f){}};
struct Bind4 {const ScriptNameFilter*First;const FlourishFilter*Second;const CreatedBeforeOrOn*Third;const CreatedAfter*Fourth;Bind4(const ScriptNameFilter&a,const FlourishFilter&b,const CreatedBeforeOrOn&c,const CreatedAfter&d):First(&a),Second(&b),Third(&c),Fourth(&d){}};
struct EntityEventsMethods:CTCEntityEvents{bool GetEvent(HitByEvent**,const Bind4&)const;}; //008D4A90 PDB0090B790
struct CGameScriptThingMethods:CGameScriptThing{bool MsgIsHitByWithFlourish(const CCharString&)const;};
bool CGameScriptThingMethods::MsgIsHitByWithFlourish(const CCharString& name)const {
 const PointerView* p=reinterpret_cast<const PointerView*>(PThing);
 if(p->Get()&&!(p->Get()->LifeFlags&1)) {
 long last=LastFrame(QuestManager->PQuestCurrentlyRunning);
 long current=CurrentFrame(QuestManager->PQuestCurrentlyRunning);
 CThingNative* thing=p->Get();
 if(thing->UsingComponents[2]&4) {
 EntityEventsMethods* events;
 {ETCInterfaceType key = (ETCInterfaceType)(66);ComponentMap* map=&thing->Components;const ComponentEntry*it=map->LowerBound(key);const ComponentEntry*end=map->Finish;events=static_cast<EntityEventsMethods*>(((void*)(it==end||it->first>66?end:it)->second));}
 HitByEvent* event;
 if(events->GetEvent(&event,Bind4(ScriptNameFilter(name),FlourishFilter(),CreatedBeforeOrOn(current),CreatedAfter(last))))return true;
 }
 }
 return false;
}