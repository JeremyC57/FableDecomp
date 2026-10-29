#ifndef _MSC_VER
#define __int64 int __attribute__((mode(DI)))
#define __fastcall
#define __forceinline inline
#endif
#include <stddef.h>
#include <string.h>
extern "C" int puts(const char*);
// 00917380 CAIStateGroup_IdleAtHome::Update()
// Leaf: ?Update@CAIStateGroup_IdleAtHome@@MAEXXZ
// Flags:
// Layout: original inheritance with opaque unread state; Thing flags accessed only through documented physical alias.
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
    void Rotate(float,float);
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
class CBaseClass {public:virtual~CBaseClass();};
class CBaseObject:public CBaseClass {
public:virtual void Validate()const;
protected:virtual~CBaseObject();
private:unsigned char unobservedObjectState[4];
};
#pragma pack(push,1)
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
class CThing:public CClass {
public:const C3DVector&GetPos()const;
    static unsigned long __fastcall StatGetTypeFlags(){return 0;}
    bool IsDead()const{return (reinterpret_cast<const CThingLayout*>(this)->CreatedBySerialisation[0]&1)!=0;}
    unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingLayout*>(this)->ThingTypeFlags;}
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
};
class CThingPhysical:public CThing {public:const C3DVector&PeekPos()const;
private:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {private:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {public:bool IsThingInFOV(const CThing&,bool)const;
private:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingBuilding:public CThingGameObject {};
class CBaseIntelligentPointer {
public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;
private:unsigned char unobservedPointerState[4];
};
class CThingAICreature:public CThingCreatureBase {
public:CThingBuilding*GetHomeBuilding()const {
    return static_cast<CThingBuilding*>(reinterpret_cast<const CBaseIntelligentPointer*>(reinterpret_cast<const CThingAICreatureLayout*>(this)->HomeBuilding)->GetPItem());
}
private:unsigned char unobservedAIState[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
};
#pragma pack(pop)
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
class CCreatureActionBase;template<class T>class CIntelligentPointer;
class CAIBrain;class CWorld;class CCharString;class CThingBuilding;
enum EScriptAIPriority {};enum EAIStateGroupType {};enum EAIActionReturn {};enum EAIActionRepeat {};enum EMovementType {};
#pragma pack(push,1)
class CAIStateGroupBase:public CBaseClass {
public:
    void Terminate();
    virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);
    virtual void OnActivate();virtual bool InitCreaturePosition();virtual bool AllowPriorityOverlap();
    virtual bool IsStillValid()const;virtual const CCharString*GetDebugText()const;
    virtual bool IsTimingCritical()const;virtual EAIStateGroupType GetStateGroupType()const;
protected:
    virtual void Cleanup();virtual void OnReplace();virtual bool IsRunnable();virtual void Update()=0;
    virtual void InitStateGroup(const CWorld&,CThingAICreature&);
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat,C3DVector,float,bool&,EMovementType,bool,bool,bool,const CThing*,bool,float);

    unsigned long*GetPWorldSeed()const;
    EAIActionReturn AIAction_MinionIdle();
    CAIBrain*PBrain;
    unsigned char unobservedGroupState[sizeof(CAIStateGroupBaseLayout)-8];
    CThingAICreature&GetCreature()const{return *reinterpret_cast<const CAIBrainLayout*>(PBrain)->Creature;}
};
#pragma pack(pop)
#define CAIStateGroup_IdleAtHome CAIStateGroup_IdleAtHomeLayout
#include "engine/CAIStateGroup_IdleAtHome.h"
#undef CAIStateGroup_IdleAtHome
float __fastcall GFFloatRandom(float,unsigned long*);
class CAIStateGroup_IdleAtHome:public CAIStateGroupBase {
friend int main();
protected:virtual void Update();
};
void CAIStateGroup_IdleAtHome::Update()
{
    if(GetCreature().GetHomeBuilding()) {
        C3DVector home=GetCreature().GetHomeBuilding()->GetPos();
        C3DVector offset(GFFloatRandom(4.5f,GetPWorldSeed())+0.0001f,0.0f,0.0f);
        offset.Rotate(GFFloatRandom(1.0f,GetPWorldSeed()),0.0f);
        C3DVector target=offset+home;
        EAIActionReturn result;
        do {
            if(!GetCreature().GetHomeBuilding()){Terminate();return;}
            bool reached;
            result=AIAction_MoveToPosProximity((EAIActionRepeat)2,target,1.0f,reached,(EMovementType)3,true,false,true,0,true,0.0f);
        }while(result==4);
        if(result==2&&GetCreature().GetHomeBuilding())AIAction_MinionIdle();
    }
    Terminate();
}

static CThingAICreature*creature;static CThingBuilding*home;static CAIStateGroupBase*group;
static int bad,homeCalls,posCalls,seedCalls,randomCalls,rotateCalls,moveCalls,idleCalls,terminateCalls;
static bool initialHome,postVanish;static int progressCount,finalResult,vanishIteration;
static unsigned long seed;
static C3DVector homePosition;
CBaseClass::~CBaseClass(){}
CBaseObject::~CBaseObject(){}
void CAIStateGroupBase::Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&){}
void CAIStateGroupBase::OnActivate(){}
bool CAIStateGroupBase::InitCreaturePosition(){return false;}
bool CAIStateGroupBase::AllowPriorityOverlap(){return false;}
bool CAIStateGroupBase::IsStillValid()const{return false;}
const CCharString*CAIStateGroupBase::GetDebugText()const{return 0;}
bool CAIStateGroupBase::IsTimingCritical()const{return false;}
EAIStateGroupType CAIStateGroupBase::GetStateGroupType()const{return EAIStateGroupType(0);}
void CAIStateGroupBase::Cleanup(){}
void CAIStateGroupBase::OnReplace(){}
bool CAIStateGroupBase::IsRunnable(){return false;}
void CAIStateGroupBase::InitStateGroup(const CWorld&,CThingAICreature&){}

void CBaseObject::Validate()const{}

CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const {
 if(this!=reinterpret_cast<const CBaseIntelligentPointer*>(reinterpret_cast<const CThingAICreatureLayout*>(creature)->HomeBuilding))bad=1;
 ++homeCalls;
 if(!initialHome)return 0;
 if(homeCalls>2){
  int iteration=homeCalls-3;
  if(iteration==vanishIteration)return 0;
  if(iteration==progressCount+1&&postVanish)return 0;
 }
 return home;
}
const C3DVector&CThing::GetPos()const{if(this!=home||homeCalls!=2)bad=2;++posCalls;return homePosition;}
unsigned long*CAIStateGroupBase::GetPWorldSeed()const{if(this!=group)bad=3;++seedCalls;return &seed;}
float __fastcall GFFloatRandom(float range,unsigned long*p){
 if(p!=&seed||seedCalls!=randomCalls+1||range!=(randomCalls?1.0f:4.5f))bad=4;
 ++randomCalls;return randomCalls==1?2.0f:0.5f;
}
void C3DVector::Rotate(float horizontal,float vertical){
 if(horizontal!=0.5f||vertical!=0||randomCalls!=2||X<2.00009f||X>2.00011f||Y||Z)bad=5;
 ++rotateCalls;X=1.25f;Y=2.5f;Z=-3.75f;
}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat repeat,C3DVector target,float range,bool&reached,EMovementType movement,bool a,bool b,bool c,const CThing*ignore,bool e,float f){
 if(this!=group||(int)repeat!=2||(int)movement!=3||range!=1.0f||!a||b||!c||ignore||!e||f!=0||rotateCalls!=1)bad=6;
 if(target.X!=homePosition.X+1.25f||target.Y!=homePosition.Y+2.5f||target.Z!=homePosition.Z-3.75f)bad=7;
 reached=true;++moveCalls;
 if(moveCalls>progressCount+1){bad=8;return(EAIActionReturn)0;}
 return(EAIActionReturn)(moveCalls<=progressCount?4:finalResult);
}
EAIActionReturn CAIStateGroupBase::AIAction_MinionIdle(){if(this!=group||terminateCalls)bad=9;++idleCalls;return(EAIActionReturn)3;}
void CAIStateGroupBase::Terminate(){if(this!=group)bad=10;++terminateCalls;}
class FixtureGroup:public CAIStateGroup_IdleAtHome {public:void attach(CAIBrainLayout&brain){PBrain=reinterpret_cast<CAIBrain*>(&brain);}};
int main(){
 int cases=0;
 for(int initial=0;initial!=2;++initial)for(int n=0;n!=4;++n)for(int finish=0;finish!=4;++finish)for(int vanish=0;vanish<=n+2;++vanish)for(int post=0;post!=2;++post){
  CThingAICreature a;CThingBuilding b;CAIBrainLayout brain;brain.Creature=&a;FixtureGroup candidate;candidate.attach(brain);
  creature=&a;home=&b;group=&candidate;initialHome=initial!=0;progressCount=n;finalResult=finish;vanishIteration=vanish;postVanish=post!=0;
  homePosition=C3DVector(8.0f,-16.0f,4.5f);
  bad=homeCalls=posCalls=seedCalls=randomCalls=rotateCalls=moveCalls=idleCalls=terminateCalls=0;
  candidate.Update();
  bool lost=vanish<=n;int moves=initial?(lost?vanish:n+1):0;
  bool finishCheck=initial&&!lost&&finish==2;
  int gets=initial?2+(lost?vanish+1:n+1)+(finishCheck?1:0):1;
  // When vanish is n+1, the final home check also observes loss.
  bool idle=finishCheck&&!post&&vanish!=n+1;
  if(bad||terminateCalls!=1||moveCalls!=moves||homeCalls!=gets||posCalls!=initial||seedCalls!=2*initial||randomCalls!=2*initial||rotateCalls!=initial||idleCalls!=(int)idle)return 1;
  ++cases;
 }
 puts("00917380_TEST PASS");return cases==288?0:2;
}