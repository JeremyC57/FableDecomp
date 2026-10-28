// std::__introsort_loop(pair<CCharString, C3DDummyObject>* first, pair<CCharString, C3DDummyObject>* last, pair<CCharString, C3DDummyObject>*, int depth_limit, CCompareString<C3DDummyObject> comp) -- STLport introsort recursion over the 56-byte dummy-object map entries: partition around the median of three (name order), recurse on the right part, loop on the left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A730E0, 298 bytes, /Gr `__fastcall` (first ecx, last edx; the value-type
//   pointer, the depth limit and the empty comparator's slot on the stack), `ret 0Ch`.
// Callers: std::sort 0x00A73600 (landed, binds this exact symbol) and itself. Callees: __unguarded_partition 0x00A6F6F0
//   (landed, by-value 56-byte pivot + comparator), __partial_sort 0x00A72380 (landed), CCharString::FastCompare
//   0x009E6530 (the inline CCompareString inside the inline __median), CCharString copy ctor 0x0099EC30 (pivot key).
// Identity (proven): the landed sort 0x00A73600 binds ??$__introsort_loop@PAU?$pair@VCCharString@@VC3DDummyObject@@@std@@...
//   (CPhysicsMesh::DummyObjects ordered by CCompareString<C3DDummyObject>).
// Shape: STLport verbatim with __median inline; the pivot is copied into the partition's argument area by pair's
//   explicit copy (key string call, then C3DDummyObject's implicit 13-dword `rep movsd`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class CMatrix3x4 {                                      // 48 bytes
public:
    float M[12];
};

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb), implicit copy
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
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
                                                   _Compare __comp);   // 0x00A6F6F0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00A72380
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

typedef pair<CCharString, C3DDummyObject> TDummyEntry;
template void __fastcall __introsort_loop(TDummyEntry*, TDummyEntry*, TDummyEntry*, int, CCompareString<C3DDummyObject>);
}