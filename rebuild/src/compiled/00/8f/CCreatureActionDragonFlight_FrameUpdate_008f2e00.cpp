// CCreatureAction_DragonFlight::FrameUpdate() -- turn current facing and rebuild right-handed orientation before base update.
// Native008F2E00 length246. Actual physics header retained, field-free native ABI adapters;
// slot70 by-value GetFacingVector lowered to explicit result buffer (ECXthis,EAXbuffer,ret4),
// physics-standard723C00 verified; slot75 SetRHSet7239C0 copies24bytes,ret4.
#define CCreatureAction_DragonFlight CCreatureAction_DragonFlight_PDB
#include "engine/CCreatureAction_DragonFlight.h"
#undef CCreatureAction_DragonFlight
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CTCPhysicsBase.h"
struct C3DVector {
 float X,Y,Z;
 C3DVector(){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 void RotateXY(float turns); //00A14040 adds turns to atan2(X,Y), preserves inclination
 C3DVector Cross(const C3DVector& v) const {return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}
};
struct CRightHandedSet {C3DVector Up,Forward;CRightHandedSet(const C3DVector& u,const C3DVector& f):Up(u),Forward(f){}};
struct CPhysicsMethods : CTCPhysicsBase {
 const C3DVector& GetFacingVectorInto(C3DVector&) const;
 void SetRHSet(const CRightHandedSet&);
};
union CPhysicsSlot {
 void* Address;
 const C3DVector& (CPhysicsMethods::*GetFacingVectorInto)(C3DVector&) const;
 void (CPhysicsMethods::*SetRHSet)(const CRightHandedSet&);
};
inline const C3DVector& CPhysicsMethods::GetFacingVectorInto(C3DVector& out) const {return (this->*reinterpret_cast<CPhysicsSlot*>(__vftable)[70].GetFacingVectorInto)(out);}
inline void CPhysicsMethods::SetRHSet(const CRightHandedSet& value){(this->*reinterpret_cast<CPhysicsSlot*>(__vftable)[75].SetRHSet)(value);}
FABLE_STATIC_ASSERT(sizeof(CPhysicsSlot)==4);
FABLE_STATIC_ASSERT(sizeof(CPhysicsMethods)==sizeof(CTCPhysicsBase));
struct CThing {
 unsigned char _nativeBeforePhysics[0x60];
 CPhysicsMethods* PPhysics;
 __forceinline CPhysicsMethods* GetPhysics() const {return PPhysics;}
};
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer(); //00A01C10
 void* PData;
 CThing* GetPItem() const; //00A01B50
};
struct CCombatActionBase {
 void* __vftable;void* IntelligentPointer;
 CBaseIntelligentPointer PCreature;
 long TotalTime;
 unsigned char _nativeBeforeDerived[0x100];
 long GetTotalTime() const; //006925B0 loads action+10; propagated CWorld label is unrelated
 void FrameUpdate(); //00859D80
};
struct CCreatureAction_DragonFlight : CCombatActionBase {
 bool FaceTarget;float TurnRate;
 void FrameUpdate();
};
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_DragonFlight,TurnRate)==0x118);
void CCreatureAction_DragonFlight::FrameUpdate()
{
 if(TurnRate!=0.0f){
  long frames=GetTotalTime();
  C3DVector facing;
  PCreature.GetPItem()->GetPhysics()->GetFacingVectorInto(facing);
  facing.RotateXY(TurnRate/frames);
  C3DVector side(facing.Y,-facing.X,0.0f);
  C3DVector up=side.Cross(facing);
  CPhysicsMethods& physics=*PCreature.GetPItem()->GetPhysics();
  CRightHandedSet orientation(up,facing);
  physics.SetRHSet(orientation);
 }
 CCombatActionBase::FrameUpdate();
}