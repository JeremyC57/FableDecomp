// CThingFilter_CanCollideOrInteractWithThing::operator()(CThing* thing) const -- a thing passes when (creatures allowed or it is not a creature) both sides' solid-to-thing components accept each other, and it has a different physics component that collides or interacts with things
// Leaf: ??RCThingFilter_CanCollideOrInteractWithThing
// Grade: DIFFER(226v224) -- second IsSolidToThing call: retail fetches the component straight into ecx (`mov ecx,[eax+4]`) and then loads the parent argument, ours keeps the component in eax and adds `mov ecx,eax` (chained, named-local, parent-local and accessor forms tried)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8370, length 222 bytes, `ret 4`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x4d), CTCSolidToThing::IsSolidToThing(const CThing*) const
//   @ 0x00770DE0, CTCPhysicsBase::GetCollidesWithThings (slot 57, +0xe4) / GetInteractsWithThings (slot 60, +0xf0).
// Layout: CThingFilter_CanCollideOrInteractWithThing (Ego_r.pdb, 8 bytes): TCPhysics +0, ExcludeCreatures +4;
//   CTCBase ParentThing +4; retail CThing interface bitset +0x20 (TC 0x4d = word +0x28 bit 13), interface map +0x44,
//   PhysicsTC +0x60, ThingTypeFlags +0x6c (bit 3: creature).
// Shape: the thing's physics component is read first into a local (homed in the dead argument slot); each TC 0x4d test
//   is HasTC + the inline const GetTC (the filter's own parent re-read for the second call's argument); the final
//   collide/interact pair is an `||` inside the null and identity tests with separate true/false exits.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

class CThing;
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
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

struct CTCSolidToThing {
    bool IsSolidToThing(const CThing* thing) const;     // 0x00770DE0
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
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CThingFilter_CanCollideOrInteractWithThing {
    CTCPhysicsBase* TCPhysics;                  // +0x00
    bool ExcludeCreatures;                      // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_CanCollideOrInteractWithThing::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (ExcludeCreatures && (thing->ThingTypeFlags & 8))
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
    return false;
}
