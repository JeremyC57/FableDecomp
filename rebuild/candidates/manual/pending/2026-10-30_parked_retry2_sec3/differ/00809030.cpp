// CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV(const C3DVector& position, const C3DVector& facing, CThing* targeter, CTargetingDef* targetingDef, CThing* carriedThing, float targetingFOV, bool angleTest3D) -- store the targeting cone (the facing flattened to XY unless the angle test is 3D, then normalised) and the targeter's current lock target
// Leaf: ??0CThingFilter_IsInTargetingRangeAndFOV
// Grade: DIFFER(166v166) -- wave 2 sec3: the real const CVectorMap::LowerBound 0x004365B0 body in the TU with the GetTC key-copy accessor; residue unchanged (FacingVector copy through lea ecx,[esi+0Ch] in retail; the key address in ecx vs eax at the TC 8 lookup)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809030, length 166 bytes, `ret 1Ch`, returns `this`.
// Callees: C3DVector::Normalise @ 0x00A14440, CInterfaceMap::LowerBound const @ 0x004365B0 (TC 8), the TC 8 component's
//   virtual slot 36 (+0x90, returns the locked target thing; class and name unproven).
// Layout: CThingFilter_IsInTargetingRangeAndFOV (Ego_r.pdb, 48 bytes, matches retail): Position +0, FacingVector +0xc,
//   Targeter +0x18, PTargetLockThing +0x1c, TargetingDef +0x20, PCarriedThing +0x24, TargetingFOV +0x28,
//   AngleTest3D +0x2c; retail CThing interface bitset +0x20 (TC 8 = word +0x20 bit 8), interface map +0x44.
// Shape: the two vectors, the def, the carried thing and the FOV are initialiser-list copies (memberwise C3DVector
//   copy constructor); Targeter, the cleared lock target and the 3D flag are body assignments (stored after them, one
//   zero register shared with the flag test); the Z clear, Normalise on the facing and the TC 8 query follow.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& Normalise();                     // 0x00A14440
};
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CTCBase;
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp* end() const { return _M_finish; }
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    const value_type* LowerBound(const K& key) const;       // 0x004365B0
    std::vector<value_type> Data;
    Cmp  Compare;
    bool Dirty;
};
template <class K, class V, class Cmp>
__declspec(noinline) const std::pair<K, V>* CVectorMap<K, V, Cmp>::LowerBound(const K& key) const   // 0x004365B0: the retail body, out of line (same-TU mod-ref)
{
    long count = Data._M_finish - Data._M_start;
    const value_type* first = Data._M_start;
    while (count > 0) {
        long half = count / 2;
        const value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count = count - half - 1;
        } else
            count = half;
    }
    return first;
}
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCTargetLockTC_8 {                      // the component behind ETCInterfaceType 8 (class not identified)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2)
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual CThing* GetTargetLockThing_slot36();   // slot 36 (+0x90)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(ETCInterfaceType type) const
    {
        const CInterfaceMap* map = &Components;
        const ETCInterfaceType key = type;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        const CInterfaceMap::value_type* end = map->Data.end();
        return (it == end || type < it->first ? end : it)->second;
    }
};
struct CTargetingDef;

struct CThingFilter_IsInTargetingRangeAndFOV {
    C3DVector Position;                         // +0x00
    C3DVector FacingVector;                     // +0x0c
    CThing* Targeter;                           // +0x18
    CThing* PTargetLockThing;                   // +0x1c
    CTargetingDef* TargetingDef;                // +0x20
    CThing* PCarriedThing;                      // +0x24
    float TargetingFOV;                         // +0x28
    bool AngleTest3D;                           // +0x2c
    CThingFilter_IsInTargetingRangeAndFOV(const C3DVector& position, const C3DVector& facing, CThing* targeter,
        CTargetingDef* targetingDef, CThing* carriedThing, float targetingFOV, bool angleTest3D);
};

CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV(const C3DVector& position, const C3DVector& facing,
    CThing* targeter, CTargetingDef* targetingDef, CThing* carriedThing, float targetingFOV, bool angleTest3D)
    : Position(position), FacingVector(facing), TargetingDef(targetingDef), PCarriedThing(carriedThing), TargetingFOV(targetingFOV)
{
    Targeter = targeter;
    PTargetLockThing = 0;
    AngleTest3D = angleTest3D;
    if (!AngleTest3D)
        FacingVector.Z = 0;
    FacingVector.Normalise();
    if (targeter->HasTC(8))
        PTargetLockThing = ((CTCTargetLockTC_8*)targeter->GetTC((ETCInterfaceType)8))->GetTargetLockThing_slot36();
}
