// std::make_heap(pair<C3DVector, long>* first, pair<C3DVector, long>* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport make_heap front end for the first-person targeting positions: forward to __make_heap with the value and distance type pointers
// Leaf: ??$make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008097D0, 99 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx -- passed through untouched), the 0x2c-byte comparator by value, `ret 2Ch`.
// Name: Ego_r.pdb `std::make_heap<pair<C3DVector,long>*, `anonymous namespace'::
//   CCompareTargetedPositionsForFirstPersonTargeting>` (donor 0x00802B20); the catalog's
//   "std_pop_heap_FirstPersonTargeting" is descriptive.
// Callees: STLport __make_heap(first, last, comp, _Tp*, _Distance*) 0x00808CF0 (this chunk).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (0x2c, wraps CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes.
// Shape: STLport `__make_heap(first, last, comp, _STLP_VALUE_TYPE(first, ..), _STLP_DISTANCE_TYPE(first, ..))` --
//   the two null type pointers pushed first, then the comparator copied memberwise into its slot (ten dwords and the
//   CompareInXYPlane byte through cl).
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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x00808CF0

template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (pair<C3DVector, long>*)0, (int*)0);
}

template void __fastcall make_heap(pair<C3DVector, long>*, pair<C3DVector, long>*, CCompareTargetedPositionsForFirstPersonTargeting);
}