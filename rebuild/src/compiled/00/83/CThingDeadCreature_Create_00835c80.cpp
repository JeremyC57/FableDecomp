// 00835c80 CThingDeadCreature::Create
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <new>
#include <stdlib.h>
#include "engine/CTCPhysicsBase.h"
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace ExternalRetry_00835c80 {
// Layout:835c20 allocates target240 and initializes counted filter+d0/d4; physics+60.
// No CThingDeadCreature header exists. Matrix6a06e0 is genuine vptr+Position12+CRightHandedSet24.
//836580 owns CCPPointerInfo12; callback8364b0 deletes CAnimFilterBase through destructor slot2.
struct C3DVector {float X,Y,Z;};
struct CRightHandedSet {C3DVector Up,Forward;CRightHandedSet(){Up.X=0;Up.Y=0;Up.Z=1;Forward.X=0;Forward.Y=1;Forward.Z=0;}};struct CBaseClass {virtual ~CBaseClass();};struct CBaseClassNonCopyable:CBaseClass {CBaseClassNonCopyable();__declspec(noinline) virtual ~CBaseClassNonCopyable(){}};struct CTCParamsPhysicsBase:CBaseClassNonCopyable {C3DVector Position;CRightHandedSet Rhs;__declspec(noinline) CTCParamsPhysicsBase(const C3DVector& p,const CRightHandedSet& a):Position(p),Rhs(a){}~CTCParamsPhysicsBase();};
// Active flattened header retained. Method-only adapter dispatches actual slot3 ConstructFromParams.
struct CTCPhysicsMethods {virtual void Slot0();virtual void Slot1();virtual void Slot2();virtual void ConstructFromParams(const CTCParamsPhysicsBase&);};
typedef ::CTCPhysicsBase CTCPhysicsBaseDispatch;
void* __fastcall operator_new(unsigned charSize,bool);
#pragma pack(push,1)
struct CCharStringData {char* text;int size;unsigned capacity;unsigned char flags;int refs;};
#pragma pack(pop)
extern const char Empty[];extern int StringCount;
struct CCharString {
 CCharStringData* data;
 __declspec(noinline) CCharString():data(0){++StringCount;}
 __declspec(noinline) CCharString(const char* text,int length=-1):data(0){++StringCount;if(text&&*text)data=Allocate(text,length);}
 __declspec(noinline) CCharString(const CCharString& x):data(0){++StringCount;if(this!=&x&&x.data){data=x.data;++data->refs;}}
 CCharStringData* Allocate(const char*,int);
 __declspec(noinline) void Clear(){if(data){if(--data->refs<=0){CCharStringData* p=data;if(p->text){operator delete[](p->text);p->text=0;}p->size=0;p->capacity&=0x80000000;delete p;}data=0;}}
 __declspec(noinline) ~CCharString(){Clear();--StringCount;}
 const char* Text()const{return !data?Empty:data->text;}
 int Size()const{return !data?0:data->size;}
};
struct CAnimationEntry;struct CAnimFilterBase {virtual bool operator()(const CAnimationEntry&)const=0;virtual CAnimFilterBase*Clone()const=0;virtual ~CAnimFilterBase(){}};
struct CCPPointerInfo {int refs;void (__fastcall* destroy)(const CAnimFilterBase*);const CAnimFilterBase* object;};
void __fastcall DeleteAnimFilter(const CAnimFilterBase* p){if(p)delete p;}
struct CCountedPointer {const CAnimFilterBase* data;CCPPointerInfo* info;__declspec(noinline) void Reset(const CAnimFilterBase* p){if(info&&!--info->refs){info->destroy(info->object);delete info;}info=0;data=p;if(p){info=new CCPPointerInfo;if(info){info->refs=1;info->destroy=DeleteAnimFilter;info->object=p;}}}};
struct CThingDeadCreature {char prefix[96];CTCPhysicsBaseDispatch* physics;char gap[108];CCountedPointer extra;char tail[24];CThingDeadCreature();bool ConstructFromParams(int,const C3DVector&,int,unsigned,const CCharString&,CThingDeadCreature*,CThingDeadCreature*,bool);bool DoActivationSequence();void AddTC(const CCharString&,int,int);};
CThingDeadCreature* __fastcall Create(int a,const C3DVector& position,int b,const CCharString& c,const CAnimFilterBase& h){CThingDeadCreature* result=0;CThingDeadCreature* p=(CThingDeadCreature*)operator_new(0xf0,false);if(p){p=new(p)CThingDeadCreature;if(p&&p->ConstructFromParams(a,position,b,0,c,0,0,1)){{CCharString component("CTCPhysicsStandard");p->AddTC(component,0,0);}{CCharString component("CTCGraphicAppearance");p->AddTC(component,0,0);}{CCharString component("CTCAnimationComplex");p->AddTC(component,0,0);}{CCharString component("CTCHitLocations");p->AddTC(component,0,0);}{CCharString component("CTCSkeletalMorph");p->AddTC(component,0,0);}p->extra.Reset(h.Clone());CTCPhysicsBaseDispatch* physics=p->physics;reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(CTCParamsPhysicsBase(position,CRightHandedSet()));if(p->DoActivationSequence())result=p;}}return result;}
}