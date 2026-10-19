// CLookAtFrontInit::Alloc(CGameCameraManager&,const CDefPointer&) const -- create camera ahead of observed thing
// Native00873930 length221. Physics virtual70=GetFacingVector; graphics4C10E0 is no-argument focalpos value return.
// Required actual headers include quarantine; native projections below document donor layout differences.
#define CGameCameraManager CGameCameraManagerDonor
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CCameraModeDef CCameraModeDefDonor
#include "engine/_quarantine/CCameraModeDef.h"
#undef CCameraModeDef
#include "engine/CTCPhysicsBase.h"
#include "engine/_quarantine/CTCGraphicAppearance.h"
#include "engine/CBaseObject.h"
struct C3DVector {
 float X,Y,Z;
 C3DVector(){}
 C3DVector(const C3DVector& r):X(r.X),Y(r.Y),Z(r.Z){} // actual PDB owning scalar-copy ctor
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}
 C3DVector operator+(const C3DVector& r)const{C3DVector v;v.X=X+r.X;v.Y=Y+r.Y;v.Z=Z+r.Z;return v;}
};
#ifdef _MSC_VER
#pragma pointers_to_members(full_generality,single_inheritance)
#endif
struct PhysicsMethods;
union PhysicsSlot {void* Native;C3DVector(PhysicsMethods::*Facing)()const;};
struct PhysicsMethods:CTCPhysicsBase {
 __forceinline C3DVector GetFacingVector()const{return (this->*static_cast<PhysicsSlot*>(__vftable)[70].Facing)();} // native base6B1410/standard723C00
};
struct GraphicMethods:CTCGraphicAppearance {C3DVector GetFocalPos()const;}; // native004C10E0 (not the donor float overload)
struct ThingMethods {unsigned char BeforePhysics[0x60];PhysicsMethods* PhysicsTC;GraphicMethods* PTCGraphicAppearance;
 const C3DVector& GetPos()const; //004C73D0
}; // donor PhysicsTC70,graphics74 versus native60/64
struct IntelligentObserved {void* VTable;void* ReferenceInfo;ThingMethods* GetPItem()const;}; //00A01B50: object at referenceinfo0,refs4
struct CGameCameraManager {unsigned char BeforeObserved[0x100];IntelligentObserved PObservedThing;}; //donor110,native100
struct CCameraModeDef {unsigned char BaseDefinition[0x40];float StringLengthXY,CageRadius,HeightOffsetZ;}; //donor44/4C,native40/48
struct CDefPointer {const CCameraModeDef* Object;};
struct CBase {void* VTable;};
struct CSimplePointBase:CBase {
 unsigned char BaseCameraState[0xac];C3DVector Position,PointAt;bool Fixed;unsigned char Tail[3];
 CSimplePointBase(CGameCameraManager&,const CDefPointer&,const C3DVector&,const C3DVector&,bool); //00873A10,VT01277644
};
extern unsigned long g_LookAtFrontVTable; //01277714, RTTI confirms derived class
struct CLookAtFront:CSimplePointBase {
 CLookAtFront(CGameCameraManager& m,const CDefPointer& d,const C3DVector& p,const C3DVector& at,bool b):CSimplePointBase(m,d,p,at,b){VTable=&g_LookAtFrontVTable;} //inline ctor native00873AE0
};
struct CLookAtFrontInit {
 virtual CBase* Alloc(CGameCameraManager&,const CDefPointer&)const;
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(PhysicsSlot)==4);
FABLE_STATIC_ASSERT(sizeof(CLookAtFront)==0xcc);
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager,PObservedThing)==0x100);
FABLE_STATIC_ASSERT(offsetof(ThingMethods,PhysicsTC)==0x60);
FABLE_STATIC_ASSERT(offsetof(CCameraModeDef,StringLengthXY)==0x40);
#endif
CBase* CLookAtFrontInit::Alloc(CGameCameraManager& manager,const CDefPointer& definition)const {
 ThingMethods* thing=manager.PObservedThing.GetPItem();
 C3DVector forward=(thing->PhysicsTC->*static_cast<PhysicsSlot*>(thing->PhysicsTC->__vftable)[70].Facing)();
 float height=definition.Object->HeightOffsetZ;
 C3DVector position=forward*definition.Object->StringLengthXY+thing->GetPos();
 position.Z+=height;
 C3DVector focal=thing->PTCGraphicAppearance->GetFocalPos();
 return new CLookAtFront(manager,definition,position,focal,true);
}
