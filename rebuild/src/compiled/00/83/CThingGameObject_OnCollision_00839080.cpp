// CThingGameObject::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other) -- runs the CThing collision handler, forwards the collision to the kind-0x77 component, and removes the kind-0x2a component if present
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Leaf: OnCollision
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00839080, length 123 bytes (`ret 10h`, + int3 padding).
// Callees: CThing::OnCollision @ 0x004C8DA0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC of
// kind 0x77 = bit 23 of CThing+0x2c), the component's virtual slot 28 (+0x70, its OnCollision with the same four arguments),
// CThing::RemoveTC(long) @ 0x004C9840 (kind 0x2a = bit 10 of CThing+0x24; manifest "CBaseObject::OnPropertyChanged" is a
// propagated label).
// Layout: retail CThing component bits +0x20, interface map +0x44 (local model).
// Shape: a qualified base call, then HasTC + GetTC with the component used directly as `this` (the lookup key homed in
// the dead `other` argument slot, the arguments kept in ebx/ebp), then HasTC + RemoveTC.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
enum ECollideType { ECT_DUMMY = 0 };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other);   // 0x004C8DA0
    void RemoveTC(long interfaceType);          // 0x004C9840
};
struct CTCCollisionHandler {                    // component kind 0x77
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other);   // slot 28 (+0x70)
};
class CThingGameObject : public CThing {
public:
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other);
};

void CThingGameObject::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other)
{
    CThing::OnCollision(type, position, normal, other);
    if (HasTC(0x77))
        ((CTCCollisionHandler*)GetTC(0x77))->OnCollision(type, position, normal, other);
    if (HasTC(0x2a))
        RemoveTC(0x2a);
}