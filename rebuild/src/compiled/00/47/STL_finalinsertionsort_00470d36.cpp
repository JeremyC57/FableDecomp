// std::__final_insertion_sort(first, last, comp) -- STLport sort tail over pair<unsigned long, CSoundPair> by CKeyPairCompareLess: insertion-sort the first 16, unguarded insertion for the rest
// Leaf: ??$__final_insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00470D36, 65 bytes, __fastcall (first ecx, last edx), `ret 4` (the empty comparator);
//   size-optimised TU (odd address, `push 0Ch ; cdq ; pop esi ; idiv esi` for the 12-byte element count).
// Identity: STLport 4.6 `__final_insertion_sort`: `if (__last - __first > __stl_threshold) { __insertion_sort(__first,
//   __first + 16, __comp); __unguarded_insertion_sort(__first + 16, __last, __comp); } else __insertion_sort(__first,
//   __last, __comp);` with the inline `__unguarded_insertion_sort` passing `_STLP_VALUE_TYPE` = `(_Tp*)0` to
//   __unguarded_insertion_sort_aux. Only caller: sort() 0x00470D2C. The element is the 12-byte pair<unsigned long, CSoundPair>
//   of a CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<...> > (Ego_r.pdb CSoundPair is 8 bytes). The catalog's
//   "make_heap" label is propagated.
// Callees: __insertion_sort(first, last, comp) @ 0x00470DBC, __unguarded_insertion_sort_aux(first, last, (pair*)0, comp)
//   @ 0x00470D77 (both __fastcall, same TU, declared only).
// Pragma: #pragma optimize("s", on). Shape: the comparator push is hoisted above the threshold test (both arms start with it).
#pragma optimize("s", on)

class CSoundPair {                                      // 8 bytes (Ego_r.pdb)
public:
    unsigned long First;
    unsigned long Second;
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
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);              // 0x00470DBC

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00470D77

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

template void __fastcall __final_insertion_sort(pair<unsigned long, CSoundPair>*, pair<unsigned long, CSoundPair>*,
                                                CKeyPairCompareLess<unsigned long, CSoundPair>);

} // namespace std