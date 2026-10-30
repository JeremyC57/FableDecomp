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
class C3DVector{public:float X,Y,Z;__forceinline C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
#pragma pack(pop)
class CThing;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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