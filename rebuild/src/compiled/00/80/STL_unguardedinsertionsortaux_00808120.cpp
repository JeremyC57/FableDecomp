// std::__unguarded_insertion_sort_aux(pair<C3DVector, long>* first, pair<C3DVector, long>* last, pair<C3DVector, long>*, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport unguarded insertion sort of the first-person targeting positions (best score first)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808120, 140 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the unused value-type pointer and the 0x2c-byte comparator by value on the stack, `ret 30h`.
// Name: STLport `__unguarded_insertion_sort_aux`; the catalog's "_Copy_impl<CNavigationPosition*, CNavigationPosition*>"
//   is propagated.
// Callees: STLport __unguarded_linear_insert(last, pair<C3DVector, long> val, comp) 0x008074A0 (landed).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting { CVectorCompare_BestForTargeting VectorCompare; }
//   (0x2c: AnchorPos, PreferredLine, four floats, CompareInXYPlane); pair<C3DVector, long> 16 bytes.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` -- the comparator is
//   copied memberwise into its argument slot first (ten dwords and the CompareInXYPlane byte, hoisted into bl: the
//   engine C3DVector's user copy constructor makes the implicit copies non-trivial), then the value through STLport's
//   explicit pair copy constructor (four dwords).
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
        return VectorCompare.GetCompareValueForPoint(a.first) > VectorCompare.GetCompareValueForPoint(b.first);
    }
};
}

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x008074A0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(pair<C3DVector, long>*, pair<C3DVector, long>*, pair<C3DVector, long>*,
                                                        CCompareTargetedPositionsForFirstPersonTargeting);
}