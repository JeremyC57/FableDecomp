// 00933db0 CAIStateGroup_MinionKidAnnoy::Update
// Codex parked-50 best attempt: DIFFER(347v347); residue: 347/347 bytes; incremental radius accumulation removes four excess bytes; remaining 14 normalized diff lines require scheduling/register review.
// Leaf: ?Update@CAIStateGroup_MinionKidAnnoy@@UAEXXZ
// Flags:
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CThingPhysical CThingPhysicalLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureLayout
#include "engine/CThingAICreature.h"
#undef CThingAICreature

#pragma pack(push,1)
class C3DVector {
public:float X,Y,Z;
    C3DVector(){}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
    const C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
    const C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
};
#pragma pack(pop)
namespace std {
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
}
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CSuspendableProcessMicrothread.h"
#include "engine/CBaseClassNonCopyable.h"
#pragma pack(push,1)
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
class CTCPhysicsBase;
class CThing:public CClass {
 unsigned char unreadThing[0x60-sizeof(CClass)];CTCPhysicsBase*PhysicsTC;
 unsigned char unreadThing2[0x91-0x64];bool Dead:1;unsigned char unreadFlags:7;
 unsigned char unreadTail[sizeof(CThingLayout)-0x92];
public:const C3DVector&GetPos()const;bool IsDead()const{return Dead;}
 CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}const CTCPhysicsBase*GetPhysicsTC()const{return PhysicsTC;}
};
class CThingPhysical:public CThing {public:const C3DVector&PeekPos()const;
private:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {private:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {public:bool IsThingInFOV(const CThing&,bool)const;
private:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingAICreature:public CThingCreatureBase {private:unsigned char unreadAI[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];};
#pragma pack(pop)
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
class CMainGameComponent;class CThingSearchTools;
// Layout: original microthread base20; private Component reference20, Creature24.
class CAIBrain:public CSuspendableProcessMicrothread {
 const CMainGameComponent&Component;CThingAICreature&Creature;
 unsigned char unreadBrain[sizeof(CAIBrainLayout)-28];
public:CThingAICreature&GetCreature()const{return Creature;}
};

#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;};
// Layout: original physics prefix12 and Radius52, native caller reads +34.
class CTCPhysicsBase:public CTCBase{protected:unsigned char unreadPhysics[0x34-0xc];float Radius;
public:float GetRadius()const{return Radius;}};
class CBaseIntelligentPointer{CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{
public:T*Get()const{return static_cast<T*>(GetPItem());}T*operator->()const{return Get();}
 template<class U>__forceinline CIntelligentPointer(CIntelligentPointer<U>&other){SetPItem(other.Get());}
};
template<class T>inline bool __fastcall IS_THING_ALIVE(const T&p){return p.Get()!=0&&!p->IsDead();}
#define CCreatureActionBase CCreatureActionBaseLayout
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
class CCreatureActionBase:public CBaseObject{unsigned char unreadAction[0xa8-sizeof(CBaseObject)];public:virtual~CCreatureActionBase();};
class CCreatureAction_KickCreature:public CCreatureActionBase{
 CIntelligentPointer<CThingCreatureBase>PTarget;
public:CCreatureAction_KickCreature(CThingCreatureBase&,CThingCreatureBase*);
};
enum EAIActionReturn{};enum EAIActionRepeat{};enum EMovementType{};enum EScriptAIPriority{};enum EAIStateGroupType{};
class CCharString;
class CAIStateGroupBase:public CBaseClass{
protected:CAIBrain*PBrain;CThingAICreature&GetCreature()const{return PBrain->GetCreature();}
 EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat,CIntelligentPointer<const CThing>,float,bool&,EMovementType,bool,bool,bool,bool,float);
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&,bool);
private:unsigned char unreadGroup[sizeof(CAIStateGroupBaseLayout)-8];
public:void Terminate();
 virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);
 virtual void OnActivate();virtual bool InitCreaturePosition();virtual bool AllowPriorityOverlap();virtual bool IsStillValid()const;
 virtual const CCharString*GetDebugText()const;virtual bool IsTimingCritical()const;virtual EAIStateGroupType GetStateGroupType()const;
 virtual void Cleanup();virtual void OnReplace();virtual bool IsRunnable();virtual void Update();
};
#define CAIStateGroup_MinionKidAnnoy CAIStateGroup_MinionKidAnnoyLayout
#include "engine/CAIStateGroup_MinionKidAnnoy.h"
#undef CAIStateGroup_MinionKidAnnoy
class CAIStateGroup_MinionKidAnnoy:public CAIStateGroupBase{
 CIntelligentPointer<CThingCreatureBase>PTarget;
public:virtual void Update();
};
void CAIStateGroup_MinionKidAnnoy::Update(){
 for(;;){
  if(!IS_THING_ALIVE(PTarget))break;
  float distanceSquared;
  {
   CThingCreatureBase&creature=GetCreature();
   const C3DVector&target=PTarget->GetPos();const C3DVector&me=creature.GetPos();
   float dx=me.X-target.X;float dy=me.Y-target.Y;distanceSquared=dx*dx+dy*dy;
  }
  if(!(distanceSquared<81.0f))break;
  const CTCPhysicsBase&physics=*GetCreature().GetPhysicsTC();
  float proximity=PTarget->GetPhysicsTC()->GetRadius();
  proximity+=physics.GetRadius();
  bool reached;
  EAIActionReturn result=AIAction_MoveToThingProximity((EAIActionRepeat)2,CIntelligentPointer<const CThing>(PTarget),proximity,reached,(EMovementType)5,true,false,true,true,0.0f);
  if(result==4)continue;if(result!=2)break;if(!IS_THING_ALIVE(PTarget))continue;
  CThingCreatureBase&annoyer=GetCreature();
  EAIActionReturn kicked=AIAction_PerformActionAndWait(CCreatureAction_KickCreature(annoyer,PTarget.Get()),false);
  if(kicked!=2)break;
 }
 Terminate();
}
