// 008385b0 CThingNoise::Create (catalog: Create; symbol of caller 00662110, bsim CThingNoise::Create)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <new>
struct C3DVector {float x,y,z;};
struct CRightHandedSet {float x,y,z,a,b,c;CRightHandedSet():x(0),y(0),z(1),a(0),b(1),c(0){}};
struct CBaseClass {CBaseClass();virtual ~CBaseClass();};
struct CTCParamsPhysicsBase:CBaseClass {C3DVector position;CRightHandedSet axes;CTCParamsPhysicsBase(const C3DVector&,const CRightHandedSet&);};
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
struct CThing {virtual ~CThing();char pad[32];unsigned flags;char gap[28];Map map;char gap2[16];Physics* physics;__forceinline Map& GetComponents(){return map;}CThing(unsigned long);static void* __fastcall operator new(unsigned int,bool);bool ConstructFromParams(long,long,long,bool);bool DoActivationSequence();char tail[52];};
struct CThingNoise:CThing {CThingNoise():CThing(14){}virtual ~CThingNoise();static CThingNoise* __fastcall Create(long,const C3DVector&,float);};
// Layout: allocation0x98, flags+0x24, components+0x44, physics+0x60.
CThingNoise* __fastcall CThingNoise::Create(long owner,const C3DVector& position,float duration){CThingNoise* result=0;CThingNoise* p=(CThingNoise*)CThing::operator new(0x98,false);if(p){::new(p)CThingNoise;if(p->ConstructFromParams(owner,5,global->defs->Get(position),true)){Physics* physics=p->physics;physics->Set(CTCParamsPhysicsBase(position,CRightHandedSet()));if(p->DoActivationSequence()){if(-Epsilon<duration){Component* c;if(p->flags&0x800000){int key=55;Map&components=p->GetComponents();Pair* i=components.LowerBound(key);if(i==components.last||key<i->key)i=components.last;c=i->component;}c->Set(duration);}result=p;}}}return result;}
