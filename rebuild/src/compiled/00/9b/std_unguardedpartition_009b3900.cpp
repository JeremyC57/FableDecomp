// std::__unguarded_partition(std::pair<unsigned long, unsigned long >* first, std::pair<unsigned long, unsigned long >* last, std::pair<unsigned long, unsigned long > pivot, CKeyPairCompareLess<unsigned long, unsigned long > comp) -- STLport Hoare partition: advance first past elements below pivot, retreat last past elements above it, swap and continue until they cross; return first
// Leaf: ??$__unguarded_partition@PAU?$pair@KK@std@@U12@V?$CKeyPairCompareLess@KK@@@std@@YIPAU?$pair@KK@0@PAU10@0U10@V?$CKeyPairCompareLess@KK@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009b3900, 85 bytes, /Gr __fastcall (first in ecx, last in edx; pivot, comp on the stack), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) ??$__unguarded_partition@PAU?$pair@KK@std@@U12@V?$CKeyPairCompareLess@KK@@@std@@YIPAU?$pair@KK@0@PAU10@0U10@V?$CKeyPairCompareLess@KK@@@Z; its only caller is the landed __introsort_loop 0x009B7FA0.
// Callees: none (the comparator is inline: an unsigned compare of the first dword).
// Shape (family L054): STLport 4.6 _algo.c __unguarded_partition verbatim with the inline iter_swap (two-dword swap
//   through ebx/esi/edi); the pivot's key stays in ecx.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
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
}
template std::pair<unsigned long, unsigned long >* __fastcall std::__unguarded_partition(std::pair<unsigned long, unsigned long >*, std::pair<unsigned long, unsigned long >*, std::pair<unsigned long, unsigned long >, CKeyPairCompareLess<unsigned long, unsigned long >);