#include <stdio.h>
#include <string.h>
#include <new>
// 0065dce8 CVillageTaskSecuritySweep::CVillageTaskSecuritySweep
// Flags: /Os
// Leaf: ??0CVillageTaskSecuritySweep@@QAE@ABVC3DVector@@M0M@Z
#define CVillageTaskSecuritySweep Pass9SecurityTaskStorage
#include "engine/CVillageTaskSecuritySweep.h"
#undef CVillageTaskSecuritySweep
#ifndef _MSC_VER
#define __forceinline inline
#endif
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}__forceinline C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
#pragma pack(pop)
class CThing;
class CBaseClass{public:virtual~CBaseClass();};
class CBaseObject:public CBaseClass{unsigned char pass9_unread_pointer[4];protected:CBaseObject();virtual~CBaseObject();public:virtual void Validate()const;};
enum EVillageTaskType{};
class CVillageTaskBase:public CBaseObject{public:
 __forceinline CVillageTaskBase():CBaseObject(){}virtual~CVillageTaskBase();
 virtual EVillageTaskType GetTaskType()const=0;
 virtual bool IsValid()const=0;
 virtual bool IsTaskAvailable()const=0;
 virtual bool IsThingInvolvedWithTask(const CThing&)const=0;
 virtual void RemoveThingFromTask(const CThing&,bool&)=0;
 virtual bool IsCreatureAssignedToTask(const CThing&)const=0;
 virtual void AssignCreatureToTask(const CThing&)=0;
};
class CBaseIntelligentPointer{public:virtual~CBaseIntelligentPointer();unsigned char PData[4];CBaseIntelligentPointer();};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:__forceinline CIntelligentPointer(){}};
// Layout: header and donor agree on52bytes; genuine task/object ancestry8,
// intelligentpointer8 at8, packedvectors16/32, floats28/44, flag48.
class CVillageTaskSecuritySweep:public CVillageTaskBase{public:
 CIntelligentPointer<const CThing>CreatureAssignedToTask;
 C3DVector PositionToGoTo;float RadiusAroundThePositionToStay;
 C3DVector PositionToLookTowards;float AngleToLookTowards;bool HasBecomeInvalid;
 virtual~CVillageTaskSecuritySweep();
 virtual EVillageTaskType GetTaskType()const;
 virtual bool IsValid()const;
 virtual bool IsTaskAvailable()const;
 virtual bool IsThingInvolvedWithTask(const CThing&)const;
 virtual void RemoveThingFromTask(const CThing&,bool&);
 virtual bool IsCreatureAssignedToTask(const CThing&)const;
 virtual void AssignCreatureToTask(const CThing&);
 CVillageTaskSecuritySweep(const C3DVector&,float,const C3DVector&,float);
};
CVillageTaskSecuritySweep::CVillageTaskSecuritySweep(const C3DVector&position,float radius,const C3DVector&lookTowards,float angle):CVillageTaskBase(),CreatureAssignedToTask(),PositionToGoTo(position),RadiusAroundThePositionToStay(radius),PositionToLookTowards(lookTowards),AngleToLookTowards(angle),HasBecomeInvalid(false){}

static unsigned int objectConstructed,pointerConstructed,objectDestroyed,pointerDestroyed;
CBaseClass::~CBaseClass(){}
CBaseObject::CBaseObject(){memset(pass9_unread_pointer,0x5a,4);++objectConstructed;}
CBaseObject::~CBaseObject(){++objectDestroyed;}
void CBaseObject::Validate()const{}
CVillageTaskBase::~CVillageTaskBase(){}
CBaseIntelligentPointer::CBaseIntelligentPointer(){memset(PData,0,4);++pointerConstructed;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++pointerDestroyed;}
CVillageTaskSecuritySweep::~CVillageTaskSecuritySweep(){}
EVillageTaskType CVillageTaskSecuritySweep::GetTaskType()const{return static_cast<EVillageTaskType>(4);}
bool CVillageTaskSecuritySweep::IsValid()const{return !HasBecomeInvalid;}
bool CVillageTaskSecuritySweep::IsTaskAvailable()const{return true;}
bool CVillageTaskSecuritySweep::IsThingInvolvedWithTask(const CThing&)const{return false;}
void CVillageTaskSecuritySweep::RemoveThingFromTask(const CThing&,bool&v){v=false;}
bool CVillageTaskSecuritySweep::IsCreatureAssignedToTask(const CThing&)const{return false;}
void CVillageTaskSecuritySweep::AssignCreatureToTask(const CThing&){}
int main(){
 const float values[3]={-4.0f,0.0f,3.0f},radii[5]={0.0f,0.5f,1.0f,8.0f,-2.0f},angles[4]={-4.0f,0.0f,1.0f,16.0f};
 unsigned int cases=0;
 for(int p=0;p<27;++p)for(int q=0;q<27;++q)for(int ri=0;ri<5;++ri)for(int ai=0;ai<4;++ai){
  C3DVector position(values[p%3],values[(p/3)%3],values[p/9]),look(values[q%3],values[(q/3)%3],values[q/9]);
  const C3DVector&target=(cases&1)?position:look;C3DVector posBefore(position),lookBefore(look);
  union FixtureStorage{double alignment;unsigned char bytes[84];}storage;
  memset(storage.bytes,0xa5,sizeof(storage.bytes));objectConstructed=pointerConstructed=objectDestroyed=pointerDestroyed=0;
  CVillageTaskSecuritySweep*task=new(storage.bytes+16)CVillageTaskSecuritySweep(position,radii[ri],target,angles[ai]);
  if(objectConstructed!=1||pointerConstructed!=1||objectDestroyed||pointerDestroyed)return 1;
  if(task->PositionToGoTo.X!=position.X||task->PositionToGoTo.Y!=position.Y||task->PositionToGoTo.Z!=position.Z)return 2;
  if(task->PositionToLookTowards.X!=target.X||task->PositionToLookTowards.Y!=target.Y||task->PositionToLookTowards.Z!=target.Z)return 3;
  if(task->RadiusAroundThePositionToStay!=radii[ri]||task->AngleToLookTowards!=angles[ai]||task->HasBecomeInvalid)return 4;
  for(int i=0;i<4;++i)if(task->CreatureAssignedToTask.PData[i])return 5;
  CVillageTaskBase*base=task;if(base->GetTaskType()!=static_cast<EVillageTaskType>(4)||!base->IsValid())return 6;
  if(memcmp(&position,&posBefore,12)||memcmp(&look,&lookBefore,12))return 7;
  for(int i=0;i<16;++i)if(storage.bytes[i]!=0xa5||storage.bytes[68+i]!=0xa5)return 8;
  task->~CVillageTaskSecuritySweep();if(objectDestroyed!=1||pointerDestroyed!=1)return 9;++cases;
 }
 if(cases!=14580)return 10;puts("0065dce8_TEST PASS");return 0;
}