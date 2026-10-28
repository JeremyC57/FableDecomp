// std::__introsort_loop(pair<CCharString, unsigned long>* first, pair<CCharString, unsigned long>* last, pair<CCharString, unsigned long>*, int depth_limit, CKeyPairCompareLess comp) -- introsort recursion of a name-keyed vector map: median-of-three partition until 16 entries remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8B50, 171 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU: first
//   in ecx, last in edx; the value-type tag pointer, the depth limit and the empty comparator on the stack).
// Identity: the CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > sort family of
//   the definition-manager templates TU: sort 0x009B8EF0 (landed), __unguarded_partition 0x009B5800 (landed),
//   __partial_sort 0x009B86B0 (landed), __adjust_heap 0x009B5C70 / sort_heap 0x009B8170 (landed, labelled
//   pair<CCharString, unsigned long>). The catalog label "introsort_partial_pair_quicksort" is descriptive.
// Callees: __median(a, b, c, comp) @ 0x009B5730 (out of line here: the CCharString comparison is too big to inline
//   six times), CCharString(const CCharString&) @ 0x0099EC30 (the pivot pair built in its argument slot by the inline
//   pair copy constructor), __unguarded_partition @ 0x009B5800, __partial_sort(first, last, last, (_Tp*)0, comp) @
//   0x009B86B0, itself.
// Shape: STLport `while (last - first > 16)` loop; the comparator push is hoisted above the depth test (both arms start
//   with it); the pivot slot's address is homed in the dead comparator argument slot across the string copy.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                   // +0x00
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {
template <class _T1, class _T2> struct pair {           // STLport 4.6 pair (a struct: mangled U)
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};
typedef std::pair<CCharString, unsigned long> TNameEntry;
typedef CKeyPairCompareLess<CCharString, unsigned long> TNameCompare;

namespace std {
const int __stl_threshold = 16;

template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x009B5730
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x009B5800
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x009B86B0
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (TNameEntry*)0, __comp);
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
template void __fastcall __introsort_loop<TNameEntry*, TNameEntry, int, TNameCompare>(TNameEntry*, TNameEntry*,
                                                                                    TNameEntry*, int, TNameCompare);
}