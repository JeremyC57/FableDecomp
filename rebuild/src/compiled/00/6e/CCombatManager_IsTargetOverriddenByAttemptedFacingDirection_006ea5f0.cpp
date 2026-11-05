// CCombatManager::IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase&, const CThing&, const C3DVector&) const -- true when the target lies outside the cone (the controlling thing's combat override angle, default 60 degrees) around the direction the creature tries to face
// Leaf: IsTargetOverriddenByAttemptedFacingDirection
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA5F0, 383 bytes (384 with padding). `ret 0Ch`.
// Callees: CThing::GetPos 0x004C73D0 (twice), C3DVector::AccurateNormalise @ 0x00A14440 (catalogued GetScaled),
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
extern const float DELTA_NOTIONAL_ZERO; // 0x0129BA3C (1e-4f)

#include "engine/C3DVector.h"
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CDefPointeeBase { public:                    // a counted definition object
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
class CDefClassBase : public CDefPointeeBase { public:
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

class CTCBase;
class CThing;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCCarrying { public:                        // interface type 0x46
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad54[0x1c];
    CDefClassPointer PBaseDef;              // +0x70
    unsigned char _pad74[0x1d];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    const C3DVector& GetPos() const;   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = (const CInterfaceMap*)&m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: };

class CCombatManager { public:
    bool IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase& creature, const CThing& target,
                                                      const C3DVector& facing) const;
};

bool CCombatManager::IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase& creature, const CThing& target,
                                                                  const C3DVector& facing) const
{
    C3DVector targetPosition = target.GetPos();
    C3DVector direction = targetPosition - creature.GetPos();
    if (direction.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        direction.AccurateNormalise();
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