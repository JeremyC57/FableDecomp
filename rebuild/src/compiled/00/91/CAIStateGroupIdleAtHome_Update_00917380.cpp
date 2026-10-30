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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
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