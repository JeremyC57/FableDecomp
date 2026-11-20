// CModeBerserk::FrameUpdate -- select berserk animation group, restart changed mode, then update base frame.
// Leaf: FrameUpdate
// Native008D9530 length278. Genuine mode slots5/6 from VT1267F00=8621A0/8D6830; typed PMF table retains native4/full-host width.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
// Naming (NF209, 2026-11-20, q11692): the local stand-in AnimationGroupFilter (SetAndHash) is the shared engine/CAnimFilterOfGroup.h
//   (Ego_r.pdb CAnimFilterOfGroup: vptr, Group +4, GroupCRC +8); the hash call is FableWin.pdb
//   ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z (0x01DD3450), retail's out-of-line copy 0x0073DFF0 (NF128 inline opt-in,
//   called out of line here as retail does). The saved copy of the old group is a CCharString built by the copy constructor 0x0099EC30
//   (Ego_r / FableWin ??0CCharString@@QAE@ABV0@@Z), no longer the NativeStringValue stand-in copy constructor.
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE
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
#include "engine/CAnimFilterOfGroup.h"
// Name fix (NF185, 2026-11-20, q4457): the inequality is CCharString::operator!=(const CCharString&) const 0x0099E900 (fable_string.h,
//   Ego_r.pdb ??9CCharString@@QBE_NABV0@@Z), no longer the NativeStringValue::NotEqual stand-in member; FABLE_RETAIL_CCHARSTRING declares it.
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
 FABLE_FORCEINLINE void* GetInterface(int type){ETCInterfaceType key = (ETCInterfaceType)(type);ComponentMap*map=&Components;ComponentEntry*p=map->LowerBound(key);if(p==map->Last||type<p->first)p=map->Last;return ((void*)p->second);}
 template<class T>FABLE_FORCEINLINE bool GetInterface(T*&out,int type) {if(UsingComponents[type/32]&(1u<<(type%32))){out=(T*)GetInterface(type);return true;}return false;}
};

struct WeaponMethods:CTCWeapon{long GetWeaponClass()const;}; //5DA7F0, def+2C (PDB Class), not propagated CCategory label
struct CarryingMethods{ThingNative*GetThingInPrimarySlot();}; //6A1440: walks carry list and chooses primary entity
bool __fastcall IsThingCarryingMeleeWeapon(const ThingNative*); //6A15A0, presence70 +liveprimary +weapon72
struct NativeStringValue:CCharString {
 NativeStringValue():CCharString(){} //99E4B0
 NativeStringValue(const NativeStringValue&); //99EC30, actual header lacks narrow copy declaration
};
struct CountedAnimEntry {CAnimEntryLite*Data;CCPPointerInfo*Info;};
// Native typed projection of actual flattened CModeStandBase: inherited ParentThing and embedded string/filter remain typed for host fixtures.
struct ModeStandNative {
 void*VTable;ThingNative*ParentThing;CAnimFilterOfGroup WeaponBasedFilter;long FramesBeforeRemovingSelf;bool Active;unsigned char ActivePadding[3];
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
struct CModeBerserk:ModeStandNative {CAnimFilterOfGroup Filter;void FrameUpdate();void BaseFrameUpdate();}; //73AC50
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
 {CCharString previous(Filter.Group);changed=previous!=group;}
 if(changed){Filter.SetCurrentGroupName(group);(this->*static_cast<ModeSlot*>(VTable)[6].NoArg)();(this->*static_cast<ModeSlot*>(VTable)[5].NoArg)();}
 BaseFrameUpdate();
}