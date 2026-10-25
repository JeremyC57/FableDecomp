// _STL::__linear_insert(pair<long, CAIStateGroupBase*>* first, pair<long, CAIStateGroupBase*>* last, pair<long, CAIStateGroupBase*> val, CKeyPairCompareGreater comp) -- insertion-sort step of the AI brain's state-group map: a new highest priority shifts the prefix up one place, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088CD30, 99 bytes, `ret 0Ch` (/Gr algorithm TU: first in ecx, last in
//   edx, the 8-byte value and the empty comparator by value on the stack).
// Identity: one caller, the __insertion_sort @ 0x0088CE60 (landed, `__linear_insert(first, i, *i, comp)`) of CAIBrain's
//   StateGroupList sort (pair<long, CAIStateGroupBase*>, CKeyPairCompareGreater; Ego_r.pdb). The catalog label
//   "insertion_sort_8byte" names the caller.
// Callee: __unguarded_linear_insert(last, val, comp) @ 0x0088C780 (landed; the value rebuilt in its argument slot by the
//   pair copy constructor: `sub esp,8 ; mov eax,esp`).
// Shape: STLport 4.x `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- the pair is not trivially assignable for STLport's type traits, so
//   copy_backward is the counted random-access `__copy_backward` loop, inlined (`for (n = last - first; n > 0; --n)
//   *--result = *--last;`).
class CAIStateGroupBase;

namespace _STL {
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
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first > b.first; }
};
typedef _STL::pair<long, CAIStateGroupBase*> TGroupEntry;
typedef CKeyPairCompareGreater<long, CAIStateGroupBase*> TGroupCompare;

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0088C780

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template void __fastcall __linear_insert<TGroupEntry*, TGroupEntry, TGroupCompare>(TGroupEntry*, TGroupEntry*, TGroupEntry,
                                                                                 TGroupCompare);
}