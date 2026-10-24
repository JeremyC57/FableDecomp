// std::__unguarded_partition(pair<long,long>* first, pair<long,long>* last, pair<long,long> pivot, CKeyPairCompareLess<long,long> comp) -- STLport quicksort partition of keyed pairs around a by-value pivot; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F8CE2, 57 bytes, `ret 0Ch` (/Gr TU: first in ecx, last in edx; the 8-byte
//   pivot by value and the empty comparator on the stack). Caller: __introsort_loop 0x005FA2F6 (ExpressionButtons sort).
// Label correction: the catalog `std_partition_Swap` is behavioural; the body is STLport `__unguarded_partition`
//   (`for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`). Byte twin 0x005F8C9B (pair<long,EHeroAbility>).
// Shape: size TU (`#pragma optimize("s", on)`: the first scan is entered by a jump to its test); only pivot.first is read
//   (edi); the inlined `__iter_swap_aux` temporary is the STLport pair copy (ecx/esi), then two word-wise assignments
//   (`__forceinline` on iter_swap / __iter_swap_aux: the size TU otherwise calls them out of line).
#pragma optimize("s", on)

enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class ForwardIter1, class ForwardIter2, class T>
__forceinline void __iter_swap_aux(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
__forceinline void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap_aux(a, b, a);
}

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __fastcall __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

template pair<long, long>* __fastcall __unguarded_partition<pair<long, long>*, pair<long, long>,
                                                                    CKeyPairCompareLess<long, long> >(
    pair<long, long>*, pair<long, long>*, pair<long, long>, CKeyPairCompareLess<long, long>);
}