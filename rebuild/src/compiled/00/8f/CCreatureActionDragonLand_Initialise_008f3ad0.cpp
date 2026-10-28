// CCreatureAction_DragonLand::Initialise() -- initialise landing, lower position, target dragon and suspend animation.
// Retail008F3AD0 length300. Physics slot31 ResetPosition006B01D0 const-vector reference; slot70
// GetFacingVector723C00 native hidden-buffer ABI ECXthis/EAXbuffer/ret4. Real header + field-free adapter.
#define CCreatureAction_DragonLand CCreatureAction_DragonLand_PDB
#include "engine/CCreatureAction_DragonLand.h"
#undef CCreatureAction_DragonLand
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CClass.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCPhysicsBase.h"
#include "fable_string.h"
class C3DVector { public:float X,Y,Z;C3DVector(){} C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}};
struct CPhysicsMethods : CTCPhysicsBase { void ResetPosition(const C3DVector&); const C3DVector& GetFacingVectorInto(C3DVector&)const;};
union CPhysicsSlot {
 void* Address;
 void (CPhysicsMethods::*ResetPosition)(const C3DVector&);
 const C3DVector& (CPhysicsMethods::*GetFacingVectorInto)(C3DVector&) const;
};
inline void CPhysicsMethods::ResetPosition(const C3DVector& v){(this->*reinterpret_cast<CPhysicsSlot*>(__vftable)[31].ResetPosition)(v);}
inline const C3DVector& CPhysicsMethods::GetFacingVectorInto(C3DVector& v)const{return (this->*reinterpret_cast<CPhysicsSlot*>(__vftable)[70].GetFacingVectorInto)(v);}
FABLE_STATIC_ASSERT(sizeof(CPhysicsSlot)==4);
FABLE_STATIC_ASSERT(sizeof(CPhysicsMethods)==sizeof(CTCPhysicsBase));
// Interface0x3a proven CTCMapwho: RTTI table1265A3C slot21=71A7E0 returns58.
#include "engine/CTCMapwho.h"
struct CInterfaceMap {struct Entry {long Type;void* Component;};Entry* First;Entry* Last;Entry* LowerBound(const long*);}; //0040F020
class CThing : public CClass { public:
 unsigned char _nativeBeforeUsingComponents[0xf];unsigned long UsingComponents[9];CInterfaceMap Components;
 unsigned char _nativeBeforePhysics[0x14];CPhysicsMethods* PPhysics;
 __forceinline CPhysicsMethods* GetPhysics()const{return PPhysics;}
 const C3DVector& GetPos() const; //004C73D0
 void SetSuspendAnimation(bool,const void*); //00833A10, requester keyed suspension
};
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();void* PData;CThing* GetPItem() const;}; //00A01C10/00A01B50
class CCombatActionBase { public:void* __vftable;void* IntelligentPointer;CBaseIntelligentPointer PCreature;
 void SetAnimation(const CCharString&); //008574D0
 void Initialise(); //00857520
};
extern const float g_DragonLandingHeightDrop; //0128838C =30.148378372192383
class CCreatureAction_DragonLand : public CCombatActionBase { public:void Initialise();};
FABLE_STATIC_ASSERT(offsetof(CThing,PPhysics)==0x60);
void CCreatureAction_DragonLand::Initialise(){
 {CCharString animation("LAND",-1);SetAnimation(animation);}
 CCombatActionBase::Initialise();
 CThing* thing=PCreature.GetPItem();
 if(thing->UsingComponents[1]&0x4000000){
  CInterfaceMap* map=&thing->Components;long type=0x3a;CInterfaceMap::Entry* p=map->LowerBound(&type);
  if(p==map->Last || p->Type>0x3a)p=map->Last;
  static_cast<CTCMapwho*>(p->Component)->InMapwho[0]&=~1;
 }
 C3DVector position=PCreature.GetPItem()->GetPos();position.Z-=g_DragonLandingHeightDrop;
 PCreature.GetPItem()->GetPhysics()->ResetPosition(position);
 C3DVector facing;
 PCreature.GetPItem()->GetPhysics()->GetFacingVectorInto(facing);
 thing=PCreature.GetPItem();CTCTargeted* target;
 if(thing->UsingComponents[0]&0x200){
  CInterfaceMap* map=&thing->Components;long type=9;CInterfaceMap::Entry* p=map->LowerBound(&type);
  if(p==map->Last || p->Type>9)p=map->Last;
  target=static_cast<CTCTargeted*>(p->Component);
 }
 target->Targetable=true;
 if(PCreature.GetPItem()->Type==1){CThing* creature=PCreature.GetPItem();creature->SetSuspendAnimation(true,this);}
}