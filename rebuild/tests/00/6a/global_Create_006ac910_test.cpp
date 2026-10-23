// 006ac910 NFactory006ac910::Create
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <new>
namespace NFactory006ac910 {
// Layout: factory object physics pointer +0x60; temporary orientation two three-float vectors.
struct V {float x,y,z;__forceinline V(float a,float b,float c):x(a),y(b),z(c){}};
struct Axes {V forward,up;__forceinline Axes():forward(0,0,1),up(0,1,0){}};
// Layout: 6a06e0 constructs a polymorphic 40-byte object: base vptr0, position4, axes16; base destructor99a300.
struct MatrixBase {virtual ~MatrixBase();};
struct Matrix:MatrixBase {V position;Axes axes;Matrix(const V&,const Axes&);};
struct Physics {virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void Set(const Matrix&)=0;};
void* __fastcall Allocate(unsigned charSize,bool);
struct String {void* data;String(const char*,int=-1);~String();};
void __fastcall Notify(const String&,bool,float,int);
struct Extra {void Set(void*);};struct Handle {virtual void v0()=0;virtual void* Get()=0;};
struct Object {char prefix[96];Physics* physics;char gap[108];Extra extra;Object();bool Init(int,const V&,int,int);bool Init8(int,int,int,int,int,int,int,int);bool Activate();void Add(const String&,int,int);};
Object* __fastcall Create(int a,const V& position,int b,int c){Object* result=0;Object* p=new(Allocate(0x208,false))Object;if(p&&p->Init(a,position,b,c)){Notify("CThingPlayerCreature::Create 1",false,-1.0f,0);Physics* physics=p->physics;physics->Set(Matrix(position,Axes()));if(p->Activate())result=p;Notify("CThingPlayerCreature::Create 2",false,-1.0f,0);}return result;}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace NFactory006ac910 {
union Storage {double align;char bytes[0x208];} storage;
bool allocate=false,init=true,activate=true;int notices=0,sets=0,matrices=0;
struct TestPhysics:Physics{void v0(){}void v1(){}void v2(){}void Set(const Matrix& m){assert(matrices==1&&m.position.x==2&&m.position.y==5&&m.position.z==8&&m.axes.forward.z==1&&m.axes.up.y==1);++sets;}} physicsImpl;
void* __fastcall Allocate(unsigned n,bool b){assert(n==0x208&&!b);return allocate?storage.bytes:0;}
Matrix::Matrix(const V& p,const Axes& a):position(p),axes(a){assert(a.forward.x==0&&a.forward.y==0&&a.forward.z==1&&a.up.x==0&&a.up.y==1&&a.up.z==0);++matrices;}MatrixBase::~MatrixBase(){--matrices;}
String::String(const char* s,int n):data((void*)s){assert(n==-1);}String::~String(){}
void __fastcall Notify(const String& s,bool b,float f,int n){assert(!b&&f==-1&&n==0);assert(strcmp((char*)s.data,(notices%2)?"CThingPlayerCreature::Create 2":"CThingPlayerCreature::Create 1")==0);++notices;}
Object::Object():physics(&physicsImpl){}bool Object::Init(int a,const V& p,int b,int c){assert(a==1&&p.x==2&&p.y==5&&p.z==8&&b==3&&c==4);return init;}bool Object::Activate(){return activate;}
}
int main(){using namespace NFactory006ac910;assert(sizeof(Matrix)==40);V position(2,5,8);assert(!Create(1,position,3,4));allocate=true;init=false;assert(!Create(1,position,3,4)&&notices==0);init=true;assert(Create(1,position,3,4)==(Object*)storage.bytes&&notices==2&&sets==1&&matrices==0);activate=false;assert(!Create(1,position,3,4)&&notices==4&&sets==2&&matrices==0);puts("006ac910_TEST PASS");return 0;}