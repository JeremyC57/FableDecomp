// CCombatAction_StabThing::GetNextPos()const -- use stealth approach position on first frame and face target, otherwise retain current position.
// Native0087ED70,244 bytes; component54 CTCStealth, actual PDB GetPositionToMoveToForStabOn(CThing&,float&)const at donor759570 -> native746EC0.
#include "fable_string.h"
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCombatAction_StabThing CCombatAction_StabThing_PDB
#include "engine/CCombatAction_StabThing.h"
#undef CCombatAction_StabThing
#include "engine/CTCPhysicsBase.h"
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
class C3DVector { public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}};
struct Creature;struct PhysicsView;
#pragma pointers_to_members(full_generality,single_inheritance)
union PhysicsSlot {void* Address;void(PhysicsView::*SetFacingAngleXY)(float);};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(PhysicsSlot)==4);
#endif
struct PhysicsView:CTCPhysicsBase {__forceinline void SetFacingAngleXY(const float& angle){(this->*reinterpret_cast<PhysicsSlot*>(__vftable)[66].SetFacingAngleXY)(angle);}}; //slot66=00723760 nativeCTCPhysicsStandard
class CTCStealth { public:C3DVector GetPositionToMoveToForStabOn(Creature& target,float& angle)const;}; //00746EC0
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> InterfaceEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:InterfaceEntry* First;InterfaceEntry* Last;const InterfaceEntry* LowerBound(const ETCInterfaceType& type) const;};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > InterfaceMap; //004365B0 const lower_bound
class CThing { public:
    const C3DVector& GetPos() const; //004C73D0
};
struct Creature : public CThing {
 unsigned char _beforeFlags[0x20];unsigned long PresentBits[9];InterfaceMap Interfaces;unsigned char _beforePhysics[0x14];PhysicsView* Physics;unsigned char _beforeDead[0x2d];unsigned char DeadFlags;
 __forceinline PhysicsView* GetPhysics()const{return Physics;}
 __forceinline bool GetInterface(CTCStealth** out,long type)const{if(PresentBits[type>>5]&(1UL<<(type&31))){ETCInterfaceType key = (ETCInterfaceType)(type);const InterfaceMap* map=&Interfaces;const InterfaceEntry* p=map->LowerBound(key);if(p==map->Last||type<p->first)p=map->Last;*out=((CTCStealth*)p->second);return true;}return false;}
};
struct CBaseObjectPointer{void* object;unsigned long refcount;};
struct CBaseIntelligentPointer{virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;Creature* GetPItem()const;}; //00A01B50
class CCombatAction_StabThing { public:
 virtual ~CCombatAction_StabThing();unsigned char _baseState[4];CBaseIntelligentPointer PCreature;unsigned char _beforeFrame[0xc];long FramesRunning;unsigned char _beforeTarget[0x88];CBaseIntelligentPointer PTarget;
 C3DVector GetNextPos()const;
};
C3DVector CCombatAction_StabThing::GetNextPos()const{
 if(FramesRunning==1){CTCStealth* stealth;if(PCreature.GetPItem()->GetInterface(&stealth,54)&&PTarget.GetPItem()&&!(PTarget.GetPItem()->DeadFlags&1)){
  float angle;C3DVector position=stealth->GetPositionToMoveToForStabOn(*PTarget.GetPItem(),angle);
  PCreature.GetPItem()->GetPhysics()->SetFacingAngleXY(angle);return position;
 }}
 return PCreature.GetPItem()->GetPos();
}