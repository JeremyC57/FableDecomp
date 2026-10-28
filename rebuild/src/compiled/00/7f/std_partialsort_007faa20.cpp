// std::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CThingCompare_BestForTargeting2 comp) -- STLport partial sort of the targeting candidates: heapify [first, middle), pop every later candidate the best-for-targeting comparator prefers into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FAA20, 472 bytes, /Gr `__fastcall` (first ecx, middle edx; last, the
//   value-type pointer and the 52-byte comparator by value on the stack, `ret 3Ch`).
// Identity: the CThing* / CThingCompare_BestForTargeting2 instantiation; its caller is the partial_sort front end
//   0x007FAC00 (landed) and its callees the landed make_heap 0x007FA890, __pop_heap 0x007FA680 and sort_heap
//   0x007FA980 of the same pair. The catalog's "std_Sort_IntroHeap_Things" is descriptive.
// Callees: make_heap 0x007FA890, CThingCompare_BestForTargeting2::GetThingCompareMultiplier 0x007F9E70, CThing::
//   GetPos 0x004C73D0, CVectorCompare_BestForTargeting::GetCompareValueForPoint 0x007F9660 (the landed
//   operator() 0x007F9F00 inlined), __pop_heap(first, last, result, val, comp, int*) 0x007FA680, sort_heap 0x007FA980.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- the comparator runs
//   inline on the by-value parameter; every by-value comparator copy is memberwise.
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
class CInterfaceMap {                                           // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;                 // 0x004365B0
};
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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
// GetCompareValue 0x007F9480 (as in the 0x0080AC10 source); the TU model shared with the insertion sort 0x007FA450.
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
template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x007FA890
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);                                  // 0x007FA680
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x007FA980

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort(CThing**, CThing**, CThing**, CThing**, CThingCompare_BestForTargeting2);
}