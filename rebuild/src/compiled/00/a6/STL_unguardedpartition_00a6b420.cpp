// _STL::__unguarded_partition(pair<unsigned long,unsigned long>* first, pair<unsigned long,unsigned long>* last, pair<unsigned long,unsigned long> pivot, less<pair<unsigned long,unsigned long> > comp) -- quicksort partition of the pairs around a by-value pivot; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B420, 87 bytes (+9 int3), `ret 0Ch` (link-time __fastcall: first in
//   ecx, last in edx, the 8-byte pivot and the empty comparator on the stack). No callees.
// Caller: 0x00A6BF6E, the same TU's __introsort_loop (catalogued "QuickSort_Introsort_u64Pair" 0x00A6C080 family).
// Identity: the manifest's "QuickSort_Partition_u64Pair" is descriptive. Element = std::pair<unsigned long, unsigned
//   long> under the default less<pair> (as the catalogued _Push_heap 0x00A6B1E0 / _Adjust_heap 0x00A6B4F0 of this TU).
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot words stay in edi/ebx, the
//   inlined __iter_swap temporary in ecx/esi.
namespace _STL {

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> class less {
public:
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
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

typedef pair<unsigned long, unsigned long> TPair;
template TPair* __fastcall __unguarded_partition<TPair*, TPair, less<TPair> >(TPair*, TPair*, TPair, less<TPair>);

}