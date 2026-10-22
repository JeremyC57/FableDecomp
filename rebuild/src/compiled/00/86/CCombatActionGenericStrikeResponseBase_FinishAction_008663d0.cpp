// CCombatAction_GenericStrikeResponseBase::FinishAction() -- finish response, recover stun or enter alert below dazed threshold.
// Native008663D0,285 bytes. CreatureAction nativeA8 vs donorB4; stunned adds FlipA8. Physical HealthB4/KillableBC bit1 vs donorCC/D4.
#include "fable_string.h"
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCombatAction_GenericStrikeResponseBase CGenericStrikeResponse_PDB
#include "engine/_quarantine/CCombatAction_GenericStrikeResponseBase.h"
#undef CCombatAction_GenericStrikeResponseBase
#define CCreatureAction_BeStunned CCreatureAction_BeStunned_PDB
#include "engine/_quarantine/CCreatureAction_BeStunned.h"
#undef CCreatureAction_BeStunned
#include "engine/CThingPhysical.h"
#include "engine/_quarantine/CThing.h"
#include "engine/CThingCreatureBase.h"
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
struct Creature;
struct CCreatureActionBase {void* __vftable;unsigned char NativeActionState[0xa4];~CCreatureActionBase();}; //00693EF0; opaque native state constructed externally, no local field accesses.
struct CCreatureAction_BeStunned:CCreatureActionBase {bool Flip;CCreatureAction_BeStunned(Creature&,bool);}; //00865940
struct CCreatureAction_AlertToPlayer:CCreatureActionBase {CCreatureAction_AlertToPlayer(Creature&);}; //00865A00; PDB sizeB4 same as donor base.
struct CTCCreatureModeManager {struct CModeBase {virtual ~CModeBase();virtual unsigned long GetType()const;};CModeBase& GetCurrentMode()const;}; //0073A6E0; GetType genuine mode slot1
struct InterfaceEntry {long Type;CTCCreatureModeManager* Component;};
struct InterfaceMap {InterfaceEntry* First;InterfaceEntry* Last;InterfaceEntry* LowerBound(const long* type)const;}; //004365B0
struct Creature {
 unsigned char _beforeFlags[0x20];unsigned long PresentBits[9];InterfaceMap Interfaces;unsigned char _beforeDead[0x45];unsigned char DeadFlags;
 unsigned char _beforeHealth[0x22];float Health;long CanComeBetweenCameraAndObservedThing;unsigned char PhysicalFlags;
 float GetDazedHealthThreshold()const; //00662090
 bool SetCurrentAction(const CCreatureActionBase&); //006644F0
 __forceinline bool GetInterface(CTCCreatureModeManager** out,long type)const {if(PresentBits[type>>5]&(1UL<<(type&31))){long key=type;const InterfaceMap* map=&Interfaces;InterfaceEntry* p=map->LowerBound(&key);if(p==map->Last||type<p->Type)p=map->Last;*out=p->Component;return true;}return false;}
};
struct CBaseObjectPointer {void* object;unsigned long refcount;};
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;Creature* GetPItem()const;}; //00A01B50
struct CCombatActionBase {void* __vftable;unsigned char _baseState[4];CBaseIntelligentPointer PCreature;void FinishAction();}; //00859A20
struct CCombatAction_GenericStrikeResponseBase:CCombatActionBase {void FinishAction();};
void CCombatAction_GenericStrikeResponseBase::FinishAction(){
 CCombatActionBase::FinishAction();
 if(PCreature.GetPItem()&&!(PCreature.GetPItem()->DeadFlags&1)){
  CTCCreatureModeManager* modes;PCreature.GetPItem()->GetInterface(&modes,49);
  if(modes->GetCurrentMode().GetType()==29){Creature* owner=PCreature.GetPItem();owner->SetCurrentAction(CCreatureAction_BeStunned(*PCreature.GetPItem(),false));}
  else {Creature* creature=PCreature.GetPItem();if(creature->GetDazedHealthThreshold()>creature->Health && (creature->PresentBits[6]&0x20000) && (creature->PhysicalFlags&2)){
   Creature* owner=PCreature.GetPItem();owner->SetCurrentAction(CCreatureAction_AlertToPlayer(*PCreature.GetPItem()));
  }}
 }
}