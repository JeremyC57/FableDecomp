// CThingFilter_CanCollideWithThing::operator()(CThing* thing) const -- may the filter's physics body collide with this thing: it needs another physics body that collides with things, then the owner's TC 0x4d component decides
// Leaf: ??RCThingFilter_CanCollideWithThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A8300, 109 bytes (`ret 4`).
// Callees: CTCPhysicsBase::GetCollidesWithThings slot 57 (+0xe4, base 0x006B11D0), CInterfaceMap::LowerBound @ 0x004365B0,
//   the TC 0x4d component's CanCollideWith(CThing*) @ 0x00770DE0 (auto label "ContainerFindElement": true at once
//   when its list +0xc..+0x10 is empty, else a search).
// Layout: CThingFilter_CanCollideWithThing (Ego_r.pdb 4 bytes) = CTCPhysicsBase* TCPhysics; retail CThing physics component
//   +0x60, interface bitset +0x20 (TC 0x4d: word +0x28 bit 13), CVectorMap +0x44; CTCBase ParentThing +0x04.
// Shape: the agent160 GetTC form with the key in the dead thing-argument slot (no frame); the owner thing taken
//   through the filter's physics component each time.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
class CTCSolidToThing { public:                  // the component behind ETCInterfaceType 0x4d (class not identified)
public:
    bool IsSolidToThing(const CThing*) const;   // 0x00770DE0
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56();
    virtual bool GetCollidesWithThings();       // slot 57 (+0xe4)
    CThing* ParentThing;                        // +0x04
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

class CThingFilter_CanCollideWithThing { public:
    CTCPhysicsBase* TCPhysics;                  // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_CanCollideWithThing::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics && physics != TCPhysics && physics->GetCollidesWithThings()) {
        CThing* owner = TCPhysics->ParentThing;
        if (owner->HasTC(0x4d))
            return owner->GetTC<CTCSolidToThing>(0x4d)->IsSolidToThing(thing);
        return true;
    }
    return false;
}