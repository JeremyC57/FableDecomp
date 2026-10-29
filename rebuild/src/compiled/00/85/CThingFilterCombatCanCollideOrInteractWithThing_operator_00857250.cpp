// CThingFilter_CombatCanCollideOrInteractWithThing::operator()(CThing* thing) const -- the combat variant of the collide/interact filter: creatures and dead creatures can be excluded, both sides' solid-to-thing components must accept each other, and a thing passes with a colliding/interacting physics component, as a dead creature, or when melee strikes can hit it
// Leaf: ??RCThingFilter_CombatCanCollideOrInteractWithThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x00857250, 312 bytes (+2 int3), `ret 4`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x4d twice, TC 0x77), CTCSolidToThing::IsSolidToThing(const
//   CThing*) const @ 0x00770DE0, CTCPhysicsBase::GetCollidesWithThings (slot 57, +0xe4) / GetInteractsWithThings (slot 60,
//   +0xf0), the type-0x77 component's CanBeHitByMeleeStrike @ 0x007BEFA0 (manifest class CTCShotDefinitionBase).
// Layout: Ego_r.pdb CThingFilter_CombatCanCollideOrInteractWithThing (8 bytes): TCPhysics +0, ExcludeCreatures +4,
//   ExcludeDeadCreatures +5; CTCBase ParentThing +4; retail CThing interface bitset +0x20 (TC 0x4d = word +0x28 bit 13,
//   TC 0x77 = word +0x2c bit 23), map +0x44, PhysicsTC +0x60, ThingTypeFlags +0x6c (bit 3 creature, bit 7 dead).
// Shape: the landed CThingFilter_CanCollideOrInteractWithThing::operator() 0x006A8370 body with the dead-creature
//   exclusion after the creature one and the two extra pass tests before the final `return false`.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing;
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56();
    virtual bool GetCollidesWithThings() const;         // slot 57 (+0xe4)
    virtual bool GetCollidesWithThingsOneWay() const;   // slot 58 (+0xe8)
    virtual void v59();
    virtual bool GetInteractsWithThings() const;        // slot 60 (+0xf0)
    CThing* ParentThing;                        // +0x04 (CTCBase)
};

class CTCSolidToThing { public:
    bool IsSolidToThing(const CThing* thing) const;     // 0x00770DE0
};
struct CTCMeleeHittable {                       // interface type 0x77 (class name not pinned)
    bool CanBeHitByMeleeStrike() const;         // 0x007BEFA0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;               // +0x6c
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};

struct CThingFilter_CombatCanCollideOrInteractWithThing {
    CTCPhysicsBase* TCPhysics;                  // +0x00
    bool ExcludeCreatures;                      // +0x04
    bool ExcludeDeadCreatures;                  // +0x05
    bool operator()(CThing* thing) const;
};

bool CThingFilter_CombatCanCollideOrInteractWithThing::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (ExcludeCreatures && (thing->ThingTypeFlags & 8))
        return false;
    if (ExcludeDeadCreatures && (thing->ThingTypeFlags & 0x80))
        return false;
    if (TCPhysics->ParentThing->HasTC(0x4d)) {
        if (!TCPhysics->ParentThing->GetTC<CTCSolidToThing>(0x4d)->IsSolidToThing(thing))
            return false;
    }
    if (thing->HasTC(0x4d)) {
        CTCSolidToThing* solid = thing->GetTC<CTCSolidToThing>(0x4d);
        CThing* parent = TCPhysics->ParentThing;
        if (!solid->IsSolidToThing(parent))
            return false;
    }
    if (physics && physics != TCPhysics) {
        if (physics->GetCollidesWithThings() || physics->GetInteractsWithThings())
            return true;
    }
    if (thing->ThingTypeFlags & 0x80)
        return true;
    if (thing->HasTC(0x77) && thing->GetTC<CTCMeleeHittable>(0x77)->CanBeHitByMeleeStrike())
        return true;
    return false;
}