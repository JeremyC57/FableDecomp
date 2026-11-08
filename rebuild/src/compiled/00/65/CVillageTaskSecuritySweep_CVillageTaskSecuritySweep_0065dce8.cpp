// 0065dce8 CVillageTaskSecuritySweep::CVillageTaskSecuritySweep
// Flags: /Os
// Leaf: ??0CVillageTaskSecuritySweep@@QAE@ABVC3DVector@@M0M@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "fable_compiler.h"
#define CVillageTaskSecuritySweep Pass9SecurityTaskStorage
#include "engine/CVillageTaskSecuritySweep.h"
#undef CVillageTaskSecuritySweep
#ifndef _MSC_VER
#define __forceinline inline
#endif
#pragma pack(push,1)
#include "engine/C3DVector.h"
#pragma pack(pop)
class CThing;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
enum EVillageTaskType{};
class CVillageTaskBase:public CBaseObject{public:
 FABLE_FORCEINLINE CVillageTaskBase():CBaseObject(){}virtual~CVillageTaskBase();
 virtual EVillageTaskType GetTaskType()const=0;
 virtual bool IsValid()const=0;
 virtual bool IsTaskAvailable()const=0;
 virtual bool IsThingInvolvedWithTask(const CThing&)const=0;
 virtual void RemoveThingFromTask(const CThing&,bool&)=0;
 virtual bool IsCreatureAssignedToTask(const CThing&)const=0;
 virtual void AssignCreatureToTask(const CThing&)=0;
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
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