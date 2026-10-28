// std::__introsort_loop(pair<CCharString, C3DHelperPoint>* first, pair<CCharString, C3DHelperPoint>* last, pair<CCharString, C3DHelperPoint>*, int depth_limit, CCompareString<C3DHelperPoint> comp) -- STLport introsort recursion over the 20-byte helper-point map entries: partition around the median of three (name order), recurse on the right part, loop on the left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A72FA0, 306 bytes, /Gr `__fastcall` (first ecx, last edx; the value-type
//   pointer, the depth limit and the empty comparator's slot on the stack), `ret 0Ch`.
// Callers: the sort 0x00A735A0 (landed) and itself. Callees: __unguarded_partition 0x00A6F610 (landed, by-value
//   20-byte pivot + comparator, `ret 18h`), __partial_sort 0x00A72300 (landed), CCharString::FastCompare 0x009E6530
//   (the inline CCompareString inside the inline __median), CCharString copy ctor 0x0099EC30 (the pivot copy).
// Identity: element and comparator are the ones its two landed callees prove (pair<CCharString, C3DHelperPoint>,
//   CCompareString<C3DHelperPoint>: CPhysicsMesh::HelperPoints). The binding `IntrosortLoop_00a72fa0` from the
//   landed sort is an invented global stand-in; this is STLport _algo.c `__introsort_loop`.
// Shape: STLport verbatim with __median inline (the comp(a,b)/comp(b,c)/comp(a,c) ladder of FastCompare calls),
//   the pivot copied into the partition's argument area by pair's explicit copy (string call, then C3DHelperPoint's
//   four words through C3DVector's memberwise copy).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class C3DVector {                                       // user memberwise copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                                  // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

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

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x00A6F610
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00A72300
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type_of(__first), __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
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

typedef pair<CCharString, C3DHelperPoint> THelperEntry;
template void __fastcall __introsort_loop(THelperEntry*, THelperEntry*, THelperEntry*, int, CCompareString<C3DHelperPoint>);
}