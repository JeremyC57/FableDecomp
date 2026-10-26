// std::__final_insertion_sort(pair<C3DVector, long>* first, pair<C3DVector, long>* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport introsort finish for the first-person targeting positions: a guarded insertion sort of the first 16 then an unguarded one of the rest (or one guarded pass when 16 or fewer)
// Leaf: ??$__final_insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809E40, 223 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 0x2c-byte comparator by value, `ret 2Ch`.
// Name: STLport `__final_insertion_sort`; the catalog's "make_heap<CThing**, CThingCompare_BestForTargeting2>" is
//   propagated.
// Callees: STLport __insertion_sort(first, last, comp) 0x00809590 and __unguarded_insertion_sort(first, last, comp)
//   0x00808A30 (both landed, out of line).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (0x2c, wraps CVectorCompare_BestForTargeting);
//   pair<C3DVector, long> 16 bytes.
// Shape: STLport `if (last - first > __stl_threshold) { __insertion_sort(first, first + 16, comp);
//   __unguarded_insertion_sort(first + 16, last, comp); } else __insertion_sort(first, last, comp);` -- both arms open
//   with the same comparator copy, so VC7.1 hoists its ten dwords above the branch and stores the CompareInXYPlane byte
//   per arm (bl / dl).
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
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00809590
template <class _RandomAccessIter, class _Compare>
void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00808A30

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}

template void __fastcall __final_insertion_sort(pair<C3DVector, long>*, pair<C3DVector, long>*,
                                                CCompareTargetedPositionsForFirstPersonTargeting);
}