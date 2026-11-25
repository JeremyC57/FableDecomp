// GetToughnessDamageReductionFactorForThing(const CThing&) -- the thing's damage multiplier: the toughness def's high/low range scaled by its component, then three optional state multipliers (each only while the component's current count is below its total)
// Flags: /O2 /Oy /W3 
// Leaf: ?GetToughnessDamageReductionFactorForThing@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path.)
// Identity (sweep3; was the descriptive "CTCBase_CalculateEffectiveDamageMultiplier"): Ego_r.pdb
//   ?GetToughnessDamageReductionFactorForThing@?A0x23f4dc30@@YIMABVCThing@@@Z, one of the five anonymous-namespace armour
//   helpers of the combat-damage TU (with GetArmourThresholdValueFromArmourWeights 0x008686E0,
//   GetArmourResponseDamageAttributeDamageMultiplier 0x008687A0, GetArmourResponseAugmentationDamageMultiplier 0x00868A80,
//   GetArmourWeightsFromThing 0x00868E50); the only one taking just a const CThing& and returning a float (__fastcall,
//   plain `ret`); its caller CHitParameters::SetAsModifiedByArmour 0x00869020 scales the damage by it. Body unchanged.
#include "fable_compiler.h"
class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in CInterfaceMap (?LowerBound@Map@Row00868b40@@QBEPAUEntry@2@ABH@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): the thing's Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE spelling at 0x004365B0.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
// Naming (NF443, 2026-11-25; coordinator round 3): callee-only rebind of the interface 0x85 (TCI_DIVINE_WRATH) call 0x0078E7B0 to its
//   PDB spelling ?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ (was ?B@Component@Row00868b40@@QAEMXZ) through a bodiless declaration (no
//   layout; rulings r2/r3); the self row 0x0078E7B0 is respelt to it in the same batch.
class CTCDivineWrath { public: float GetDamageMultiplier() const; };
namespace Row00868b40 {
struct Component;struct Scale{float Get(Component*);};
struct Def {virtual void Dummy()=0;virtual void Release()=0;int refs;char gap[0xb4];float high,low;Scale scale;};
struct Component{char pad[0x58];Def*def;int total,current;char gap[0x60];Scale scale;float A();float C();};
struct Entry{int key;Component*component;};
typedef ::CInterfaceMap Map;
// Layout: this is thing interface owner, flags+2c/+30, map+44. Component and def offsets taken from retail, no shared matching headers.
}
using namespace Row00868b40;
class CThing{public:char pad[0x2c];unsigned flags;unsigned flags2;char gap[0x10];Map map;
 template<int Key> FABLE_FORCEINLINE Component*Get()const{const Map*m=&map;ETCInterfaceType key=(ETCInterfaceType)Key;const Map::value_type*p=m->LowerBound(key);const Map::value_type*end=m->end();return (Component*)(p==end||Key<p->first?end:p)->second;}
};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00851BD0 ?GetParameter@CHeroAbilityStatWeight@@QBEMABVCTCHeroExperience@@@Z (was ?Get@Scale@Row00868b40@@QAEMPAUComponent@2@@Z)
class CTCHeroExperience;
class CHeroAbilityStatWeight { public: float GetParameter(CTCHeroExperience const&) const; };
namespace {
float __fastcall GetToughnessDamageReductionFactorForThing(const CThing& thing){float value=1.0f;if(thing.flags&0x100){Component*c=thing.Get<0x68>();Def*d=c->def;if(d)++d->refs;float scale=reinterpret_cast<const CHeroAbilityStatWeight*>(&(d->scale))->GetParameter(*reinterpret_cast<CTCHeroExperience const*>(c));const float low=d->low;value=(d->high-low)*scale+low;if(--d->refs==0)d->Release();}
 if(thing.flags2&0x10)value*=thing.Get<0x84>()->A();
 if(thing.flags2&0x20){Component*c=thing.Get<0x85>();if(c->current<c->total)value*=reinterpret_cast<const CTCDivineWrath*>(c)->GetDamageMultiplier();}
 if(thing.flags2&0x40){Component*c=thing.Get<0x86>();if(c->current<c->total)return c->C()*value;}
 return value;
}
}
