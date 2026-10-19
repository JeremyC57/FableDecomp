// CGameScriptThing::IsSummonedCreature -- check summon TC or ghostly-sword definition fallback
// Leaf: IsSummonedCreature
// Retail008D08A0 length191. PointerA01B50; component lowerbound4365B0; definition manager accessor4C7990; index9AD410; narrowstring ctor99EBF0,dtor99EAE0. Literal125D2A4 GHOSTLY_SWORD.
// CGameScriptThing::MsgPerformedFlourish -- query entity events within current script frame interval
// Leaf: MsgPerformedFlourish
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CQuestManager.h"
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
struct ComponentEntry{long Type;void* Component;};
struct ComponentMap{ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const long&);}; //004365B0
// Quarantined CThing donor has Components+48, retail has+44; lifecycle+91 confirmed by native.
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Metadata[0x40];unsigned short DefinitionID;unsigned char LifecycleBeforeFlags[3];unsigned char LifeFlags;};
#include "engine/CTCBase.h"
#include "engine/CDefinitionManager.h"
#include "fable_string.h"
struct IntelligentInfo{CThingNative* Object;long References;};
struct PointerView{void*VTable;IntelligentInfo* Info;CThingNative* GetPItem()const;}; //00A01B50
struct DefinitionManagerMethods:CDefinitionManager{long GetDefGlobalIndexFromName(const CCharString&);}; //009AD410
struct ThingMethods:CThingNative{DefinitionManagerMethods* GetDefinitionManager()const;__forceinline long GetDefinitionID()const{return (unsigned short)DefinitionID;}}; //004C7990
// PDB CTCSummonableCreature HasBeenSummoned+10; no generated class header exists.
struct SummonableCreature:CTCBase{unsigned long SummonState;bool HasBeenSummoned;__forceinline bool GetHasBeenSummoned()const{return HasBeenSummoned;}};
static __forceinline bool PeekSummon(CThingNative*thing,const SummonableCreature**out){if(thing->UsingComponents[5]&1){long key=160;ComponentMap*map=&thing->Components;ComponentEntry*it=map->LowerBound(key);ComponentEntry*end=map->Finish;*out=static_cast<const SummonableCreature*>((it==end||it->Type>160?end:it)->Component);return true;}return false;}
struct CGameScriptThingMethods:CGameScriptThing{bool IsSummonedCreature()const;};
bool CGameScriptThingMethods::IsSummonedCreature()const{
 const PointerView* p=reinterpret_cast<const PointerView*>(PThing);
 if(p->GetPItem()&&!(p->GetPItem()->LifeFlags&1)){
 const SummonableCreature* tc;if(PeekSummon(p->GetPItem(),&tc)&&tc->GetHasBeenSummoned())return true;
 long id;{CCharString name("GHOSTLY_SWORD",-1);id=static_cast<ThingMethods*>(p->GetPItem())->GetDefinitionManager()->GetDefGlobalIndexFromName(name);}
 if(id>0&&static_cast<ThingMethods*>(p->GetPItem())->GetDefinitionID()==id)return true;
 }return false;
}