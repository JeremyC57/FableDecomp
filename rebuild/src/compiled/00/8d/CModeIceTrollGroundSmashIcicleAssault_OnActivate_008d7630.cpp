// CModeIceTrollGroundSmashIcicleAssault::OnActivate -- retain snow-troll definition, suspend AI, and collect viable positions.
// Leaf: OnActivate
// Native008D7630 length242. Subdef8C7A70 identifies CSnowTrollDef by native123565C; SuspendAI833A10 is genuine CThingAICreature API.
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
#include "engine/CSnowTrollDef.h"
#include "fable_string.h"
// Generated Thing offsets differ after bitset/vector shrink. Native presence bits20..43, map44..4F, CurrentMap8E,life91.
class C3DVector{ public:float X,Y,Z;};struct NativeDefinition;struct EmitterMethods;
struct ThingNative;
struct ComponentEntry{int Type;void* Component;};
struct ComponentMap{ComponentEntry*First,*Last,*Limit;ComponentEntry*LowerBound(const int*);}; //40F020
class CThing { public:
    const C3DVector& GetPos() const; //4C73D0 position-interface60 +C, realGetPos reference
};
struct ThingNative : public CThing {
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
 void OnActivate(); //8621A0
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
struct SnowDefinitionPointer {const CSnowTrollDef*Object;}; //native CDefPointer<CSnowTrollDef const> view, donor size4
struct NativeDefinition:CIVCountedPointeeBase {bool GetSnowTroll(SnowDefinitionPointer*)const;}; //8C7A70, GetSubDef CSnowTrollDef literal123565C
struct NativeEffects:CIVCountedPointeeBase {bool GetEffectFromString(const CCharString&,long&)const;}; //669D30
static __forceinline void ReleaseDefinition(NativeDefinition*p){if(p&&!--p->IVRefCount)(p->*static_cast<CIVSlot<NativeDefinition>*>(p->__vftable)[1].Delete)();}
static __forceinline void ReleaseEffects(NativeEffects*p){if(p&&!--p->IVRefCount)(p->*static_cast<CIVSlot<NativeEffects>*>(p->__vftable)[1].Delete)();}
struct DefinitionPointer {NativeDefinition*Object;DefinitionPointer(NativeDefinition*p):Object(p){if(p)++p->IVRefCount;}~DefinitionPointer(){ReleaseDefinition(Object);}};
struct EffectsPointer {NativeEffects*Object;EffectsPointer():Object(0){}};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVSlot<NativeDefinition>)==4);
FABLE_STATIC_ASSERT(sizeof(CIVSlot<NativeEffects>)==4);
#endif
class CAFile;class CCamera;class CInterpolationInfo;class C3DVector;class CDisplayEngine;class CEnginePrimitivePrepareInfo;
// Actual CWorld primary VT12390F0 has14 entries. Overloaded camera declarations reverse to native slots5/6.
struct WorldNative {
 unsigned char Unrecovered04[0x1c];CThingSearchTools*Search;
 virtual ~WorldNative(); //slot0 4AE8D0
 virtual void PostLoadInit(); //slot1 49D970
 virtual void LoadWorldFromFile(CAFile&,long); //slot2 49E220
 virtual CThingManager& GetThingManager(); //slot3 49E1B0
 virtual const CThingManager& PeekThingManager()const; //slot4 49E1C0
 virtual CCamera GetCurrentRenderCamera()const; //slot6 49DA60
 virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo&)const; //slot5 49DA00
 virtual void PostInit(CDisplayEngine&); //slot7 4A6550
 virtual C3DVector GetGameLODCentre()const; //slot8 49DBE0
 virtual void Init(); //slot9 4A6E30
 virtual void Uninitialise(); //slot10 4A2E90
 virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&); //slot11 4A1CB0
 virtual CWorldMap& GetWorldMap(); //slot12 4AE8B0
 virtual const CWorldMap& PeekWorldMap()const; //slot13 4AE8C0
};

struct PositionVector {C3DVector*First,*Last,*Limit;};
struct CIcicleData;
struct IcicleVector {CIcicleData*First,*Last,*Limit;};
class CTCShapeManager;
void __fastcall GetValidPositions(PositionVector&,const CWorldMap&,const CThingSearchTools&,const C3DVector&,float,bool,const CTCShapeManager*); //74A5D0, PDB75C940 signature
struct ModeIceTroll:ModeStandNative {
 IntelligentThingPointer PTarget;AnimationGroupFilter Filter;long Timer;SnowDefinitionPointer PDef;IcicleVector Icicles;PositionVector ViablePoints;
 WorldNative&GetWorld()const; //73A680 global13B8A1C world24
 void OnActivate();
};
FABLE_STATIC_ASSERT(offsetof(ModeIceTroll,Timer)==0x7c);
FABLE_STATIC_ASSERT(offsetof(ModeIceTroll,PDef)==0x80);
FABLE_STATIC_ASSERT(offsetof(ModeIceTroll,ViablePoints)==0x90);
void ModeIceTroll::OnActivate(){
 ModeStandNative::OnActivate();
 {DefinitionPointer definition(ParentThing->Definition);definition.Object->GetSnowTroll(&PDef);}
 Timer=0;
 if(ParentThing->RuntimeType==1)ParentThing->SuspendAI(true,0);
 C3DVector pos=ParentThing->GetPos();
 if(PTarget.GetPItem()&&!(PTarget.GetPItem()->LifeFlags&1))pos=PTarget.GetPItem()->GetPos();
 WorldNative&world=GetWorld();
 const CSnowTrollDef*definition=PDef.Object;
 CThingSearchTools*search=GetWorld().Search;
 GetValidPositions(ViablePoints,world.GetWorldMap(),*search,pos,definition->DistanceRange,false,0);
}