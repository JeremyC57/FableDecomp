// _STL::__unguarded_partition(pair<C3DVector, long>* first, pair<C3DVector, long>* last, pair<C3DVector, long> pivot, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport introsort partition step for the first-person targeting position sort: elements scoring better than the pivot to the front; returns the cut
// Grade: DIFFER(199v199) -- scheduling only: in comp(*first, pivot) retail forms the pivot address (`lea eax,[esp+24h]`) between the first GetCompareValueForPoint call and the `fstp` of its result, ours after the `fstp` (RTM, QFE and /G7 alike); tried the plain expression, two named float values (kept: retail's comparator form, see 0x00808AA0), the reversed `<` form (evaluates b first)
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807250, 199 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 16-byte pivot and the 0x2c-byte comparator by value on the stack, `ret 3Ch`.
// Name: STLport `__unguarded_partition`; Ego_r.pdb's Dinkumware counterpart is `_Unguarded_partition<pair<C3DVector,
//   long>*, `anonymous namespace'::CCompareTargetedPositionsForFirstPersonTargeting>` (donor 0x008041C0). The catalog's
//   "std_introsort_Partition_TargetPoint" is descriptive.
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (twice per compare).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (0x2c, wraps CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if
//   (!(first < last)) return first; iter_swap(first, last); ++first; }` -- comp(x, y) is `value(x.first) >
//   value(y.first)` (x's value parked in a frame word); iter_swap inlined as STLport `swap`: the temporary pair is
//   copied memberwise into the frame (C3DVector's user copy constructor), the two assignments are the implicit
//   four-dword copies through pointer registers.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // user copy ctor, implicit operator=
};

struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;                                   // +0x00
    C3DVector PreferredLine;                               // +0x0c
    float AngleCompare;                                    // +0x18
    float SqrDistCompare;                                  // +0x1c
    float OverrideShortDistanceFactor;                     // +0x20
    float OverrideShortDistanceFalloff;                    // +0x24
    bool CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy constructor
};
}

namespace {
struct CCompareTargetedPositionsForFirstPersonTargeting {
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const _STL::pair<C3DVector, long>& a, const _STL::pair<C3DVector, long>& b) const
    {
        const float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        const float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}

namespace _STL {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template pair<C3DVector, long>* __fastcall __unguarded_partition(pair<C3DVector, long>*, pair<C3DVector, long>*, pair<C3DVector, long>,
                                                                 CCompareTargetedPositionsForFirstPersonTargeting);
}
