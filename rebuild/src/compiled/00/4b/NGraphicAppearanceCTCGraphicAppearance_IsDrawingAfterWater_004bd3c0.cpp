// 004bd3c0 NGraphicAppearance::CTCGraphicAppearance::IsDrawingAfterWater
// Leaf: ?IsDrawingAfterWater@CTCGraphicAppearance@NGraphicAppearance@@IBE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags:
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CTCBase.h"
#include "engine/_quarantine/CThingBaseDef.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CVectorMap.h"
enum ETCInterfaceType{};
// Layout: original CThing fields from supplied flattened header. No storage added.
class CThing:public CThingFields{};
class CTCCarrying:public CTCBase{};
// Layout: CTCCarryable original private PCarryingTC +0x10; CTCBase12 plus bool12 and alignment.
class CTCCarryable:public CTCBase{public:bool ForceFragility;private:CTCCarrying*PCarryingTC;public:bool IsCarried()const{return PCarryingTC!=0;}const CThing*GetPCarriedByThing()const;};

namespace NGraphicAppearance {
// Layout: supplied flat class: parent pointer +4, flag byte59 bit4. Access has no added storage.
class CTCGraphicAppearance:public CTCGraphicAppearanceFields{protected:bool IsDrawingAfterWater()const;};
bool CTCGraphicAppearance::IsDrawingAfterWater()const{
 if(PrimitivesDrawn[0]&0x10)return true;
 const CThing*parent=reinterpret_cast<const CTCBase*>(this)->ParentThing;
 if(reinterpret_cast<const unsigned long*>(parent->UsingComponents)[2]&(1UL<<7)){
  ETCInterfaceType key=static_cast<ETCInterfaceType>(0x47);
  const CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<const CVectorMap<ETCInterfaceType,CTCBase*>&>(parent->Components);
  const std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);
  const CTCCarryable*carried=static_cast<const CTCCarryable*>((i==map.end()||static_cast<ETCInterfaceType>(0x47)<i->first?map.end():i)->second);
  if(carried->IsCarried()){
   const CThing*carrier=carried->GetPCarriedByThing();
   const CThingBaseDef*def=reinterpret_cast<const CThingBaseDef*>(reinterpret_cast<const CDefPointer<const CThingBaseDef>&>(carrier->PBaseDef_Object).InternalGetObject());
   // Fidelity: shared header exposes the original bool bitfield as byte storage.
   // Store only its valid zero/one representation to preserve the byte result.
   bool result;
   *reinterpret_cast<unsigned char*>(&result)=static_cast<unsigned char>((def->GiveHeroStatChangesOnBeingHit[0]>>1)&1);
   return result;
  }
 }
 return false;
}
}