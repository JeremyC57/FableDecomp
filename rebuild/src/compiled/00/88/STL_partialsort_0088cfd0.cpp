// std::__partial_sort(pair<long, CAIStateGroupBase*>* first, pair<long, CAIStateGroupBase*>* middle, pair<long, CAIStateGroupBase*>* last, pair<long, CAIStateGroupBase*>*, CKeyPairCompareGreater comp) -- introsort depth-limit fallback for the AI brain's state-group map: heap the prefix, pop every higher-priority group from the rest into it, then heap-sort the prefix
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088CFD0, 107 bytes, `ret 0Ch` (/Gr algorithm TU: first in ecx, middle
//   in edx; last, the value-type tag pointer and the empty comparator on the stack).
// Identity: called by the __introsort_loop @ 0x0088D060 (`partial_sort(first, last, last, comp)`) and by the inline
//   partial_sort at 0x0088D04C of CAIBrain's StateGroupList sort (pair<long, CAIStateGroupBase*>,
//   CKeyPairCompareGreater; Ego_r.pdb). The catalog label "partial_sort_8byte" is right in spirit.
// Callees: __make_heap(first, middle, comp, _Tp*, _Distance*) @ 0x0088CB70 (`push 0` x2), __adjust_heap @ 0x0088CA20
//   (landed), sort_heap @ 0x0088CEA0 (landed).
// Shape: STLport make_heap / __pop_heap inline; the popped value `_Tp(*i)` is copied (both dwords) before
//   `*i = *first`, then rebuilt in its argument slot by the pair copy constructor (`sub esp,8 ; mov eax,esp`).
class CAIStateGroupBase;

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

// Ego_r.pdb: CAIBrain::StateGroupList is a CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater<long,
// CAIStateGroupBase*> > -- the groups are kept in DESCENDING key (priority) order; the comparator is empty.
template <class K, class V> class CKeyPairCompareGreater {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first > b.first; }
};
typedef std::pair<long, CAIStateGroupBase*> TGroupEntry;
typedef CKeyPairCompareGreater<long, CAIStateGroupBase*> TGroupCompare;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x0088CA20
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x0088CB70
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0088CEA0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (TGroupEntry*)0, (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<TGroupEntry*, TGroupEntry, TGroupCompare>(TGroupEntry*, TGroupEntry*, TGroupEntry*,
                                                                                TGroupEntry*, TGroupCompare);
}