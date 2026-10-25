// std::__make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*) -- (std::pair<unsigned long, unsigned long>, CKeyPairCompareLess<unsigned long, unsigned long>) STLport heapify: sift every parent down from (len - 2) / 2 to the root through the out-of-line __adjust_heap
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B3CF0, 74 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx, last
//   in edx, then the 4-byte empty comparator slot and the value-type / distance-type tag pointers on the stack).
// Identity: std::pair<unsigned long, unsigned long> under CKeyPairCompareLess<unsigned long, unsigned long> -- proven; evidence: its callers are the landed __partial_sort 0x009B6BE0 and __linear_insert 0x009B46B0's twin path of the sort 0x009B85A0 used by CBankFile::ReadEntryList / PostInitEntries (Ego_r.pdb SymbolCRCIndices = CVectorMap<unsigned long, unsigned long>) and CDefinitionManager::SaveBinaryDefinitions; FableWin.pdb names make_heap<pair<unsigned long, unsigned long>*, CKeyPairCompareLess<unsigned long, unsigned long> >.
// Label: the manifest's "MakeHeap_Pair8Byte_Build" is propagated across byte-identical bodies (the eight rows differ only in the
//   __adjust_heap call displacement); this is STLport 4.6 `__make_heap` over an 8-byte pair.
// Callee: std::__adjust_heap(first, holeIndex, len, _Tp value, comp) @ 0x009B3060 (__fastcall: first in ecx, holeIndex in
//   edx; len, the by-value 8-byte pair and the comparator on the stack).
// Body: STLport 4.6 _heap.c verbatim: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2;
//   for (;;) { __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`
//   -- the element is rebuilt in the argument area by pair's user copy constructor (`sub esp,8 ; mov eax,esp`).

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                          // +0
    _T2 second;                                         // +4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

}
template <class _Key, class _Value> class CKeyPairCompareLess {      // empty key comparator (CVectorMap)
public:
    bool operator()(const std::pair<_Key, _Value>& __a, const std::pair<_Key, _Value>& __b) const { return __a.first < __b.first; }
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>                   // 0x009B3060
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
typedef std::pair<unsigned long, unsigned long> _Elem;
template void __fastcall __make_heap(_Elem*, _Elem*, CKeyPairCompareLess<unsigned long, unsigned long>, _Elem*, int*);
}