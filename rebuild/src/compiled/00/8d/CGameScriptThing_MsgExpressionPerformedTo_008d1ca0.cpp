// CGameScriptThing::MsgExpressionPerformedTo -- query entity events within current script frame interval
// Leaf: MsgExpressionPerformedTo
#include "fable_string.h"
#include "engine/CIVCountedPointeeBase.h"
#define CDefClassBase CDefClassBaseDonor
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#include "engine/_quarantine/CExpressionDef.h"
#include "engine/CBaseObject.h"
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CQuestManager.h"
#include "engine/CSpawnedFuncBase.h"
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
// Actual donor parent is0x3D with InstantiationName+25; retail native parent0x3C also has name+25 (getter004303B0) and smaller native map. Native21-slot hierarchy below is independently RTTI verified at ExpressionDef VT01233D1C.
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
// Event kind3 ExpressionPerformedTo; virtual slot1 GetType.
// PDB ExpressionPerformedTo: WorldFrame+4, PerformedBy+8, expression definition+10h.
// Real event virtual order: destructor, GetType, OnAdd, GetLifeTime, IsReadyToDie.
// Native base0124F2C4 slots2=57A4FC(ret4),3=810E60,4=57A4FF(true).
namespace NEntityEvents {enum EEventType { HitThing=29,MoralityChanged=35,CutSceneAnimEvent=36 };}
struct EventBase { EventBase(); /* PDB01D93440 */ virtual ~EventBase(); virtual NEntityEvents::EEventType GetType()const=0; virtual void OnAdd(const CTCEntityEvents&); virtual long GetLifeTime()const; virtual bool IsReadyToDie()const; long WorldFrame; };
struct CDefString { long TablePos; };
struct CCharString;
struct CDefClassBase { struct CSubDefInfo; };
struct CPersistContext;
struct CParentDefClassBase {
 virtual ~CParentDefClassBase(); // slot0 virtual deleting destructor (native derived00458B81)
 virtual void Delete() const; // slot1 0042D930
 virtual bool UnloadResource(); // slot2 009ACE90
 virtual unsigned long GetUnloadableMemory() const; // slot3 009FBEF0
 virtual bool IsResourceLoaded() const; // slot4 0042D950
 virtual void OnResourceLoaded(); // slot5 0042D960
 virtual bool CanAbortResourceLoad(long) const; // slot6 0042D970
 virtual void OnResourceLoadAborted(); // slot7 0042D980
 virtual unsigned long GetDebugMemoryInUse() const; // slot8 009FBF00
 virtual void ResetDebugMemoryInUse(); // slot9 009FBF10
 virtual CDefString GetInstantiationName() const; // slot10 004303B0
 virtual void SetInstantiationName(CDefString); // slot11 004303C0
 virtual void AddSubDef(const CCharString&,long,long); // slot12 009B00D0
 virtual void RemoveSubDef(const CCharString&); // slot13 009B0170
 virtual CDefClassBase::CSubDefInfo* GetPSubDefInfo(const CCharString&); // VC reverses overload order: native15 009AD510
 virtual const CDefClassBase::CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // native14 009AD4A0
 virtual bool IsParentDef() const; // slot16 004303D0
 virtual void PostTransfer(); // slot17 0042DA80
 virtual void Transfer(CPersistContext&); // slot18 inherited override
 virtual void Copy(const CDefClassBase*); // slot19
 virtual unsigned long GetSizeofClass(); // slot20
 mutable unsigned long IVRefCount;unsigned char NativeBaseData[0x34];
 CParentDefClassBase(); // 0x00430370
};

// Flattened native virtual projection retains all real slots; quarantined expression donor uses larger containers.
struct ExpressionDefNative:CParentDefClassBase {};
struct CDefStringTable {CCharString GetString(long)const;}; //009D49B0, PDB GetString@@QBE...J
extern CDefStringTable DefinitionStrings; //013CA828
class CThingCreatureBase; class CExpressionDef; template<class T> class CDefPointer;
struct ExpressionEvent:EventBase {ExpressionEvent(CThingCreatureBase*,long,const CDefPointer<const CExpressionDef>&,long,long); /* PDB00526270 */ PointerView PerformedBy;const ExpressionDefNative* PExpressionDef; };
struct EventNode { EventNode* Next;EventNode* Previous;EventBase* Data;void* Info; };
struct EventList { EventNode* Head; };
struct CGameScriptThingMethods:CGameScriptThing{bool MsgExpressionPerformedTo(CCharString& name)const;};
bool CGameScriptThingMethods::MsgExpressionPerformedTo(CCharString& name)const {
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
 if(e->GetType()==3&&e->WorldFrame>last&&e->WorldFrame<=current) {
 const ExpressionDefNative* def=static_cast<ExpressionEvent*>(e)->PExpressionDef;
 if(def)++def->IVRefCount;
 {CCharString text=DefinitionStrings.GetString(def->GetInstantiationName().TablePos);name=text;}
 if(--def->IVRefCount==0)def->Delete();
 return true;
 }
 }
 }
 }
 return false;
}