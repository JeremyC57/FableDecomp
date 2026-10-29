// CModeBerserk::FrameUpdate -- select berserk animation group, restart changed mode, then update base frame.
// Leaf: FrameUpdate
// Native008D9530 length278. Genuine mode slots5/6 from VT1267F00=8621A0/8D6830; typed PMF table retains native4/full-host width.
#ifndef _MSC_VER
#define __int64 long long
#define __fastcall
#define __forceinline inline
#endif
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CModeStandBase.h"
#include "engine/CTCWeapon.h"
#include "fable_string.h"
// Generated Thing offsets differ after bitset/vector shrink. Native presence bits20..43, map44..4F, CurrentMap8E,life91.
struct ThingNative;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry*First,*Last,*Limit;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //40F020
struct ThingNative {
 void* VTable;unsigned char BaseData[0x1c];unsigned int UsingComponents[9];ComponentMap Components;
 unsigned char Unrecovered50[0x3c];unsigned short DefGlobalIndex;short CurrentMapNumber;signed char PlayerNumber;unsigned char LifeFlags;
 __forceinline void* GetInterface(int type){ETCInterfaceType key = (ETCInterfaceType)(type);ComponentMap*map=&Components;ComponentEntry*p=map->LowerBound(key);if(p==map->Last||type<p->first)p=map->Last;return ((void*)p->second);}
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
#ifdef _MSC_VER
#pragma pointers_to_members(full_generality,single_inheritance)
#endif
struct CModeBerserk;
union ModeSlot {void*Native;void(CModeBerserk::*NoArg)();};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(ModeSlot)==4);
#endif
struct CModeBerserk:ModeStandNative {AnimationGroupFilter Filter;void FrameUpdate();void BaseFrameUpdate();}; //73AC50
void CModeBerserk::FrameUpdate(){
 NativeStringValue group;
 if(IsThingCarryingMeleeWeapon(ParentThing)){
  CarryingMethods*carrying;
  if(ParentThing->GetInterface(carrying,70)){
   ThingNative*weapon=carrying->GetThingInPrimarySlot();
   if(weapon && !(weapon->LifeFlags&1)){
    WeaponMethods*component;
    if(weapon->GetInterface(component,72)&&component->GetWeaponClass()==2)group.CCharString::operator=("BERSERK_WITH_HEAVY_SWORD");
    else group.CCharString::operator=("BERSERK_WITH_SWORD");
   }
  }else group.CCharString::operator=("BERSERK_WITH_SWORD");
 }else group.CCharString::operator=("BERSERK_NO_WEAPON");
 bool changed;
 {NativeStringValue previous(Filter.Group);changed=previous.NotEqual(group);}
 if(changed){Filter.SetAndHash(group);(this->*static_cast<ModeSlot*>(VTable)[6].NoArg)();(this->*static_cast<ModeSlot*>(VTable)[5].NoArg)();}
 BaseFrameUpdate();
}