// Retail CTCWillLightning::CLightningBeam(CThing*,CThing*), PDB026AFF00.
// Native list is one pointer rather than donor12 bytes; subsequent fields shift -8.
#include "engine/CTCWillLightning.h"
#include "engine/CBaseObject.h"
#include "engine/_quarantine/CThing.h"
#include "engine/CIVCountedPointeeBase.h"
#include <stdlib.h>
namespace LightningNative {
struct BaseIntelligentPointer {
 BaseIntelligentPointer(); //A01B10; PDBB4C7A0
 virtual ~BaseIntelligentPointer(); //A01C10; PDBB4C8A0, sole virtual slot
 CBaseObjectPointer* PData;
 CBaseObject* GetPItem()const; //A01B50 PDBB4C7E0
 void SetPItem(CBaseObject*); //A01B90 PDBB4C820
};
struct ThingPointer:BaseIntelligentPointer {
 ThingPointer(){}
 ThingPointer(CThing* p){SetPItem((CBaseObject*)p);}
 // Implicit virtual destructor: VT1238C6C slot0=4A9710
 ThingPointer& operator=(const ThingPointer& p){SetPItem(p.GetPItem());return *this;}
};
struct DefNative {virtual ~DefNative();virtual void Delete()const;unsigned long IVRefCount;long GetRefCount()const{return (long)IVRefCount;}long Release()const{unsigned long r=--const_cast<DefNative*>(this)->IVRefCount;if(!r)Delete();return(long)r;}};
struct DefPointer {DefNative* Object;DefPointer():Object(0){}void Reset(){if(Object){DefNative* old=Object;if(--old->IVRefCount==0)old->Delete();Object=0;}Object=0;}};
struct LightningBeam;
struct ChildNode {ChildNode* Next;ChildNode* Prev;LightningBeam* Data;void* CountInfo;};
struct ChildList {ChildNode* Head;ChildList():Head(0){ChildNode* node=(ChildNode*)malloc(sizeof(ChildNode));node->Next=node;node->Prev=node;Head=node;}~ChildList(){free(Head);}};
struct Vector3 {float X,Y,Z;};
struct LightningBeam {
 long State;ThingPointer ParticleHandle,Home,Target;Vector3 Start,End,Destination;
 float DamagePerHit,Time,TimeBase;ChildList Children;bool Temporary,MainBeam,RightHand;
 float ActivationTime,DeactivationTime,Life,DecapitationTime,DecapitationClock,Delay;
 long OnhitEveryFrame,FrameCount;DefPointer PDef;bool SuccessfulHit;
 LightningBeam(CThing*,CThing*);
};
LightningBeam::LightningBeam(CThing* home,CThing* target){
 State=0;DamagePerHit=0;Time=0;TimeBase=0;
 Start.X=0;End.X=0;Destination.X=0;Start.Y=0;End.Y=0;Destination.Y=0;Start.Z=0;End.Z=0;Destination.Z=0;
 ActivationTime=0.2f;DeactivationTime=0.2f;Life=0;
 Target=ThingPointer(target);Home=ThingPointer(home);
 Temporary=false;MainBeam=false;RightHand=true;FrameCount=0;DecapitationTime=-1;DecapitationClock=0;Delay=0;
 PDef.Reset();OnhitEveryFrame=0;SuccessfulHit=false;
}
FABLE_STATIC_ASSERT(sizeof(ThingPointer)==8);
FABLE_STATIC_ASSERT(sizeof(LightningBeam)==0x7c);
FABLE_STATIC_ASSERT(offsetof(LightningBeam,Children)==0x4c);
FABLE_STATIC_ASSERT(offsetof(LightningBeam,PDef)==0x74);
}