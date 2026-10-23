// 008385b0 NFactory008385b0::Create
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <new>
namespace NFactory008385b0 {
struct Axes {float x,y,z,a,b,c;Axes():x(0),y(0),z(1),a(0),b(1),c(0){}};struct Vec {float x,y,z;};struct ParamBase {ParamBase();virtual ~ParamBase();};struct Matrix:ParamBase {Vec position;Axes axes;Matrix(const Vec&,const Axes&);};
struct Physics {virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void Set(const Matrix&)=0;};struct Component {void Set(float);};struct Pair {int key;Component* component;};struct Map {Pair *first,*last,*end;__declspec(noinline) Pair* Lower(const int& key){Pair*p=first;int n=last-first;while(n>0){int half=n/2;Pair*mid=p+half;if(mid->key<key){p=mid+1;n-=half+1;}else n=half;}return p;}};
extern const float Epsilon;
struct ThingBase {virtual ~ThingBase();char pad[32];unsigned flags;char gap[28];Map map;char gap2[16];Physics* physics;__forceinline Map& GetComponents(){return map;}ThingBase(int);bool Init(int,int,int,bool);bool Activate();char tail[52];};struct Object:ThingBase {Object():ThingBase(14){}virtual ~Object();};
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
virtual int Get(const Vec&)=0;};struct Global {char pad[40];Defs* defs;};extern Global* global;void* __fastcall Allocate(unsigned,bool);
// Layout: allocation0x98, flags+0x24, components+0x44, physics+0x60.
Object* __fastcall Create(int owner,const Vec& position,float duration){Object* result=0;Object* p=(Object*)Allocate(0x98,false);if(p){new(p)Object;if(p->Init(owner,5,global->defs->Get(position),true)){Physics* physics=p->physics;physics->Set(Matrix(position,Axes()));if(p->Activate()){if(-Epsilon<duration){Component* c;if(p->flags&0x800000){int key=55;Map&components=p->GetComponents();Pair* i=components.Lower(key);if(i==components.last||key<i->key)i=components.last;c=i->component;}c->Set(duration);}result=p;}}}return result;}
}