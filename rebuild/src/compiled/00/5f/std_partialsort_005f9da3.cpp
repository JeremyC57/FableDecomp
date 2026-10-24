// std::__partial_sort(pair<long,long>* first, pair<long,long>* middle, pair<long,long>* last, pair<long,long>*, CKeyPairCompareLess<long,long> comp) -- STLport partial sort: heapify [first, middle), pop every smaller element of [middle, last) into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F9DA3, 85 bytes, `ret 0Ch` (/Gr TU: first in ecx -> edi, middle in edx ->
//   ebx; last, the value-type tag pointer and the empty comparator on the stack).
// Callees: make_heap 0x005F93E6 (the out-of-line front end of __make_heap), __pop_heap 0x005F8D82 (landed),
//   sort_heap 0x005F9B5A. Caller: partial_sort 0x005FA0D5, the depth-limit fallback of __introsort_loop 0x005FA2F6
//   (ExpressionButtons CVectorMap<long,long> sort). Byte twin 0x005F9D4E is the pair<long,EHeroAbility> instantiation.
// Label correction: the catalog `Heap_ExtractAndSort` is behavioural; the body is STLport `__partial_sort`
//   (`make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);`).
// Shape: frame pointer kept in a size TU (`#pragma optimize("y", off)` + `("s", on)`); the by-value pair is built in the
//   argument area by STLport pair's memberwise copy constructor after the null Distance* and the comparator pushes.
#pragma optimize("y", off)
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
template <class RandomAccessIter, class Compare>
void __fastcall make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);              // 0x005F93E6
template <class RandomAccessIter, class T, class Compare, class Distance>
void __fastcall __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value,
                           Compare comp, Distance*);                                                  // 0x005F8D82
template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);              // 0x005F9B5A

template <class RandomAccessIter, class T, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

template void __fastcall __partial_sort<pair<long, long>*, pair<long, long>,
                                        CKeyPairCompareLess<long, long> >(
    pair<long, long>*, pair<long, long>*, pair<long, long>*, pair<long, long>*,
    CKeyPairCompareLess<long, long>);
}