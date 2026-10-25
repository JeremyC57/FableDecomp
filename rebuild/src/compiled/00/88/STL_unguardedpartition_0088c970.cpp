// std::__unguarded_partition(pair<long, CAIStateGroupBase*>* first, pair<long, CAIStateGroupBase*>* last, pair<long, CAIStateGroupBase*> pivot, CKeyPairCompareGreater comp) -- introsort partition of the AI brain's state-group map: walk both ends towards the pivot priority and swap the out-of-place pairs
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088C970, 85 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU: first in
//   ecx, last in edx, the 8-byte pivot and the empty comparator by value on the stack).
// Identity: called only by the __introsort_loop @ 0x0088D060 of CAIBrain's StateGroupList sort (CVectorMap Sort
//   0x0088D1B0 -> sort 0x0088D130, landed); element pair<long, CAIStateGroupBase*> under CKeyPairCompareGreater (the
//   landed relatives 0x0088C780 / 0x0088CA20 model the same pair descriptively as SKeyedPair / SKeyGreater). The
//   catalog label "introsort_partition_8byte" is descriptive.
// Shape: STLport 4.x `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`: the comparator inline as a signed
//   `cmp ; jle/jg` on the keys (pivot.first in ecx), the iter_swap through a pair temporary (both dwords).
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
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, (TGroupEntry*)0);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
template TGroupEntry* __fastcall __unguarded_partition<TGroupEntry*, TGroupEntry, TGroupCompare>(TGroupEntry*, TGroupEntry*,
                                                                                               TGroupEntry, TGroupCompare);
}