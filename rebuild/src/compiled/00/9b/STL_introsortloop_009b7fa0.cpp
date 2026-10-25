// std::__introsort_loop(pair<unsigned long, unsigned long>* first, pair<unsigned long, unsigned long>* last, pair<unsigned long, unsigned long>*, int depth_limit, CKeyPairCompareLess comp) -- introsort recursion of a CRC-keyed vector map: median-of-three partition until 16 pairs remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B7FA0, 196 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU: first
//   in ecx, last in edx; the value-type tag pointer, the depth limit and the empty comparator on the stack).
// Identity: called by sort @ 0x009B85A0 (landed, `__lg(n) * 2`) and recursively by itself; the sorted map is
//   CBankFile's SymbolCRCIndices / CDefinitionManager::CompressedOffsets (Ego_r.pdb: CVectorMap<unsigned long,
//   unsigned long, CKeyPairCompareLess<unsigned long, unsigned long> >; the landed __partial_sort 0x009B6BE0 documents
//   the callers).
//   The catalog label is descriptive.
// Callees: __unguarded_partition(first, last, pivot, comp) @ 0x009B3900 (pivot by value, rebuilt in its argument slot by the
//   pair copy constructor), __partial_sort(first, last, last, (_Tp*)0, comp) @ 0x009B6BE0 (landed), itself.
// Shape: byte twin of the CAIBrain instantiation 0x0088D060 with an UNSIGNED key (`jae/jb` where that one has
//   `jle/jg`): `while (last - first > 16)`, `if (depth_limit == 0) { partial_sort(first, last, last, comp); return; }`,
//   `--depth_limit` written back to its argument slot, __median inline returning a reference (the three keys in
//   ecx/edx/ebp, the chosen element's address in eax), `last = cut`.
namespace std {
template <class _T1, class _T2> struct pair {           // STLport 4.6 pair (a struct: mangled U)
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned long, unsigned long> TMapEntry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> TMapCompare;

namespace std {
const int __stl_threshold = 16;

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
                                                   _Compare __comp);   // 0x009B3900
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x009B6BE0
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (TMapEntry*)0, __comp);
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
template void __fastcall __introsort_loop<TMapEntry*, TMapEntry, int, TMapCompare>(TMapEntry*, TMapEntry*,
                                                                                       TMapEntry*, int, TMapCompare);
}