// 008385b0 CThingNoise::Create (catalog: Create; symbol of caller 00662110, bsim CThingNoise::Create)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <new>
class C3DVector { public:float x,y,z;};
class CRightHandedSet { public:float x,y,z,a,b,c;CRightHandedSet():x(0),y(0),z(1),a(0),b(1),c(0){}};
class CBaseClass { public:CBaseClass();virtual ~CBaseClass();};
class CTCParamsPhysicsBase:public CBaseClass { public:C3DVector position;CRightHandedSet axes;CTCParamsPhysicsBase(const C3DVector&,const CRightHandedSet&);};
namespace NFactory008385b0 {
struct Physics {virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void Set(const CTCParamsPhysicsBase&)=0;};struct Component {void Set(float);};struct Pair {int key;Component* component;};struct Map {Pair *first,*last,*end;__declspec(noinline) Pair* LowerBound(const int& key){Pair*p=first;int n=last-first;while(n>0){int half=n/2;Pair*mid=p+half;if(mid->key<key){p=mid+1;n-=half+1;}else n=half;}return p;}};
extern const float Epsilon;
struct Defs {
virtual void v0()=0;
virtual void v1()=0;
virtual void v2()=0;
virtual void v3()=0;
virtual void v4()=0;
virtual void v5()=0;
virtual void v6()=0;
virtual void v7()=0;
virtual void v8()=0;
virtual void v9()=0;
virtual void v10()=0;
virtual void v11()=0;
virtual void v12()=0;
virtual void v13()=0;
virtual void v14()=0;
virtual void v15()=0;
virtual int Get(const C3DVector&)=0;};struct Global {char pad[40];Defs* defs;};extern Global* global;
}
using namespace NFactory008385b0;
class CThing { public:virtual ~CThing();char pad[32];unsigned flags;char gap[28];Map map;char gap2[16];Physics* physics;__forceinline Map& GetComponents(){return map;}CThing(unsigned long);static void* __fastcall operator new(unsigned int,bool);bool ConstructFromParams(long,long,long,bool);bool DoActivationSequence();char tail[52];};
class CThingNoise:public CThing { public:CThingNoise():CThing(14){}virtual ~CThingNoise();static CThingNoise* __fastcall Create(long,const C3DVector&,float);};
// Layout: allocation0x98, flags+0x24, components+0x44, physics+0x60.
CThingNoise* __fastcall CThingNoise::Create(long owner,const C3DVector& position,float duration){CThingNoise* result=0;CThingNoise* p=(CThingNoise*)CThing::operator new(0x98,false);if(p){::new(p)CThingNoise;if(p->ConstructFromParams(owner,5,global->defs->Get(position),true)){Physics* physics=p->physics;physics->Set(CTCParamsPhysicsBase(position,CRightHandedSet()));if(p->DoActivationSequence()){if(-Epsilon<duration){Component* c;if(p->flags&0x800000){int key=55;Map&components=p->GetComponents();Pair* i=components.LowerBound(key);if(i==components.last||key<i->key)i=components.last;c=i->component;}c->Set(duration);}result=p;}}}return result;}


#include <assert.h>
#include <stdio.h>
#include <stddef.h>
namespace NFactory008385b0 {
const float Epsilon=0.125f;bool allocates=false,initializes=true,activates=true;int sets=0,matrices=0,componentSets=0;float lastDuration=0;
union Storage{double align;char bytes[0x98];}storage;
struct TestPhysics:Physics{void v0(){}void v1(){}void v2(){}void Set(const CTCParamsPhysicsBase&m){assert(matrices==1&&m.position.x==2&&m.position.y==3&&m.position.z==4&&m.axes.z==1&&m.axes.b==1);++sets;}}physicsImpl;
Component component;Pair pairs[2];
struct TestDefs:Defs{void v0(){}void v1(){}void v2(){}void v3(){}void v4(){}void v5(){}void v6(){}void v7(){}void v8(){}void v9(){}void v10(){}void v11(){}void v12(){}void v13(){}void v14(){}void v15(){}int Get(const C3DVector& p){assert(p.x==2&&p.y==3&&p.z==4);return 7;}}defs;
Global g;Global* global=&g;
}
using namespace NFactory008385b0;
void* __fastcall CThing::operator new(unsigned n,bool b){assert(n==0x98&&!b);return allocates?storage.bytes:0;}
CThing::CThing(unsigned long n){assert(n==14);flags=0x800000;physics=&physicsImpl;map.first=pairs;map.last=pairs+1;map.end=pairs+2;}
bool CThing::ConstructFromParams(long a,long type,long id,bool b){assert(a==1&&type==5&&id==7&&b);return initializes;}bool CThing::DoActivationSequence(){return activates;}
void NFactory008385b0::Component::Set(float f){assert(this==&component);lastDuration=f;++componentSets;}

CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){--matrices;}CThing::~CThing(){}CThingNoise::~CThingNoise(){}
CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector& p,const CRightHandedSet& a):position(p),axes(a){assert(p.x==2&&p.y==3&&p.z==4&&a.x==0&&a.y==0&&a.z==1&&a.a==0&&a.b==1&&a.c==0);++matrices;}
int main(){using namespace NFactory008385b0;assert(sizeof(CThingNoise)==0x98&&sizeof(CTCParamsPhysicsBase)==40&&offsetof(CThing,flags)==0x24&&offsetof(CThing,map)==0x44&&offsetof(CThing,physics)==0x60);Pair search[3];search[0].key=-7;search[1].key=4;search[2].key=55;Map sorted;sorted.first=search;sorted.last=sorted.end=search+3;int key=-8;assert(sorted.LowerBound(key)==search);key=4;assert(sorted.LowerBound(key)==search+1);key=20;assert(sorted.LowerBound(key)==search+2);key=56;assert(sorted.LowerBound(key)==search+3);assert(key==56);sorted.last=sorted.first;assert(sorted.LowerBound(key)==search);C3DVector position={2,3,4};g.defs=&defs;pairs[0].key=55;pairs[0].component=&component;pairs[1].key=0;pairs[1].component=0;assert(!CThingNoise::Create(1,position,3));allocates=true;initializes=false;assert(!CThingNoise::Create(1,position,3)&&sets==0);((CThingNoise*)storage.bytes)->~CThingNoise();initializes=true;activates=false;assert(!CThingNoise::Create(1,position,3)&&sets==1&&matrices==0);((CThingNoise*)storage.bytes)->~CThingNoise();activates=true;assert(CThingNoise::Create(1,position,3)==(CThingNoise*)storage.bytes&&componentSets==1&&lastDuration==3);((CThingNoise*)storage.bytes)->~CThingNoise();assert(CThingNoise::Create(1,position,-1)==(CThingNoise*)storage.bytes&&componentSets==1&&matrices==0);((CThingNoise*)storage.bytes)->~CThingNoise();puts("008385b0_TEST PASS");return 0;}