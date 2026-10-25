// std::__partial_sort(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* middle, NPlayerTargeting::CTargetPoint* last, CTargetPoint*, CThingCompare_BestForTargeting2 comp) -- STLport partial sort of the player-targeting points: heapify [first, middle), pop every later point the best-for-targeting comparator prefers into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080BAB0, 504 bytes, /Gr `__fastcall` (first ecx, middle edx; last, the
//   value-type pointer and the 52-byte comparator by value on the stack, `ret 3Ch`).
// Identity: the CTargetPoint / CThingCompare_BestForTargeting2 instantiation of this TU (its callees are the landed
//   __pop_heap 0x0080B210 and sort_heap 0x0080B9F0 of the same pair); the catalog's "std_introsort_TargetPointMain" is
//   descriptive.
// Callees: make_heap(first, last, comp) 0x0080B810, CThingCompare_BestForTargeting2::operator() 0x008098B0 (out of line
//   here), __pop_heap(first, last, result, val, comp, int*) 0x0080B210 (landed), sort_heap 0x0080B9F0 (landed),
//   CBaseIntelligentPointer ctor / GetPItem / SetPItem 0x00A01B10 / 0x00A01B50 / 0x00A01B90; vtable 0x01238C6C.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- VC7.1 turns the loop into
//   a trip count ((last - middle - 1) / 28 + 1, kept in the dead `last` slot); each by-value comparator is copied
//   memberwise (Hero cached in esi), the target point copy-constructed in its argument slot.
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
class CTargetPoint {                                        // 0x1c bytes (Ego_r.pdb)
public:
    CIntelligentPointer<CThing> PThing;                     // +0x00
    long                        Index;                      // +0x08
    C3DVector                   TargetingPosition;          // +0x0C
    long                        FrameRejected;              // +0x18
    __forceinline CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
};
}
using NPlayerTargeting::CTargetPoint;

struct CVectorCompare_BestForTargeting {                    // Ego_r.pdb, 0x2c bytes
    C3DVector AnchorPos;                                    // +0x00
    C3DVector PreferredLine;                                // +0x0C
    float     AngleCompare;                                 // +0x18
    float     SqrDistCompare;                               // +0x1C
    float     OverrideShortDistanceFactor;                  // +0x20
    float     OverrideShortDistanceFalloff;                 // +0x24
    bool      CompareInXYPlane;                             // +0x28
};

class CThingCompare_BestForTargeting2 {                     // Ego_r.pdb, 0x34 bytes
public:
    CThing*                         Hero;                   // +0x00
    CVectorCompare_BestForTargeting VectorCompare;          // +0x04
    float                           EnemyPreferenceFactor;  // +0x30
    bool operator()(const CTargetPoint& a, const CTargetPoint& b);         // 0x008098B0 (same-TU body below)
};

// Same-TU stand-in for the comparator 0x008098B0 (retail's TU holds its body: VC7.1 then knows the call leaves the
// by-value comparator alone and keeps comp.Hero in esi across it).  It reads only, like the real scorer.
#pragma auto_inline(off)
__declspec(noinline) bool CThingCompare_BestForTargeting2::operator()(const CTargetPoint& a, const CTargetPoint& b)
{
    return Hero != 0 && a.Index < b.Index;
}
#pragma auto_inline(on)

namespace std {
template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0080B810
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);                                  // 0x0080B210
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0080B9F0

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

template void __fastcall __partial_sort(CTargetPoint*, CTargetPoint*, CTargetPoint*, CTargetPoint*,
                                        CThingCompare_BestForTargeting2);
}