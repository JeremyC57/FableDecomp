// CModeBriarRoseTeleportIntangible::OnDeactivate -- resume AI, re-enable mapwho/targeting/drawability, restore environment themes.
// Native008E7CF0 length273. SetAsDrawable slot28 nativeCThingVT1239ABC->4C8E50; GetBriar8E7790 uses CBriarRoseDef.
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
#include "engine/CEnvironment.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCTargeted.h"
#include "fable_string.h"
// Generated Thing offsets differ after bitset/vector shrink. Native presence bits20..43, map44..4F, CurrentMap8E,life91.
struct C3DVector{float X,Y,Z;};struct NativeDefinition;struct EmitterMethods;
struct ThingNative;
struct ComponentEntry{int Type;void* Component;};
struct ComponentMap{ComponentEntry*First,*Last,*Limit;ComponentEntry*LowerBound(const int*);}; //40F020
struct ThingNative {
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

#ifdef _MSC_VER
#pragma pointers_to_members(full_generality,single_inheritance)
#endif
struct NativeEffects;
template<class T>union CIVSlot{void*Native;void(T::*Delete)()const;}; //genuine CIV Delete const virtualslot1
// Actual flattened donor headers included; native prefix projection restores inherited unsigned IVRefCount at4.
struct BriarDefinitionPointer;
struct NativeDefinition:CIVCountedPointeeBase {bool GetBriar(BriarDefinitionPointer*)const;}; //8E7790, literal123562C CBriarRoseDef
struct NativeEffects:CIVCountedPointeeBase {bool GetEffectFromString(const CCharString&,long&)const;}; //669D30
static __forceinline void ReleaseDefinition(NativeDefinition*p){if(p&&!--p->IVRefCount)(p->*static_cast<CIVSlot<NativeDefinition>*>(p->__vftable)[1].Delete)();}
static __forceinline void ReleaseEffects(NativeEffects*p){if(p&&!--p->IVRefCount)(p->*static_cast<CIVSlot<NativeEffects>*>(p->__vftable)[1].Delete)();}
struct DefinitionPointer {NativeDefinition*Object;DefinitionPointer(NativeDefinition*p):Object(p){if(p)++p->IVRefCount;}~DefinitionPointer(){ReleaseDefinition(Object);}};
struct EffectsPointer {NativeEffects*Object;EffectsPointer():Object(0){}};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVSlot<NativeDefinition>)==4);
FABLE_STATIC_ASSERT(sizeof(CIVSlot<NativeEffects>)==4);
#endif

struct NativeBriar:CIVCountedPointeeBase {
 unsigned char DefInherited08[0x20];float SecondsToTeleport,SecondsToBeIntangible,SecondsToChargeDrainLife;long GuranteedImpostersHitBeforeAllowedToHitBriar,NumberOfHitsToKillImposters,EnvironmentThemeForIntangibleMode;float SecondsForIntangibleEnvironmentThemeTransitionIn,SecondsForIntangibleEnvironmentThemeTransitionOut;
}; //actual CBriarRoseDef typed inherited CIV projection, native header field44
FABLE_STATIC_ASSERT(offsetof(NativeBriar,SecondsForIntangibleEnvironmentThemeTransitionOut)==offsetof(CBriarRoseDef,SecondsForIntangibleEnvironmentThemeTransitionOut));
struct BriarDefinitionPointer{NativeBriar*Object;BriarDefinitionPointer():Object(0){}};
static __forceinline void ReleaseBriar(NativeBriar*p){if(p&&!--p->IVRefCount)(p->*static_cast<CIVSlot<NativeBriar>*>(p->__vftable)[1].Delete)();}
struct EnvironmentMethods:CEnvironment {void TransitionToDefaultThemeAllChannels(long,float);}; //6B3950, donor6EF590 QAEXJM
union DrawableSlot{void*Native;void(ThingNative::*SetAsDrawable)(bool);};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(DrawableSlot)==4);
#endif
struct ModeBriar:ModeStandNative {CWorld&GetWorld()const;void OnDeactivate();}; //73A680 global13B8A1C->World24
void ModeBriar::OnDeactivate(){
 ModeStandNative::OnDeactivate();
 if(ParentThing->RuntimeType==1)ParentThing->SuspendAI(false,this);
 CTCMapwho*mapwho;
 if(ParentThing->GetInterface(mapwho,58))mapwho->InMapwho[0]|=1;
 CTCTargeted*targeted;
 if(ParentThing->GetInterface(targeted,9))targeted->Targetable=true;
 ThingNative*parent=ParentThing;
 (parent->*static_cast<DrawableSlot*>(parent->VTable)[28].SetAsDrawable)(true);
 NativeDefinition*parentDefinition=ParentThing->Definition;
 float seconds=1.0f;BriarDefinitionPointer briar;bool found;
 {DefinitionPointer definition(parentDefinition);found=definition.Object->GetBriar(&briar);}
 NativeBriar*definition=briar.Object;
 if(found)seconds=definition->SecondsForIntangibleEnvironmentThemeTransitionOut;
 CWorld&world=GetWorld();
 static_cast<EnvironmentMethods*>(world.PEnvironment_ptr)->TransitionToDefaultThemeAllChannels(2,seconds);
 ReleaseBriar(definition);
}
