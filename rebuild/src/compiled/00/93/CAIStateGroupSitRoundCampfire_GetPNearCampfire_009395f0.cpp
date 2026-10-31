// 009395f0 CAIStateGroup_SitRoundCampfire::GetPNearCampfire
// Leaf: ?GetPNearCampfire@CAIStateGroup_SitRoundCampfire@@ABEPAVCThingObject@@XZ
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
#pragma pack(push,1)
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
class CThing:public CClass {
public:const C3DVector&GetPos()const;
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
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
class CAIStateGroupBase:public CBaseClass {
protected:CAIBrain*PBrain;CThingAICreature&GetCreature()const{return PBrain->GetCreature();}
 const CThingSearchTools&PeekThingSearchTools()const;
private:unsigned char unreadGroup[sizeof(CAIStateGroupBaseLayout)-8];
};
class CThingObject:public CThingGameObject{};
class CThingCompare_Nearest{private:C3DVector Pos;public:CThingCompare_Nearest(const C3DVector&p):Pos(p){}};
class CIsNearCampfireWithSpace:public std::unary_function<CThing,bool>{
private:float DistanceSq;C3DVector FromPos;
public:CIsNearCampfireWithSpace(float distance,const C3DVector&p):FromPos(p){DistanceSq=distance*distance;}
};
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
class CThingSearchTools{private:unsigned char unobservedSearch[sizeof(CThingSearchToolsLayout)];
public:template<class Filter,class Compare,class T>bool GetPBestLeafThing(const Filter&,const Compare&,T**)const;
};
#define CAIStateGroup_SitRoundCampfire CAIStateGroup_SitRoundCampfireLayout
#include "engine/CAIStateGroup_SitRoundCampfire.h"
#undef CAIStateGroup_SitRoundCampfire
class CAIStateGroup_SitRoundCampfire:public CAIStateGroupBase{
private:unsigned char unobservedCampfire[sizeof(CAIStateGroup_SitRoundCampfireLayout)-sizeof(CAIStateGroupBase)];CThingObject*GetPNearCampfire()const;
};
// Analysis context only: a visible input-reading, conditional OUT stand-in.
// Not offered as a reconstruction of the called search helper.
template<> __declspec(noinline) bool CThingSearchTools::GetPBestLeafThing<CIsNearCampfireWithSpace,CThingCompare_Nearest,CThingObject>(const CIsNearCampfireWithSpace&filter,const CThingCompare_Nearest&compare,CThingObject**result)const {
 const unsigned char*f=reinterpret_cast<const unsigned char*>(&filter);
 const unsigned char*c=reinterpret_cast<const unsigned char*>(&compare);
 unsigned int any=0;for(unsigned int i=0;i<sizeof(filter);++i)any|=f[i];
 for(unsigned int j=0;j<sizeof(compare);++j)any|=c[j];
 if(any==0)*result=0;
 return *result!=0;
}
CThingObject*CAIStateGroup_SitRoundCampfire::GetPNearCampfire()const{
 CThingObject*campfire=0;CThingCompare_Nearest compare(GetCreature().GetPos());CIsNearCampfireWithSpace filter(22.0f,GetCreature().GetPos());
 PeekThingSearchTools().GetPBestLeafThing(filter,compare,&campfire);return campfire;
}