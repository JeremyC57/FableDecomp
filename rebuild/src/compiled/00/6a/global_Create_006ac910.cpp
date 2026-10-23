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