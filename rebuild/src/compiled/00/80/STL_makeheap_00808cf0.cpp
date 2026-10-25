// std::__make_heap(pair<C3DVector, long>* first, pair<C3DVector, long>* last, CCompareTargetedPositionsForFirstPersonTargeting comp, pair<C3DVector, long>*, int*) -- STLport make_heap of the first-person targeting positions (best score at the root): sift every parent down, last parent first
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808CF0, 183 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 0x2c-byte comparator by value and the two type pointers on the stack, `ret 34h`.
// Name: STLport `__make_heap`; Ego_r.pdb's Dinkumware counterpart is `_Make_heap<pair<C3DVector,long>*, int,
//   pair<C3DVector,long>, `anonymous namespace'::CCompareTargetedPositionsForFirstPersonTargeting>` (donor 0x00802590).
//   The catalog's "std_pop_heap_WithPredicate" is descriptive.
// Callees: STLport __adjust_heap(first, hole, len, value, comp) 0x008082C0.
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (0x2c, wraps CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the
//   comparator copied memberwise into its argument slot (ten dwords and the CompareInXYPlane byte), then the value via
//   STLport's explicit pair copy constructor through the strength-reduced `first + parent` pointer (+8).
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp);   // 0x008082C0

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template void __fastcall __make_heap(pair<C3DVector, long>*, pair<C3DVector, long>*, CCompareTargetedPositionsForFirstPersonTargeting,
                                     pair<C3DVector, long>*, int*);
}