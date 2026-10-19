// CModeStrafe::OnActivate -- initialise strafing group, animation timing and clear combination layers23,24,25,1F.
// Native008E9A30 length297. Genuine Strafe VT1268588 slot3=8EA220; ego90F530 supplies animation-distance/index signature.
#ifndef _MSC_VER
#define __int64 long long
#define __fastcall
#define __forceinline inline
#endif
#include <string.h>
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CTCWeapon.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CModeStandBase.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationInfo.h"
#include "fable_string.h"
// Generated Thing offsets differ after bitset/vector shrink. Native presence bits20..43, map44..4F, CurrentMap8E,life91.
struct ThingNative;
struct ComponentEntry{int Type;void* Component;};
struct ComponentMap{ComponentEntry*First,*Last,*Limit;ComponentEntry*LowerBound(const int*);}; //40F020
struct ThingNative {
 void InitialiseMovementSpeeds(); //6A9A00 valid only runtime player-type2
 void* VTable;unsigned char BaseData[0xc];unsigned char RuntimeType;unsigned char BaseData11[0xf];unsigned int UsingComponents[9];ComponentMap Components;
 unsigned char Unrecovered50[0x3c];unsigned short DefGlobalIndex;short CurrentMapNumber;signed char PlayerNumber;unsigned char LifeFlags;
 __forceinline void* GetInterface(int type){int key=type;ComponentMap*map=&Components;ComponentEntry*p=map->LowerBound(&key);if(p==map->Last||type<p->Type)p=map->Last;return p->Component;}
 template<class T>__forceinline bool GetInterface(T*&out,int type) {if(UsingComponents[type/32]&(1u<<(type%32))){out=(T*)GetInterface(type);return true;}return false;}
};
struct WeaponMethods:CTCWeapon {const CCharString& GetAnimationGroupName()const;}; //005DABE0, returned def-string at weapondef+34 or shared13BAF5C
struct MeshBankMethods:CMeshDataBank {const C3DAnimationInfo& GetAnimationInfo(long)const;}; //00A242C0, bank pointer array+380 indexed by entry BankIndex
extern long GameFramesPerSecond; //01375550 native15
extern char EmptyNarrowString[]; //0129AAF4
unsigned long __fastcall CalculateCRC(unsigned long,const void*,unsigned long); //004014A0
struct StringAccess {
 static __forceinline CCharStringData* Data(const CCharString&s){CCharStringData*p;memcpy(&p,&s,sizeof p);return p;}
}; //read actual string representation using memcpy, preserving host alias rules
struct AnimationGroupFilter {
 void*VTable;CCharString Group;unsigned long CRC;
 void SetAndHash(const CCharString&); //0073DFF0
 __forceinline void SetLiteral(const char*name){CCharString text(name,-1);Group=text;CCharStringData*p=StringAccess::Data(text);unsigned long length;const char*data;if(!p){length=0;data=EmptyNarrowString;}else{length=p->unknown04;data=p->text;}CRC=CalculateCRC(0,data,length);}
};
struct AnimEntryLite{long BankIndex,DelayTime,TransitionTime,AnimFlags;}; //PDB native16
struct CountedAnimEntry{AnimEntryLite*Data;void*Info;};

struct NativeStringValue:CCharString {
 NativeStringValue():CCharString(){}
 NativeStringValue(const NativeStringValue&); //99EC30, recover missing actual-header narrow copy interface
};
enum EMovementType {MovementWalk=0};
NativeStringValue __fastcall GetStrafingAnimationName(const ThingNative&,EMovementType); //8E8FF0 hidden resultECX,parentEDX,movementstack
bool __fastcall GetAnimDistanceAndIndexFromAnimName(const ThingNative&,const CCharString&,long&,float&); //8E9080, donor90F530
struct CAnimSet;
struct AnimateThingMethods {CAnimSet*GetFirstCombinationAnimFromType(long);void RemoveCombinationAnimsOfType(long);}; //70C4E0,70D8C0
struct AnimationComponentMethods {AnimateThingMethods&GetAnimation();}; //70B460, actual CTCAnimationComplex +C
// Native CModeBase prefix from PDB and actual CModeStandBase offsets; no generated CModeBase/Strafe header.
struct BaseModeNative {void*VTable;ThingNative*ParentThing;AnimationGroupFilter WeaponFilter;long FramesBeforeRemovingSelf;bool Active;unsigned char Pad19[3];void OnActivate();}; //73AAC0
struct CModeStrafe:BaseModeNative {
 float AnimMovementDistance;CCharString StrafeAnimName;long AnimIndex;bool LastUsable;unsigned char Pad29[3];
 AnimationGroupFilter Filter;long ActiveWeapon,AimingNumFrames,AimingCenterFrame;float RumbleParam,PullBackAmount;bool ForceAnimNoBlendIn;unsigned char Padding4D[3];CountedAnimEntry PAnimProjectileAim;
 void FrameUpdate();void OnActivate();
};
#ifdef _MSC_VER
#pragma pointers_to_members(full_generality,single_inheritance)
#endif
union ModeSlot {void*Native;void(CModeStrafe::*FrameUpdate)();}; //genuine inherited slot3; base73AC50 and Strafe8EA220 in ownVT1268588
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(ModeSlot)==4);
#endif
FABLE_STATIC_ASSERT(offsetof(CModeStrafe,Filter)==0x2c);
FABLE_STATIC_ASSERT(offsetof(CModeStrafe,StrafeAnimName)==0x20);
void CModeStrafe::OnActivate(){
 ActiveWeapon=-2;
 Filter.SetLiteral("STAND");
 (this->*static_cast<ModeSlot*>(VTable)[3].FrameUpdate)();
 BaseModeNative::OnActivate();
 NativeStringValue name=GetStrafingAnimationName(*ParentThing,MovementWalk);
 ThingNative*parent=ParentThing;
 StrafeAnimName=name;
 GetAnimDistanceAndIndexFromAnimName(*parent,name,AnimIndex,AnimMovementDistance);
 AnimationComponentMethods*component;
 ParentThing->GetInterface(component,90); //required TC90 precondition, exactly native unchecked out-parameter
 AnimateThingMethods&animation=component->GetAnimation();
 if(animation.GetFirstCombinationAnimFromType(0x24))ForceAnimNoBlendIn=true;
 animation.RemoveCombinationAnimsOfType(0x23);
 animation.RemoveCombinationAnimsOfType(0x24);
 animation.RemoveCombinationAnimsOfType(0x25);
 animation.RemoveCombinationAnimsOfType(0x1f);
 if(ParentThing->RuntimeType==2)ParentThing->InitialiseMovementSpeeds();
}