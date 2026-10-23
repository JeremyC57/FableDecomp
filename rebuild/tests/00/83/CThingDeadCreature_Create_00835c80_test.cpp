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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
namespace ExternalRetry_00835c80 {
const char Empty[]="";int StringCount=0;CTCParamsPhysicsBase::~CTCParamsPhysicsBase(){}CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CCharStringData* CCharString::Allocate(const char* p,int n){if(n==-1)n=(int)strlen(p);CCharStringData* d=new CCharStringData;d->text=static_cast<char*>(operator new[](n+1));memcpy(d->text,p,n);d->text[n]=0;d->size=n;d->capacity=n+1;d->flags=1;d->refs=1;return d;}
static bool allocate=true,init=true,activate=true;static int added=0,sets=0,filterCalls=0,segments=0,dead=0,constructed=0;static CThingDeadCreature* latest=0;
static C3DVector position={2,-4,8};
struct TestPhysics {virtual void v0(){}virtual void v1(){}virtual void v2(){}virtual void ConstructFromParams(const CTCParamsPhysicsBase&m){assert(m.Position.X==2&&m.Position.Y==-4&&m.Position.Z==8);const CRightHandedSet&a=m.Rhs;assert(a.Up.X==0&&a.Up.Y==0&&a.Up.Z==1&&a.Forward.X==0&&a.Forward.Y==1&&a.Forward.Z==0);++sets;}};static TestPhysics physicsCallbacks;static CTCPhysicsBaseDispatch physicsImpl;
struct Tracked:CAnimFilterBase {Tracked(){++segments;}bool operator()(const CAnimationEntry&)const{return false;}CAnimFilterBase*Clone()const{return new Tracked;}~Tracked(){--segments;++dead;}};
struct TestAnimFilter:CAnimFilterBase {bool operator()(const CAnimationEntry&)const{return false;}CAnimFilterBase*Clone()const{++filterCalls;return new Tracked;}};static TestAnimFilter handle;
void* __fastcall operator_new(unsigned n,bool flag){assert(n==0xf0&&!flag);latest=allocate?static_cast<CThingDeadCreature*>(malloc(n)):0;return latest;}
CThingDeadCreature::CThingDeadCreature(){memset(this,0,sizeof(*this));physics=&physicsImpl;++constructed;}
bool CThingDeadCreature::ConstructFromParams(int a,const C3DVector& p,int b,unsigned flags,const CCharString& name,CThingDeadCreature* x,CThingDeadCreature* y,bool enabled){assert(a==123&&b==456&&flags==0&&!x&&!y&&enabled);assert(&p==&position&&p.X==2&&p.Y==-4&&p.Z==8);assert(!strcmp(name.Text(),"corpseA")&&name.data->refs==1);return init;}
void CThingDeadCreature::AddTC(const CCharString& s,int a,int b){static const char* names[]={"CTCPhysicsStandard","CTCGraphicAppearance","CTCAnimationComplex","CTCHitLocations","CTCSkeletalMorph"};assert(a==0&&b==0&&!strcmp(s.Text(),names[added%5])&&s.data->refs==1);++added;}
bool CThingDeadCreature::DoActivationSequence(){return activate;}
static void Cleanup(CThingDeadCreature* p){if(p){p->extra.Reset(0);p->~CThingDeadCreature();free(p);latest=0;}}
}
int main(){using namespace ExternalRetry_00835c80;physicsImpl.__vftable=*(void**)&physicsCallbacks;assert(sizeof(CThingDeadCreature)==240&&offsetof(CThingDeadCreature,extra)==0xd0&&offsetof(CThingDeadCreature,physics)==0x60&&sizeof(CTCParamsPhysicsBase)==40&&sizeof(CCPPointerInfo)==12);
{CCharString name("corpseA");allocate=false;assert(!Create(123,position,456,name,handle)&&constructed==0&&added==0);allocate=true;init=false;assert(!Create(123,position,456,name,handle)&&constructed==1&&added==0);Cleanup(latest);init=true;
CThingDeadCreature* o=Create(123,position,456,name,handle);assert(o==latest&&added==5&&sets==1&&filterCalls==1&&segments==1&&StringCount==1);assert(o->extra.info&&o->extra.info->refs==1&&o->extra.info->object==o->extra.data);CCountedPointer share=o->extra;++share.info->refs;Cleanup(o);assert(share.info->refs==1&&segments==1);share.Reset(0);assert(segments==0&&dead==1);
activate=false;assert(!Create(123,position,456,name,handle)&&added==10&&sets==2&&segments==1);Cleanup(latest);assert(segments==0&&dead==2);
activate=true;o=Create(123,position,456,name,handle);const CAnimFilterBase* replacement=new Tracked;o->extra.Reset(replacement);assert(dead==3&&segments==1&&o->extra.data==replacement);Cleanup(o);assert(dead==4&&segments==0);assert(name.data->refs==1&&StringCount==1);
}
assert(StringCount==0&&constructed==4&&added==15&&sets==3&&filterCalls==3&&segments==0);puts("00835c80_TEST PASS");return 0;}