// std::sort(pair<C3DVector, long>* first, pair<C3DVector, long>* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport introsort of the first-person targeting positions (best score first): introsort loop with depth 2*lg(n), then the final insertion sort
// Leaf: ??$sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080B900, 227 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 0x2c-byte comparator by value, `ret 2Ch`.
// Name: Ego_r.pdb `std::sort<..pair<C3DVector,long>.., `anonymous namespace'::
//   CCompareTargetedPositionsForFirstPersonTargeting>` (donor 0x00805520, Dinkumware iterators); the catalog's
//   "std_introsort_setup" is descriptive.
// Callees: STLport __introsort_loop(first, last, _Tp*, depth_limit, comp) 0x0080B620, __final_insertion_sort(first,
//   last, comp) 0x00809E40 (this chunk).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (0x2c, wraps CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes.
// Shape: STLport `if (first != last) { __introsort_loop(first, last, _STLP_VALUE_TYPE(first, ..), __lg(last - first)
//   * 2, comp); __final_insertion_sort(first, last, comp); }` -- each comparator copy memberwise (the
//   CompareInXYPlane byte hoisted into bl), `__lg` inlined as the shift-count loop.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // user copy ctor, implicit operator=
};

class CVectorCompare_BestForTargeting { public:
    C3DVector AnchorPos;                                   // +0x00
    C3DVector PreferredLine;                               // +0x0c
    float AngleCompare;                                    // +0x18
    float SqrDistCompare;                                  // +0x1c
    float OverrideShortDistanceFactor;                     // +0x20
    float OverrideShortDistanceFalloff;                    // +0x24
    bool CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy constructor
};
}

namespace {
struct CCompareTargetedPositionsForFirstPersonTargeting {
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        const float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        const float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}

namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);   // 0x0080B620
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00809E40

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (pair<C3DVector, long>*)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}

template void __fastcall sort(pair<C3DVector, long>*, pair<C3DVector, long>*, CCompareTargetedPositionsForFirstPersonTargeting);
}