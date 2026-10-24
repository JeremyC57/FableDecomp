// _STL::partial_sort(pair<C3DVector, long>* first, pair<C3DVector, long>* middle, pair<C3DVector, long>* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport partial_sort front end for the first-person targeting positions: forward to __partial_sort with the value type pointer
// Leaf: ??$partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080B1A0, 102 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   middle in edx -- passed through untouched), last and the 0x2c-byte comparator by value on the stack, `ret 30h`.
// Name: STLport `partial_sort<pair<C3DVector,long>*, `anonymous namespace'::
//   CCompareTargetedPositionsForFirstPersonTargeting>` (the depth-limit fallback of the position introsort); the
//   catalog's "std_introsort_GateToDeeperSort" is descriptive.
// Callees: STLport __partial_sort(first, middle, last, _Tp*, comp) 0x0080AC10.
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (0x2c, wraps CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes.
// Shape: STLport `__partial_sort(first, middle, last, _STLP_VALUE_TYPE(first, ..), comp)` -- the comparator copied
//   memberwise into its slot first (ten dwords and the CompareInXYPlane byte through cl), then the null type pointer
//   and last.
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x0080AC10

template <class _RandomAccessIter, class _Compare>
void __fastcall partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (pair<C3DVector, long>*)0, __comp);
}

template void __fastcall partial_sort(pair<C3DVector, long>*, pair<C3DVector, long>*, pair<C3DVector, long>*,
                                      CCompareTargetedPositionsForFirstPersonTargeting);
}