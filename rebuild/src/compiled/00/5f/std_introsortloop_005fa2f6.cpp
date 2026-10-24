// std::__introsort_loop(pair<long,long>* first, pair<long,long>* last, pair<long,long>*, int depth_limit, CKeyPairCompareLess<long,long> comp) -- STLport introsort loop: median-of-three partition and recurse on the right part while more than 16 pairs remain, falling back to partial_sort when the depth limit runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA2F6, 139 bytes, `ret 0Ch` (/Gr TU: first in ecx -> ebx, last in edx ->
//   edi; the value-type tag pointer, the depth limit and the empty comparator on the stack).
// Callees: __median 0x005F84F3, __unguarded_partition 0x005F8CE2, itself, partial_sort 0x005FA0D5 (the out-of-line
//   front end that pushes the null value-type pointer for __partial_sort 0x005F9DA3).
// Caller: std::sort 0x005FA41F, reached from CVectorMap<long,long>::Sort 0x005FA496 (ExpressionButtons).
// Label correction: the catalog `Std_Introsort_8BytePartitionRecurse_Alt` is behavioural; the body is STLport
//   `__introsort_loop` (byte twin 0x005FA26B, the pair<long,EHeroAbility> instantiation).
// Shape: frame pointer kept with no locals in a size TU (`#pragma optimize("y", off)` + `("s", on)`); `last - first > 16`
//   becomes `and eax,-8 ; cmp eax,80h`; the comparator push shared by both arms is hoisted above the depth test; the
//   decremented depth limit lives in its argument slot (`dec dword ptr [ebp+0Ch]`); the by-value pivot `T(__median(..))`
//   is built in the argument area by STLport pair's memberwise copy constructor.
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
const int __stl_threshold = 16;

template <class T, class Compare>
const T& __fastcall __median(const T& a, const T& b, const T& c, Compare comp);                    // 0x005F84F3
template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __fastcall __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot,
                                                  Compare comp);                                     // 0x005F8CE2
template <class RandomAccessIter, class Compare>
void __fastcall partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last,
                             Compare comp);                                                          // 0x005FA0D5

template <class RandomAccessIter, class T, class Size, class Compare>
void __fastcall __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp)
{
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        RandomAccessIter cut =
            __unguarded_partition(first, last,
                                  T(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                  comp);
        __introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}

template void __fastcall __introsort_loop<pair<long, long>*, pair<long, long>, int,
                                          CKeyPairCompareLess<long, long> >(
    pair<long, long>*, pair<long, long>*, pair<long, long>*, int,
    CKeyPairCompareLess<long, long>);
}