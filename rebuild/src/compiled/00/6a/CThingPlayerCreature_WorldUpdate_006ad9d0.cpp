// CThingPlayerCreature::WorldUpdate() -- creature update plus the player extras: per-frame movement vector from the last position, look tracking, player control, animation, collision record reset and weapon trails
// Leaf: WorldUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AD9D0, length 284 bytes.
// Callees: CThingCreatureBase::UpdateAction @ 0x006643C0, CThingCreatureBase::WorldUpdate @ 0x006666A0 (called
// non-virtually, result dropped), the last-position getter @ 0x006A4D10 and CThing::PeekPos() const @ 0x006A4D00
// (this TU's out-of-line copies; catalog "CVertexBufferWin32::DoSizeof" is a propagated twin), CVectorMap::LowerBound
// @ 0x0040F020 (interfaces 0x43 = CTCLook, bit 3 of +0x28, and 0x4c = CTCWeaponTrail, bit 12 of +0x28),
// CTCLook::TrackNearestInterestingThing(bool) @ 0x00753840, CThingPlayerCreature::UpdatePlayerControl @ 0x006AD970,
// CThingCreatureBase::UpdateAnimation(float) @ 0x00665860, CTCWeaponTrail::UpdateWeaponTrails @ 0x0076EFA0.
// Layout (retail): CThing interface bitset +0x20, map +0x44; CThingCreatureBase MovementVector +0x10c, the update flags
// byte +0x15d (bit 1: a last position exists); CThingPlayerCreature CollidedWithThing +0x1cc, LastThingCollisionNormal
// +0x1cd, LastThingCollisionPosition +0x1d9 (Ego_r.pdb - 0x2c; the two vectors are byte-packed in the PDB as well).
// Shape: `MovementVector = PeekPos() - PeekLastPos()` through a frame temporary copied dword by dword (the right
// operand's getter runs first); the animation step is a literal 0.0f pushed from the zero register; the collision
// vectors are cleared with chained `X = Y = Z = 0.0f` (Z first; a C3DVector temporary goes through the frame), which
// is what makes VC7.1 keep 0 (and the flag mask 2) in ebx; the look component comes through the out-parameter
// GetInterface (component in eax, moved to ecx after the push), the weapon trail through the plain entry pick.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCLook {
public:
    bool TrackNearestInterestingThing(bool force);             // 0x00753840
};
class CTCWeaponTrail {
public:
    void UpdateWeaponTrails();                                 // 0x0076EFA0
};
#pragma pack(push, 1)
class CThingPhysical { public:
    const C3DVector& PeekLastPos() const;    // 0x006A4D10 ?PeekLastPos@CThingPhysical@@QBEABVC3DVector@@XZ (Ego_r donor order Kill/PeekPos/PeekLastPos/OnCreate)
    virtual void v00();  // moved from CThingCreatureBase: the inserted base owns the vptr (vtable order unchanged)
    const C3DVector& PeekPos() const;                      // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical {
public:
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                            // +0x20
    CInterfaceMap m_interfaces;                                // +0x44
    unsigned char _pad4c[0x10c - 0x4c];
    C3DVector     MovementVector;                              // +0x10c
    unsigned char _pad118[0x15d - 0x118];
    unsigned char m_updateFlags;                               // +0x15d bit 1: last position valid
    unsigned char _pad15e[0x1cc - 0x15e];
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return ((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
    void UpdateAction();                                       // 0x006643C0
    bool WorldUpdate();                                        // 0x006666A0
    void UpdateAnimation(float step);                          // 0x00665860
};
class CThingPlayerCreature : public CThingCreatureBase {
public:
    bool      CollidedWithThing;                               // +0x1cc
    C3DVector LastThingCollisionNormal;                        // +0x1cd
    C3DVector LastThingCollisionPosition;                      // +0x1d9
    void UpdatePlayerControl();                                // 0x006AD970
    bool WorldUpdate();
};
#pragma pack(pop)

bool CThingPlayerCreature::WorldUpdate()
{
    UpdateAction();
    CThingCreatureBase::WorldUpdate();
    if (m_updateFlags & 2)
        MovementVector = PeekPos() - PeekLastPos();
    m_updateFlags |= 2;
    CTCLook* look;
    if (GetInterface(&look, 0x43))
        look->TrackNearestInterestingThing(true);
    UpdatePlayerControl();
    UpdateAnimation(0.0f);
    CollidedWithThing = false;
    LastThingCollisionNormal.X = LastThingCollisionNormal.Y = LastThingCollisionNormal.Z = 0.0f;
    LastThingCollisionPosition.X = LastThingCollisionPosition.Y = LastThingCollisionPosition.Z = 0.0f;
    if (HasTC(0x4c))
        ((CTCWeaponTrail*)GetTC(0x4c))->UpdateWeaponTrails();
    return false;
}