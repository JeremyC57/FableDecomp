// std::__introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, int depth_limit, CKeyPairCompareLess comp) -- (pair<unsigned long, C3DHelperPoint>) STLport introsort driver of the helper-point table: median-of-three partitions down to 16-entry runs, partial_sort once the depth limit runs out
// Leaf: ??$__introsort_loop@PAU?$pair@KVC3DHelperPoint@@@std@@U12@HV?$CKeyPairCompareLess@KVC3DHelperPoint@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34B00, 256 bytes, /Gr __fastcall (first ecx, last edx; (T*)0, depth_limit
//   and the empty comparator on the stack), `ret 0Ch`.
// Identity (proven): STLport 4.6 std::__introsort_loop<pair<unsigned long, C3DHelperPoint>*, pair<..>, int,
//   CKeyPairCompareLess<unsigned long, C3DHelperPoint> >: 20-byte stride (66666667h / sar 3), pivot copied as five dwords
//   (key + C3DVector user copy + BoneIndex), unsigned key compares; callers are itself and std::sort 0x00A35150 (the
//   C3DMeshStats::HelperPoints CPreAllocVectorMap sort; sibling of the bound sort<pair<unsigned long, C3DDummyObject>*>
//   0x00A351B0); its callees are the landed __partial_sort 0x00A342F0 and __unguarded_partition 0x00A329B0, both already
//   on pair<unsigned long, C3DHelperPoint>. Ego_r.pdb names the same instantiation family (_Adjust_heap / _Make_heap
//   <pair<unsigned long, C3DHelperPoint>*, .., CKeyPairCompareLess<unsigned long, C3DHelperPoint> >). The existing
//   binding `?IntrosortLoop_00a34b00@@...SElement20...` is a placeholder: NAME_FIXES.tsv. Catalog label
//   "Introsort_Partition" is descriptive.
// Callees: __partial_sort(first, last, last, (T*)0, comp) 0x00A342F0, __unguarded_partition(first, last, T pivot, comp)
//   0x00A329B0, itself.
// Shape: STLport 4.6 _algo.c verbatim, as the landed sibling 0x00A35490 (mesh-volume table): __median inline, the pivot
//   built in the argument area by pair's user copy constructor, partial_sort inline to __partial_sort.
class C3DVector {                                                       // 12 bytes, pack(1) in retail
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                                                  // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                                 // +0x00
    long      BoneIndex;                                                // +0x0C
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {                // empty comparator (Ego_r.pdb)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
const int __stl_threshold = 16;

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                               _Tp*, _Compare __comp);                                            // 0x00A342F0
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _Tp __pivot, _Compare __comp);                // 0x00A329B0
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                         _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}
}

namespace std {
template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}

typedef std::pair<unsigned long, C3DHelperPoint> TEntry;
template void __fastcall std::__introsort_loop<TEntry*, TEntry, int, CKeyPairCompareLess<unsigned long, C3DHelperPoint> >(
    TEntry*, TEntry*, TEntry*, int, CKeyPairCompareLess<unsigned long, C3DHelperPoint>);