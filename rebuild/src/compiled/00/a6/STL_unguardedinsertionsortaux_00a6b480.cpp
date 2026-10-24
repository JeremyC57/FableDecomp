// _STL::__unguarded_insertion_sort_aux(pair<unsigned long,unsigned long>* first, pair<unsigned long,unsigned long>* last, pair<unsigned long,unsigned long>*, less<pair<unsigned long,unsigned long> > comp) -- insertion-sort pass with no lower-bound test: each pair sifted down to its lexicographic place
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B480, 67 bytes (+13 int3), `ret 8` (link-time __fastcall: first in
//   ecx, last in edx, the value-type tag pointer and the empty comparator on the stack). No callees.
// Callers: 0x00A6B6F7, 0x00A6BB8C, 0x00A6C0DC (the same TU's __final_insertion_sort / introsort helpers).
// Identity: the manifest's "InsertionSort_u64Pair" is descriptive. Element = std::pair<unsigned long, unsigned long>
//   under the default less<pair> (as the catalogued _Push_heap 0x00A6B1E0 / _Adjust_heap 0x00A6B4F0 of this TU).
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` with the linear insert
//   inlined: the value's two words ride in edi/ebp, next in eax, the slot being filled in esi.
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

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

typedef pair<unsigned long, unsigned long> TPair;
template void __fastcall __unguarded_insertion_sort_aux<TPair*, TPair, less<TPair> >(TPair*, TPair*, TPair*, less<TPair>);

}