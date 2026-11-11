// CCombatAction_StabThing::GetNextPos()const -- use stealth approach position on first frame and face target, otherwise retain current position.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Native0087ED70,244 bytes; component54 CTCStealth, actual PDB GetPositionToMoveToForStabOn(CThing&,float&)const at donor759570 -> native746EC0.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
#include "fable_compiler.h"
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
#include "engine/C3DVector.h"
struct Creature;struct PhysicsView;
#pragma pointers_to_members(full_generality,single_inheritance)
union PhysicsSlot {void* Address;void(PhysicsView::*SetFacingAngleXY)(float);};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(PhysicsSlot)==4);
#endif
struct PhysicsView:CTCPhysicsBase {FABLE_FORCEINLINE void SetFacingAngleXY(const float& angle){(this->*reinterpret_cast<PhysicsSlot*>(__vftable)[66].SetFacingAngleXY)(angle);}}; //slot66=00723760 nativeCTCPhysicsStandard
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
 FABLE_FORCEINLINE PhysicsView* GetPhysics()const{return Physics;}
 FABLE_FORCEINLINE bool GetInterface(CTCStealth** out,long type)const{if(PresentBits[type>>5]&(1UL<<(type&31))){ETCInterfaceType key = (ETCInterfaceType)(type);const InterfaceMap* map=&Interfaces;const InterfaceEntry* p=map->LowerBound(key);if(p==map->Last||type<p->first)p=map->Last;*out=((CTCStealth*)p->second);return true;}return false;}
};
struct CBaseObjectPointer{void* object;unsigned long refcount;};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
 //00A01B50
class CCombatAction_StabThing { public:
 virtual ~CCombatAction_StabThing();unsigned char _baseState[4];CBaseIntelligentPointer PCreature;unsigned char _beforeFrame[0xc];long FramesRunning;unsigned char _beforeTarget[0x88];CBaseIntelligentPointer PTarget;
 C3DVector GetNextPos()const;
};
C3DVector CCombatAction_StabThing::GetNextPos()const{
 if(FramesRunning==1){CTCStealth* stealth;if(reinterpret_cast<Creature*>(PCreature.GetPItem())->GetInterface(&stealth,54)&&reinterpret_cast<Creature*>(PTarget.GetPItem())&&!(reinterpret_cast<Creature*>(PTarget.GetPItem())->DeadFlags&1)){
  float angle;C3DVector position=stealth->GetPositionToMoveToForStabOn(*reinterpret_cast<Creature*>(PTarget.GetPItem()),angle);
  reinterpret_cast<Creature*>(PCreature.GetPItem())->GetPhysics()->SetFacingAngleXY(angle);return position;
 }}
 return reinterpret_cast<Creature*>(PCreature.GetPItem())->GetPos();
}