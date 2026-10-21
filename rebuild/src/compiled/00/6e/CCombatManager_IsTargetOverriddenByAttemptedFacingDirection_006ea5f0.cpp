// CCombatManager::IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase&, const CThing&, const C3DVector&) const -- true when the target lies outside the cone (the controlling thing's combat override angle, default 60 degrees) around the direction the creature tries to face
// Leaf: IsTargetOverriddenByAttemptedFacingDirection
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA5F0, 383 bytes (384 with padding). `ret 0Ch`.
// Callees: CThing::GetPosition 0x004C73D0 (twice), C3DVector::Normalise 0x00A14440 (catalogued GetScaled),
//          CVectorMap::LowerBound 0x004365B0 (inlined GetInterface, type 0x46 = CTCCarrying, bit 6 of the bitset byte
//          +0x28), CTCCarrying::GetThingInPrimarySlot 0x006A1440, CDefClassBase::GetSubDef<T> 0x0042B1A6 (same-TU
//          stand-in below), the defs' virtual slot 1 on release; the fcos intrinsic. Globals: 1e-4f 0x0129BA3C,
//          1/360 0x01238E00, the double 2pi 0x0124F2B8.
//
// Shape: the target position is a named copy; the direction is built in memory (Normalise is out of line) and only
// normalised when longer than the epsilon. The thing whose definition decides is the creature, or the live thing in
// its primary carry slot; its PBaseDef is copied into a counted holder (add-ref) for GetSubDef and released; the
// sub-def pointer and the 60-degree angle sit in the dead argument slots (the angle is a variable overwritten by
// `if (found)`, which keeps its 60.0f in memory: a ternary on the literal reads the float pool), the sub-def is read
// once and released on both exits; the dot is compared with the double cos (a (float) cast adds an fxch pair).
// Layout: CThing interface bitset +0x20 and map +0x44 (landed GetInterface model), dead bit +0x91, PBaseDef +0x70
// (landed 0x006E3780); the sub-def's override angle +0xcc (descriptive name).
#include <math.h>
#pragma intrinsic(cos)
extern float g_Epsilon;                     // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise();                       // 0x00A14440
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CDefPointeeBase {                    // a counted definition object
    virtual void v000();
    virtual void DestroyDef();              // slot 1 (+4)
    long m_refCount;                        // +4
};
struct CCombatOverrideDef : CDefPointeeBase {
    unsigned char _pad08[0xc4];
    float TargetOverrideAngle;              // +0xcc (degrees)
};
struct CSubDefPointer {                     // CDefPointer<const CCombatOverrideDef>
    CCombatOverrideDef* Object;
    CSubDefPointer() : Object(0) {}
    ~CSubDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->DestroyDef();
    }
    CCombatOverrideDef* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CSubDefPointer& out) const;      // 0x0042B1A6
};
void* FindDef_GetSubDef_0042b1a6();         // stand-in callee of the stand-in body below
#pragma auto_inline(off)
// Same-TU stand-in for the GetSubDef<T> instantiation 0x0042B1A6: it writes the out slot only inline, so VC7.1 knows
// the def pointer's address does not escape the call (the landed 0x005DA6E0 idiom).
__declspec(noinline) bool CDefClassBase::GetSubDef(CSubDefPointer& out) const
{
    void* def = FindDef_GetSubDef_0042b1a6();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
struct CDefClassPointer {                   // CDefPointer<const CDefClassBase>, copied with an add-ref
    CDefClassBase* Object;
    CDefClassPointer(const CDefClassPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefClassPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->DestroyDef();
    }
    CDefClassBase* operator->() const { return Object; }
};

struct CTCBase;
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};
struct CTCCarrying {                        // interface type 0x46
    CThing* GetThingInPrimarySlot();        // 0x006A1440
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x24];
    CDefClassPointer PBaseDef;              // +0x70
    unsigned char _pad74[0x1d];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = (CInterfaceMap*)&m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };

struct CCombatManager {
    bool IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase& creature, const CThing& target,
                                                      const C3DVector& facing) const;
};

bool CCombatManager::IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase& creature, const CThing& target,
                                                                  const C3DVector& facing) const
{
    C3DVector targetPosition = target.GetPosition();
    C3DVector direction = targetPosition - creature.GetPosition();
    if (direction.GetLengthSquared() > g_Epsilon * g_Epsilon) {
        direction.Normalise();
        const CThing* controller = &creature;
        CTCCarrying* carrying;
        if (creature.GetInterface(&carrying, 0x46)) {
            CThing* carried = carrying->GetThingInPrimarySlot();
            if (carried && !(carried->m_flags91 & 1))
                controller = carried;
        }
        float angle = 60.0f;
        CSubDefPointer overrideDef;
        bool found;
        {
            CDefClassPointer def = controller->PBaseDef;
            found = def->GetSubDef(overrideDef);
        }
        if (found)
            angle = overrideDef->TargetOverrideAngle;
        if (Dot(direction, facing) < cos(angle * (1.0f / 360.0f) * 6.2831854820251465))
            return true;
    }
    return false;
}