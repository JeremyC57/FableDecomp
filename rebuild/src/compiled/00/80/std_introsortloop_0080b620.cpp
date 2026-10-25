// std::__introsort_loop(pair<C3DVector,long>* first, pair<C3DVector,long>* last, pair<C3DVector,long>*, int depth_limit, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport introsort recursion of the first-person targeting positions: partition around the median of three, recurse on the right part, loop on the left; partial_sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080B620, 492 bytes, /Gr `__fastcall` (first ecx, last edx; the value-type
//   pointer, the depth limit and the 0x2c-byte comparator by value on the stack, `ret 34h`).
// Identity: callers 0x0080B982 (the sort front end of this TU) and itself; the callees are the
//   pair<C3DVector, long> / CCompareTargetedPositionsForFirstPersonTargeting instantiations (the landed partial_sort
//   0x0080B1A0 names the element and comparator). The catalog's "std_introsort_TargetPointQuickSort" is descriptive.
// Callees: __median(a, b, c, comp) 0x00807170, __unguarded_partition(first, last, pivot, comp) 0x00807250,
//   partial_sort(first, middle, last, comp) 0x0080B1A0 (landed), itself.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last,
//   comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last -
//   first) / 2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }`
//   -- each by-value comparator is copied memberwise (ten dwords and the CompareInXYPlane byte, cached in bl), the pivot
//   is copy-constructed in its argument slot from the median's reference.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // user copy ctor, implicit operator=
};

struct CVectorCompare_BestForTargeting {                   // Ego_r.pdb, 0x2c bytes
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
struct CCompareTargetedPositionsForFirstPersonTargeting {  // Ego_r.pdb, 0x2c bytes
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        const float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        const float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}
typedef std::pair<C3DVector, long> TargetedPosition;

namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x00807170
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                                 // 0x00807250
template <class _RandomAccessIter, class _Compare>
void __fastcall partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                             _Compare __comp);                                                       // 0x0080B1A0

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    _RandomAccessIter __cut;
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        __cut = __unguarded_partition(__first, __last,
                                      _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                      __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop(TargetedPosition*, TargetedPosition*, TargetedPosition*, int,
                                          CCompareTargetedPositionsForFirstPersonTargeting);
}