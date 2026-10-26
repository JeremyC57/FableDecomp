// CModeBriarRoseConcentrate::OnDeactivate -- resume AI, hit surviving creature imposters, kill effect objects and deactivate.
// Leaf: OnDeactivate
// Native008E7E70 length269. Genuine CHitParameters ctor7A3BC0/dtor789358; Creature OnHit slot65 nativeVT125AC0C->667D30.
#ifndef _MSC_VER
#define __int64 long long
#define __fastcall
#define __forceinline inline
#endif
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CModeStandBase.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CDefClassBase.h"
#include "engine/_quarantine/CSpecialEffectsDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCWeapon.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingAICreature.h"
#include "engine/CBriarRoseDef.h"
#include "fable_string.h"
// Generated Thing offsets differ after bitset/vector shrink. Native presence bits20..43, map44..4F, CurrentMap8E,life91.
class C3DVector{ public:float X,Y,Z;};struct NativeDefinition;struct EmitterMethods;
struct ThingNative;
struct ComponentEntry{int Type;void* Component;};
struct ComponentMap{ComponentEntry*First,*Last,*Limit;ComponentEntry*LowerBound(const int*);}; //40F020
enum ETCInterfaceType{BriarRoseImposterTC=272};
struct ThingNative {
 void RemoveTC(ETCInterfaceType); //4C9840, genuine CThing removal
 void Kill(bool); //4C9B80
 const C3DVector&GetPos()const; //4C73D0 position-interface60 +C, realGetPos reference
 void SuspendAI(bool,void*); //833A10, actual CThingAICreature donor53A870, native suspenders1CC/AISuspended1C8bit1
 void* VTable;unsigned char BaseData[0xc];unsigned char RuntimeType;unsigned char BaseData11[0xf];unsigned int UsingComponents[9];ComponentMap Components;
 unsigned char Unrecovered50[0xc];EmitterMethods*Emitter;void*PositionInterface;unsigned char Unrecovered64[8];unsigned long TypeFlags;NativeDefinition*Definition;unsigned char Unrecovered74[0x18];unsigned short DefGlobalIndex;short CurrentMapNumber;signed char PlayerNumber;unsigned char LifeFlags;
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
 void OnDeactivate(); //861D00
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
 __forceinline IntelligentThingPointer(ThingNative*p=0):BaseIntelligentPointer(){SetPItem(p);}
 //implicit virtual destructor: VT1238C6C scalar-deleting4A9710 directly calls baseA01C10
 IntelligentThingPointer&operator=(const IntelligentThingPointer&p){SetPItem(p.GetPItem());return*this;}
};
FABLE_STATIC_ASSERT(sizeof(IntelligentThingPointer)==8);


struct NativeDamageMap {void*Head;unsigned long Count;unsigned char CompareAndAllocator[4];}; //native12 vsdonor16, head30,count34,empty policy storage38
struct AugmentationVector {long*First,*Last,*Limit;}; //native12 vsdonor16
struct CHitLocation;
enum EDamageAttribute{DamageNormal=0};
#pragma pack(push,1)
struct HitParameters {
 bool Positional,Directional;C3DVector Position,Direction;
 bool KnockDown,Decapitate,Blockable,Flourish,EpicSpell,BlockCounter,PlayHitResponse,PlayHitResponseOverrideSet,MoveBack,CreateParticleEffectOnHit,CreateDustParticleEffectOnHit,GuaranteeHit,Blocked,HitNegated,CauseRecoil;
 unsigned char Pad29[3];float ShotBuildupTime;NativeDamageMap DamageMap;AugmentationVector Augmentations;CHitLocation*PHitLocation;void*PHitMaterialDef;IntelligentThingPointer PThingHitting,PWeapon;
 HitParameters(); //7A3BC0, native defaults + tree head allocation + two IPbase constructors
 ~HitParameters(); //789358, IPs58/50, material4C, vector3C, tree30
 void AddDamageWithAttribute(EDamageAttribute,float); //868CA0, donor83EDD0 QAEXenumM, mapinsert at30
 void SetThingHitting(ThingNative&); //8685F0, donor83E4C0 QAEXAAVCThing
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(HitParameters)==0x60);
FABLE_STATIC_ASSERT(offsetof(HitParameters,PThingHitting)==0x50);
struct CreatureThingNative:ThingNative{};
#ifdef _MSC_VER
#pragma pointers_to_members(full_generality,single_inheritance)
#endif
union HitSlot{void*Native;void(CreatureThingNative::*OnHit)(HitParameters&);}; //nativeVT125AC0C slot65=667D30, donor544C80 UAEXAAVCHitParameters
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(HitSlot)==4);
#endif
struct IntelligentVector {IntelligentThingPointer*First,*Last,*Limit;};
struct ModeBriarConcentrate:ModeStandNative {
 AnimationGroupFilter Filter;long Timer;IntelligentThingPointer PTarget;IntelligentVector Imposters;bool Imposter;unsigned char Pad8D[3];const CBriarRoseDef*PDef;IntelligentThingPointer PBodyEffect,PSwordEffect;
 void OnDeactivate();
};
FABLE_STATIC_ASSERT(offsetof(ModeBriarConcentrate,Imposters)==0x80);
FABLE_STATIC_ASSERT(offsetof(ModeBriarConcentrate,PBodyEffect)==0x94);
void ModeBriarConcentrate::OnDeactivate(){
 if(ParentThing->RuntimeType==1)ParentThing->SuspendAI(false,this);
 HitParameters hit;hit.AddDamageWithAttribute(DamageNormal,10.0f);hit.SetThingHitting(*ParentThing);
 for(IntelligentThingPointer*it=Imposters.First;it!=Imposters.Last;++it){
  ThingNative*thing=it->GetPItem();
  if(thing&&!(thing->LifeFlags&1)&&(thing->TypeFlags&8)){
   CreatureThingNative*creature=static_cast<CreatureThingNative*>(thing);
   creature->RemoveTC(BriarRoseImposterTC);
   (creature->*static_cast<HitSlot*>(creature->VTable)[65].OnHit)(hit);
  }
 }
 if(PBodyEffect.GetPItem()&&!(PBodyEffect.GetPItem()->LifeFlags&1))PBodyEffect.GetPItem()->Kill(true);
 if(PSwordEffect.GetPItem()&&!(PSwordEffect.GetPItem()->LifeFlags&1))PSwordEffect.GetPItem()->Kill(true);
 ModeStandNative::OnDeactivate();
}