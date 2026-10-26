// std::sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) -- (pair<unsigned long, CDefString >*, CKeyPairCompareLess<unsigned long, CDefString>) STLport introsort: for a non-empty range run __introsort_loop to depth 2*lg(n), then __final_insertion_sort
// Flags: /Gr
// Leaf: ??$sort@PAU?$pair@KVCDefString@@@std@@V?$CKeyPairCompareLess@KVCDefString@@@@@std@@YIXPAU?$pair@KVCDefString@@@0@0V?$CKeyPairCompareLess@KVCDefString@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004711B3, 66 bytes, __fastcall (/Gr TU: first ecx, last edx, the comparator by value on the stack), `ret 4`.
// Identity (proven): std::sort<std::pair<unsigned long, CDefString >*, CKeyPairCompareLess<unsigned long, CDefString> >; evidence: its only caller is CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> >::Sort 0x0047119A (sort(Data.begin(), Data.end(), Compare) with Compare the byte at +0x0C, then Dirty +0x0D = false), called once, at 0x004706EA in CSoundMap::TransferBinaryIn 0x0047042A on this+0x10 -- the CriteriaMap whose vector<pair<unsigned long, CDefString> >::resize 0x00470A28 the same function calls on that register (the first map, SoundMap at +0, sorts through 0x00470CD8: its 12-byte pair<unsigned long, CSoundPair> cannot be this 8-byte element)
// Label: the manifest's "std::sort_introsort_init_8byte" is a behavioural / propagated label (family J067: five byte-identical sort bodies, each its own element and comparator).
// Callees: __introsort_loop(first, last, (value_type*)0, int depth_limit, comp) 0x00471338, __final_insertion_sort(first, last, comp) 0x004711F5
//   (both out of line; the bindings' ??$__introsort_loop spellings for this element where present).
// Shape: STLport 4.6 _algo.c `sort` verbatim in a size-optimised TU (the __lg loop enters at its test, the comparator is re-pushed
//   from its stack slot); __lg is inlined (`for (k = 0; n != 1; n >>= 1) ++k;`), (last - first) is the exact
//   pointer difference (`sar 3` for the 0x8-byte pair); the empty comparator travels as one pushed dword.
// Element: std::pair<unsigned long, CDefString> (8 bytes), the element of Ego_r.pdb CSoundMap::CriteriaMap, CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> >
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
    typedef int difference_type;
};
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00471338
template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004711F5
template <class _RandomAccessIter, class _Compare>
void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0,
                         __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
}   // namespace std
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
template <class K, class V> class CKeyPairCompareLess { };   // Fable's vector-map key order (1 byte; its operator() lives in the out-of-line callees)
namespace std {
template void sort<pair<unsigned long, CDefString >*, CKeyPairCompareLess<unsigned long, CDefString> >(pair<unsigned long, CDefString >*, pair<unsigned long, CDefString >*, CKeyPairCompareLess<unsigned long, CDefString>);
}