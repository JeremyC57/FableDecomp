// std::__final_insertion_sort(pair<unsigned long,CDefString>* first, pair<unsigned long,CDefString>* last, CKeyPairCompareLess<unsigned long,CDefString> comp) -- STLport sort tail over the sound map's 8-byte criteria entries: insertion-sort the first 16, unguarded insertion for the rest
// Instantiation: std::__final_insertion_sort<pair<unsigned long,CDefString>*,CKeyPairCompareLess<unsigned long,CDefString> >
// Leaf: ??$__final_insertion_sort@PAU?$pair@KVCDefString@@@std@@V?$CKeyPairCompareLess@KVCDefString@@@@@std@@YIXPAU?$pair@KVCDefString@@@0@0V?$CKeyPairCompareLess@KVCDefString@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004711F5, 68 bytes, __fastcall (first ecx, last edx), `ret 4` (the empty
//   comparator); frameless size TU (`and eax,0FFFFFFF8h ; cmp eax,80h`: 16 eight-byte entries).
// Identity (sweep1, 2026-10-25): the 8-byte CriteriaMap twin of the landed CSoundPair std::__final_insertion_sort
//   0x00470D36 (FINDINGS chunk 457: "0x470D36 = __final_insertion_sort"; same source). Its only caller is the CriteriaMap
//   sort 0x004711B3 (call at 0x004711EB, right after __introsort_loop 0x00471338 with 2 * lg(n)); its callees are
//   __insertion_sort 0x0047127A (twice) and __unguarded_insertion_sort_aux 0x00471239 (hand457 README: "0x00471239 is the
//   CDefString __unguarded_insertion_sort_aux"). The landed name std::make_heap<CThing**,CThingCompare_BestForTargeting2>
//   (an "MSVC _Make_heap threshold split" model over an 8-byte CThingHeapElem) is propagated: no CThing* is 8 bytes.
// Shape: STLport 4.6 `__final_insertion_sort` with the inline `__unguarded_insertion_sort` passing `_STLP_VALUE_TYPE`
//   = `(_Tp*)0`; the comparator push is hoisted above the threshold test.
#pragma optimize("s", on)

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

template <class K, class V> class CKeyPairCompareLess {
public:
};

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
};

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);              // 0x0047127A

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00471239

template <class _Iterator> struct iterator_traits {
    typedef typename _Iterator::value_type value_type;
};
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};

template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}

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

template void __fastcall __final_insertion_sort(pair<unsigned long, CDefString>*, pair<unsigned long, CDefString>*,
                                                CKeyPairCompareLess<unsigned long, CDefString>);

} // namespace std
