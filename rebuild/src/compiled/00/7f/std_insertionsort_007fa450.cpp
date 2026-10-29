// std::__insertion_sort(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp) -- STLport insertion sort of the targeting candidates: walk the range, moving each candidate the comparator ranks above the first to the front (memmove) or sliding it down with __unguarded_linear_insert
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FA450, 441 bytes, /Gr `__fastcall` (first ecx, last edx, the 52-byte
//   comparator by value on the stack, `ret 34h`).
// Identity: called twice by __final_insertion_sort 0x007FA830 (landed; its 16-element head and the short-range path);
//   the catalog's "Things_InsertionSortByCompare" is descriptive. STLport spelling `__insertion_sort` (the Ego_r
//   `_Insertion_sort` name the manifest gives 0x007FA610 is the Dinkumware one; 0x007FA610 is STLport's
//   __unguarded_insertion_sort wrapper).
// Callees: CThingCompare_BestForTargeting2::GetThingCompareMultiplier 0x007F9E70, CThing::GetPos 0x004C73D0,
//   CVectorCompare_BestForTargeting::GetCompareValueForPoint 0x007F9660 (the landed operator() 0x007F9F00 inlined),
//   memmove 0x00BFEAE6, __unguarded_linear_insert(last, val, comp) 0x007FA0E0 (landed).
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i,
//   _STLP_VALUE_TYPE(first), comp);` with the inline __linear_insert (`val = *last; if (comp(val, *first)) {
//   copy_backward(first, last, last + 1); *first = val; } else __unguarded_linear_insert(last, val, comp);`) and
//   STLport's trivial copy_backward (`n = (char*)last - (char*)first; if (n > 0) memmove((char*)result - n, first, n)`).
//   The by-value comparator of the inlined __linear_insert is copied into a local once, ahead of the loop.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // user copy ctor, implicit operator=
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { float s = Y * Y; s += X * X; s += Z * Z; return s; }
    void operator*=(float f) { X *= f; Y *= f; Z *= f; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                           // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;                 // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCInterface9 {                                           // the component behind ETCInterfaceType 9
public:
    unsigned char _pad[0xc];
    unsigned long m_flags;                                      // +0x0c (bit 2: low-priority target)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                             // +0x20
    CInterfaceMap m_interfaces;                                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                     // +0x91 bit 0
    const C3DVector& GetPos() const;                             // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);         // 0x0076D2F0
__forceinline bool IS_THING_ALIVE(CThing* p) { return p && !p->Dead; }

class CVectorCompare_BestForTargeting { public:                        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                                        // +0x00
    C3DVector PreferredLine;                                    // +0x0c
    float     AngleCompare;                                     // +0x18
    float     SqrDistCompare;                                   // +0x1c
    float     OverrideShortDistanceFactor;                      // +0x20
    float     OverrideShortDistanceFalloff;                     // +0x24
    bool      CompareInXYPlane;                                 // +0x28
    float GetCompareValue(float sqrDist, float angleDot) const;     // 0x007F9480
    float GetCompareValueForPoint(const C3DVector& point) const;    // 0x007F9660 (same-TU body below)
};

class CThingCompare_BestForTargeting2 {                         // Ego_r.pdb, 52 bytes
public:
    CThing* Hero;                                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;              // +0x04
    float EnemyPreferenceFactor;                                // +0x30
    float GetThingCompareMultiplier(CThing* thing) const;       // 0x007F9E70 (same-TU body below)
    bool operator()(CThing* a, CThing* b)                       // the landed 0x007F9F00 body, inlined here
    {
        float multiplierA = GetThingCompareMultiplier(a);
        float multiplierB = GetThingCompareMultiplier(b);
        const C3DVector& posB = b->GetPos();
        const C3DVector& posA = a->GetPos();
        float valueA = VectorCompare.GetCompareValueForPoint(posA);
        float valueB = VectorCompare.GetCompareValueForPoint(posB);
        if (valueA > valueB)
            return true;
        return false;
    }
};

// The comparator's out-of-line scorers are in this TU (0x007F9660 / 0x007F9E70 sit just below the sort helpers):
// noinline bodies, the landed ones for GetCompareValueForPoint and GetThingCompareMultiplier, a read-only stand-in for
// GetCompareValue 0x007F9480 (as in the 0x0080AC10 source). They never let `this` escape, so VC7.1 treats the inlined
// __linear_insert's comparator copy as untouched by the calls and hoists it out of the loop, as retail does.
#pragma auto_inline(off)
__declspec(noinline) float CVectorCompare_BestForTargeting::GetCompareValue(float sqrDist, float angleDot) const
{
    long scale = 1L << (long)CompareInXYPlane;
    return (sqrDist * SqrDistCompare + (1.0f - angleDot) * AngleCompare) * (float)scale;
}
__declspec(noinline) float CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector& point) const
{
    C3DVector d = point - AnchorPos;
    if (CompareInXYPlane)
        d.Z = 0.0f;
    float sqrDist = d.GetLengthSquared();
    if (sqrDist > 1e-4f)
        d *= 1.0f / (float)sqrt(sqrDist);
    return GetCompareValue(sqrDist, d.Dot(PreferredLine));
}
__declspec(noinline) float CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing* thing) const
{
    if (!IS_THING_ALIVE(thing))
        return 0.0f;
    CTCInterface9* tc;
    if (thing->GetInterface(&tc, 9) && (tc->m_flags & 4))
        return 0.1f;
    if (AreThingsEnemies(Hero, thing))
        return EnemyPreferenceFactor;
    return 1.0f;
}
#pragma auto_inline(on)

namespace std {
inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : 0;
}
template <class _Tp>
inline _Tp** copy_backward(_Tp** __first, _Tp** __last, _Tp** __result)
{
    return (_Tp**)__copy_trivial_backward(__first, __last, __result);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x007FA0E0

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    _Tp __val = *__last;
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, (CThing**)0, __comp);
}

template void __fastcall __insertion_sort(CThing**, CThing**, CThingCompare_BestForTargeting2);
}