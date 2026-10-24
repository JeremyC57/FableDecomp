// CModeScreamerAggressive::OnDeactivate -- kill a live effect, clear target, disable melee readiness and deactivate stand state.
// Leaf: OnDeactivate
// Native008DA690 length258. Pointer template evidence same as Drained; Combat donor ReadyForMelee90 ->retail80 after3 vector and1map shrink.
#ifndef _MSC_VER
#define __int64 long long
#define __fastcall
#define __forceinline inline
#endif
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CModeStandBase.h"
#include "engine/CTCCombat.h"
#include "engine/CTCWeapon.h"
#include "fable_string.h"
// Generated Thing offsets differ after bitset/vector shrink. Native presence bits20..43, map44..4F, CurrentMap8E,life91.
struct ThingNative;
struct ComponentEntry{int Type;void* Component;};
struct ComponentMap{ComponentEntry*First,*Last,*Limit;ComponentEntry*LowerBound(const int*);}; //40F020
struct ThingNative {
 void Kill(bool); //4C9B80: native kill flags/deferred destruction
 void* VTable;unsigned char BaseData[0x1c];unsigned int UsingComponents[9];ComponentMap Components;
 unsigned char Unrecovered50[0x3c];unsigned short DefGlobalIndex;short CurrentMapNumber;signed char PlayerNumber;unsigned char LifeFlags;
 __forceinline void* GetInterface(int type){int key=type;ComponentMap*map=&Components;ComponentEntry*p=map->LowerBound(&key);if(p==map->Last||type<p->Type)p=map->Last;return p->Component;}
 template<class T>__forceinline bool GetInterface(T*&out,int type) {if(UsingComponents[type/32]&(1u<<(type%32))){out=(T*)GetInterface(type);return true;}return false;}
};

struct WeaponMethods:CTCWeapon{long GetWeaponClass()const;}; //5DA7F0, def+2C (PDB Class), not propagated CCategory label
struct CarryingMethods{ThingNative*GetThingInPrimarySlot();}; //6A1440: walks carry list and chooses primary entity
bool __fastcall IsThingCarryingMeleeWeapon(const ThingNative*); //6A15A0, presence70 +liveprimary +weapon72
struct NativeStringValue:CCharString {
 NativeStringValue():CCharString(){} //99E4B0
 NativeStringValue(const NativeStringValue&); //99EC30, actual header lacks narrow copy declaration
 bool NotEqual(const CCharString&)const; //99E900, audited compares storage/length/content and inverts
};
struct AnimationGroupFilter {void*VTable;NativeStringValue Group;unsigned long CRC;void SetAndHash(const CCharString&);}; //73DFF0, 99EFB0 assignment+4014A0 CRC
struct CountedAnimEntry {CAnimEntryLite*Data;CCPPointerInfo*Info;};
// Native typed projection of actual flattened CModeStandBase: inherited ParentThing and embedded string/filter remain typed for host fixtures.
struct ModeStandNative {
 void*VTable;ThingNative*ParentThing;AnimationGroupFilter WeaponBasedFilter;long FramesBeforeRemovingSelf;bool Active;unsigned char ActivePadding[3];
 long IdleCounter,FramesToKeepUpdatingMovement;bool ForceAddNewAnims;unsigned char ForcePadding[3];float MaxRunSpeed,MaxJogSpeed,MaxWalkSpeed;NativeStringValue PoseAnimName;
 CountedAnimEntry Stand,SlowWalk,Walk,Jog,Run,Sprint;
};
FABLE_STATIC_ASSERT(sizeof(ModeStandNative)==sizeof(CModeStandBase));

struct IntelligentInfo {ThingNative*Object;unsigned long RefCount;}; //CBaseObjectPointer native object0/count4; differs from counted-block ABI
struct BaseIntelligentPointer {
 IntelligentInfo*PData;
 BaseIntelligentPointer(); //A01B10, native baseVT129C95C
 virtual ~BaseIntelligentPointer(); //A01C10
 ThingNative*GetPItem()const; //A01B50
 void SetPItem(ThingNative*); //A01B90
};
struct IntelligentThingPointer:BaseIntelligentPointer {
 IntelligentThingPointer(ThingNative*p=0):BaseIntelligentPointer(){SetPItem(p);}
 //implicit virtual destructor: VT1238C6C scalar-deleting4A9710 directly calls baseA01C10
 IntelligentThingPointer&operator=(const IntelligentThingPointer&p){SetPItem(p.GetPItem());return*this;}
};
FABLE_STATIC_ASSERT(sizeof(IntelligentThingPointer)==8);

struct CombatNative {unsigned char Prefix[0x80];bool ReadyForMelee;}; //actual header donor90, three vectors16->12 andmap16->12
struct ModeScreamer:ModeStandNative {
 AnimationGroupFilter Filter;IntelligentThingPointer PEffect,PTarget;
 void OnDeactivate();void StandDeactivate(); //861D00
};
FABLE_STATIC_ASSERT(offsetof(ModeScreamer,PEffect)==0x74);
FABLE_STATIC_ASSERT(offsetof(ModeScreamer,PTarget)==0x7c);
void ModeScreamer::OnDeactivate(){
 if(PEffect.GetPItem()&&!(PEffect.GetPItem()->LifeFlags&1)){
  PEffect.GetPItem()->Kill(true);
  PEffect=IntelligentThingPointer(0);
 }
 PTarget=IntelligentThingPointer(0);
 CombatNative*combat; //engine precondition: Screamer owner supplies Combat48; native ignores false output-query result
 ParentThing->GetInterface(combat,48);
 combat->ReadyForMelee=false;
 StandDeactivate();
}