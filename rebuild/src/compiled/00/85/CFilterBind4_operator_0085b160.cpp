// CFilter_Bind4::operator()(const CThingGameObject& thing) -- a game object passes the combat strike search when it is not the striker, is a strike-to-strike target, its hit-location bounding sphere overlaps the strike sphere and the striker can collide or interact with it
// Leaf: ??$?RVCThingGameObject@@@?$CFilter_Bind4@VCThingFilter_IsNot@@VCThingFilter_CombatIsStrikeToStrike@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085B160, length 221 bytes (+3 int3), thiscall `ret 4`, bool in eax. No call or
//   address reference reaches it in retail (an out-of-line COMDAT instance, like the landed 0x0063F2C0 / 0x00669040).
// Full name (Ego_r.pdb name set): CFilter_Bind4<CThingFilter_IsNot, `anonymous namespace'::CThingFilter_CombatIsStrikeToStrike,
//   CThingFilter_CombatOverlappingSphere, `anonymous namespace'::CThingFilter_CombatCanCollideOrInteractWithThing>::
//   operator()<CThingGameObject>(const CThingGameObject&) const -- the filter of CThingSearchTools::GetAllThingsInRoughArea /
//   GetAllThings<CThingGameObject, list<CThingGameObject*>, CFilter_Bind4<...> > in the same Ego_r.pdb set. The leaf above is a
//   prefix: the anonymous-namespace hash is per TU (retail ?A0xad05d24c). Catalog "CThingFilter_CombatStrikeToStrike_Apply" is
//   descriptive.
// Callees: CThingFilter_IsNot::operator() 0x006A76A0, CThingFilter_CombatIsStrikeToStrike::operator() 0x008565E0,
//   CThingFilter_CombatCanCollideOrInteractWithThing::operator() 0x00857250 (all `const CThing&`, bool in al), the const
//   interface-map LowerBound 0x004365B0, CTCHitLocations::GetBoundingSphere(C3DVector&, float&, bool) const 0x00815420
//   (Ego_r.pdb `?GetBoundingSphere@CTCHitLocations@@QBEXAAVC3DVector@@AAM_N@Z`; manifest label agrees).
// Layout (Ego_r.pdb): CFilter_Bind4 { F1 +0, F2 +4, F3 +8, F4 +0xC } (pointers); CThingFilter_CombatOverlappingSphere
//   { C3DVector SpherePos +0, float Radius +0xC }; retail CThing interface bits +0x20 (0x7F = CTCHitLocations: word 3, bit 31)
//   and interface map +0x44 (the family model of this chunk).
// Shape: `return (*F1)(thing) && (*F2)(thing) && (*F3)(thing) && (*F4)(thing);` (the `&&` value gives `mov eax,1` /
//   `xor eax,eax`); the overlapping-sphere operator is __forceinline over the out-parameter GetInterface (HasTC on bit 31
//   gives `test eax,eax ; jns`; GetTC inside the call expression pushes the sphere arguments in another order); the sphere
//   centre is a 12-byte frame local and the radius out-parameter reuses the dead lookup-key slot (the thing argument's
//   home) only with the same-TU LowerBound stand-in below (an external LowerBound gives a 0x10 frame).
class CTCBase;
struct CTCInterfaceEntry {
    long     type;
    CTCBase* component;
};
class CTCInterfaceMap {                                 // CVectorMap<ETCInterfaceType, CTCBase*> (retail vector: first/last/end)
public:
    CTCInterfaceEntry* first;
    CTCInterfaceEntry* last;
    CTCInterfaceEntry* end;
    CTCInterfaceEntry* LowerBound(const long& type) const;   // 0x004365B0
};
class CThing {
public:
    void*           _vfptr;                             // +0x00
    unsigned char   _pad04[0x20 - 0x04];
    unsigned long   m_tcPresent[9];                     // +0x20 interface present bits
    CTCInterfaceMap m_tcs;                              // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_tcPresent[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (HasTC(type)) {
            long key = type;
            const CTCInterfaceMap* map = &m_tcs;
            CTCInterfaceEntry* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
// Same-TU stand-in for the const LowerBound (0x004365B0): with its body in the TU VC knows the key's address does not
// escape, so the dead key slot (the thing argument's home) is reused for the bounding-sphere radius as in retail; an
// external declaration keeps the key alive to the end and grows the frame by 4.
#pragma auto_inline(off)
__declspec(noinline) CTCInterfaceEntry* CTCInterfaceMap::LowerBound(const long& type) const
{
    CTCInterfaceEntry* lo = first;
    long count = (long)(last - first);
    while (count > 0) {
        long half = count / 2;
        CTCInterfaceEntry* mid = lo + half;
        if (mid->type < type) {
            lo = mid + 1;
            count -= half + 1;
        } else {
            count = half;
        }
    }
    return lo;
}
#pragma auto_inline(on)
class CThingGameObject : public CThing { };
class C3DVector {
public:
    float X, Y, Z;
};
class CTCHitLocations {                                 // interface 0x7F
public:
    void GetBoundingSphere(C3DVector& centre, float& radius, bool flag) const;   // 0x00815420
};

class CThingFilter_IsNot {
public:
    CThing* Thing;                                      // +0x00
    bool operator()(const CThing& thing) const;         // 0x006A76A0
};
namespace {
class CThingFilter_CombatIsStrikeToStrike {
public:
    CThing* ThingStriking;                              // +0x00
    bool operator()(const CThing& thing) const;         // 0x008565E0
};
class CThingFilter_CombatCanCollideOrInteractWithThing {
public:
    class CTCPhysicsBase* TCPhysics;                    // +0x00
    bool ExcludeCreatures;                              // +0x04
    bool ExcludeDeadCreatures;                          // +0x05
    bool operator()(const CThing& thing) const;         // 0x00857250
};
}
class CThingFilter_CombatOverlappingSphere {
public:
    C3DVector SpherePos;                                // +0x00
    float     Radius;                                   // +0x0C
    __forceinline bool operator()(const CThing& thing) const
    {
        const CTCHitLocations* hitLocations;
        if (thing.GetInterface(&hitLocations, 0x7f)) {
            C3DVector centre;
            float radius;
            hitLocations->GetBoundingSphere(centre, radius, true);
            float dx = centre.X - SpherePos.X;
            float dy = centre.Y - SpherePos.Y;
            float dz = centre.Z - SpherePos.Z;
            float r = radius + Radius;
            if (dx * dx + dy * dy + dz * dz < r * r)
                return true;
        }
        return false;
    }
};

template <class F1, class F2, class F3, class F4> class CFilter_Bind4 {
public:
    const F1* PF1;                                      // +0x00
    const F2* PF2;                                      // +0x04
    const F3* PF3;                                      // +0x08
    const F4* PF4;                                      // +0x0C
    template <class T> bool operator()(const T& thing) const
    {
        return (*PF1)(thing) && (*PF2)(thing) && (*PF3)(thing) && (*PF4)(thing);
    }
};

template bool CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere,
                            CThingFilter_CombatCanCollideOrInteractWithThing>::operator()<CThingGameObject>(const CThingGameObject&) const;