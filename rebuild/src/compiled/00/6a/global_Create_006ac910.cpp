// 006ac910 CThingPlayerCreature::Create (catalog: Create; the row's own progress strings name it)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Create
#include <new>
namespace NFactory006ac910 {
// Layout: factory object physics pointer +0x60; temporary orientation two three-float vectors.
struct C3DVector {float x,y,z;__forceinline C3DVector(float a,float b,float c):x(a),y(b),z(c){}};
struct CRightHandedSet {C3DVector forward,up;__forceinline CRightHandedSet():forward(0,0,1),up(0,1,0){}};
// Layout: 6a06e0 constructs a polymorphic 40-byte object: base vptr0, position4, axes16; base destructor99a300.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CTCParamsPhysicsBase:CBaseClass {C3DVector position;CRightHandedSet axes;CTCParamsPhysicsBase(const C3DVector&,const CRightHandedSet&);};
struct Physics {virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void Set(const CTCParamsPhysicsBase&)=0;};
void* __fastcall Allocate(unsigned charSize,bool);
struct CCharString {void* data;CCharString(const char*,int=-1);~CCharString();};
void __fastcall DisplayProgress(const CCharString&,bool,float,int);
struct Extra {void Set(void*);};struct Handle {virtual void v0()=0;virtual void* Get()=0;};
struct CThingPlayerCreature {char prefix[96];Physics* physics;char gap[108];Extra extra;CThingPlayerCreature();bool ConstructFromParams(int,const C3DVector&,int,int);bool Init8(int,int,int,int,int,int,int,int);bool DoActivationSequence();void Add(const CCharString&,int,int);static CThingPlayerCreature* __fastcall Create(int,const C3DVector&,int,int);};
CThingPlayerCreature* __fastcall CThingPlayerCreature::Create(int a,const C3DVector& position,int b,int c){CThingPlayerCreature* result=0;CThingPlayerCreature* p=new(Allocate(0x208,false))CThingPlayerCreature;if(p&&p->ConstructFromParams(a,position,b,c)){DisplayProgress("CThingPlayerCreature::Create 1",false,-1.0f,0);Physics* physics=p->physics;physics->Set(CTCParamsPhysicsBase(position,CRightHandedSet()));if(p->DoActivationSequence())result=p;DisplayProgress("CThingPlayerCreature::Create 2",false,-1.0f,0);}return result;}
}