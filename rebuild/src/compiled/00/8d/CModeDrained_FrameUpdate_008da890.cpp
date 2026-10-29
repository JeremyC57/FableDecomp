// CModeDrained::FrameUpdate -- release drainer and remove this mode when the living drainer changes mode.
// Leaf: FrameUpdate
// Native008DA890 length256. Genuine unsigned long GetType() const virtual slot1; CBaseIntelligentPointer ctor/get/set/dtor A01B10/A01B50/A01B90/A01C10.
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
#ifdef _MSC_VER
#pragma pointers_to_members(full_generality,single_inheritance)
#endif
struct ModeDrained;
struct ModeInterface {void*VTable;}; //VT1268234 slot1=741460; donor754CB0 ?GetType@CModeDrained@NCreatureMode@@UBEKXZ
union ModeIdSlot {void*Native;unsigned long(ModeInterface::*GetType)()const;unsigned long(ModeDrained::*OwnType)()const;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(ModeIdSlot)==4);
#endif
struct ModeManager {ModeInterface&GetCurrentMode()const;void RemoveMode(unsigned long);}; //73A6E0 real method; 73ABA0 actual removes matching modeID and reactivates next
struct ModeDrained:ModeStandNative {
 AnimationGroupFilter Filter;IntelligentThingPointer PDrainer;long DrainerMode;
 void FrameUpdate();void BaseFrameUpdate(); //73AC50
};
FABLE_STATIC_ASSERT(offsetof(ModeDrained,PDrainer)==0x74);
FABLE_STATIC_ASSERT(offsetof(ModeDrained,DrainerMode)==0x7c);
void ModeDrained::FrameUpdate(){
 BaseFrameUpdate();
 if(PDrainer.GetPItem() && !(PDrainer.GetPItem()->LifeFlags&1)){
  ModeManager*manager;
  if(PDrainer.GetPItem()->GetInterface(manager,49)){
   ModeInterface&mode=manager->GetCurrentMode();
   if((mode.*static_cast<ModeIdSlot*>(mode.VTable)[1].GetType)()!=DrainerMode){
    PDrainer=IntelligentThingPointer(0);
    if(ParentThing->GetInterface(manager,49))manager->RemoveMode((this->*static_cast<ModeIdSlot*>(VTable)[1].OwnType)());
   }
  }
 }
}