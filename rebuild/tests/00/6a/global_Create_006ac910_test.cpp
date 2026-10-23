// 006ac910 CThingPlayerCreature::Create (catalog: Create; the row's own progress strings name it)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <new>
namespace NFactory006ac910 {
// Layout: factory object physics pointer +0x60; temporary orientation two three-float vectors.
struct C3DVector {float x,y,z;__forceinline C3DVector(float a,float b,float c):x(a),y(b),z(c){}};
struct CRightHandedSet {C3DVector forward,up;__forceinline CRightHandedSet():forward(0,0,1),up(0,1,0){}};
// Layout: 6a06e0 constructs a polymorphic 40-byte object: base vptr0, position4, axes16; base destructor99a300.
struct CBaseClass {virtual ~CBaseClass();};
struct CTCParamsPhysicsBase:CBaseClass {C3DVector position;CRightHandedSet axes;CTCParamsPhysicsBase(const C3DVector&,const CRightHandedSet&);};
struct Physics {virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void Set(const CTCParamsPhysicsBase&)=0;};
void* __fastcall Allocate(unsigned charSize,bool);
struct CCharString {void* data;CCharString(const char*,int=-1);~CCharString();};
void __fastcall DisplayProgress(const CCharString&,bool,float,int);
struct Extra {void Set(void*);};struct Handle {virtual void v0()=0;virtual void* Get()=0;};
struct CThingPlayerCreature {char prefix[96];Physics* physics;char gap[108];Extra extra;CThingPlayerCreature();bool ConstructFromParams(int,const C3DVector&,int,int);bool Init8(int,int,int,int,int,int,int,int);bool DoActivationSequence();void Add(const CCharString&,int,int);static CThingPlayerCreature* __fastcall Create(int,const C3DVector&,int,int);};
CThingPlayerCreature* __fastcall CThingPlayerCreature::Create(int a,const C3DVector& position,int b,int c){CThingPlayerCreature* result=0;CThingPlayerCreature* p=new(Allocate(0x208,false))CThingPlayerCreature;if(p&&p->ConstructFromParams(a,position,b,c)){DisplayProgress("CThingPlayerCreature::Create 1",false,-1.0f,0);Physics* physics=p->physics;physics->Set(CTCParamsPhysicsBase(position,CRightHandedSet()));if(p->DoActivationSequence())result=p;DisplayProgress("CThingPlayerCreature::Create 2",false,-1.0f,0);}return result;}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace NFactory006ac910 {
union Storage {double align;char bytes[0x208];} storage;
bool allocate=false,init=true,activate=true;int notices=0,sets=0,matrices=0;
struct TestPhysics:Physics{void v0(){}void v1(){}void v2(){}void Set(const CTCParamsPhysicsBase& m){assert(matrices==1&&m.position.x==2&&m.position.y==5&&m.position.z==8&&m.axes.forward.z==1&&m.axes.up.y==1);++sets;}} physicsImpl;
void* __fastcall Allocate(unsigned n,bool b){assert(n==0x208&&!b);return allocate?storage.bytes:0;}
CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector& p,const CRightHandedSet& a):position(p),axes(a){assert(a.forward.x==0&&a.forward.y==0&&a.forward.z==1&&a.up.x==0&&a.up.y==1&&a.up.z==0);++matrices;}CBaseClass::~CBaseClass(){--matrices;}
CCharString::CCharString(const char* s,int n):data((void*)s){assert(n==-1);}CCharString::~CCharString(){}
void __fastcall DisplayProgress(const CCharString& s,bool b,float f,int n){assert(!b&&f==-1&&n==0);assert(strcmp((char*)s.data,(notices%2)?"CThingPlayerCreature::Create 2":"CThingPlayerCreature::Create 1")==0);++notices;}
CThingPlayerCreature::CThingPlayerCreature():physics(&physicsImpl){}bool CThingPlayerCreature::ConstructFromParams(int a,const C3DVector& p,int b,int c){assert(a==1&&p.x==2&&p.y==5&&p.z==8&&b==3&&c==4);return init;}bool CThingPlayerCreature::DoActivationSequence(){return activate;}
}
int main(){using namespace NFactory006ac910;assert(sizeof(CTCParamsPhysicsBase)==40);C3DVector position(2,5,8);assert(!CThingPlayerCreature::Create(1,position,3,4));allocate=true;init=false;assert(!CThingPlayerCreature::Create(1,position,3,4)&&notices==0);init=true;assert(CThingPlayerCreature::Create(1,position,3,4)==(CThingPlayerCreature*)storage.bytes&&notices==2&&sets==1&&matrices==0);activate=false;assert(!CThingPlayerCreature::Create(1,position,3,4)&&notices==4&&sets==2&&matrices==0);puts("006ac910_TEST PASS");return 0;}