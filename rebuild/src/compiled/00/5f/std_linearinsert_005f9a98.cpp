// std::__linear_insert(pair<long,EHeroAbility>* first, pair<long,EHeroAbility>* last, pair<long,EHeroAbility> val, CKeyPairCompareLess<long,EHeroAbility> comp) -- STLport guarded insertion step: a pair smaller than the first element shifts the whole range up and becomes the first, otherwise the unguarded insert places it
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F9A98, 67 bytes, `ret 0Ch` (/Gr TU: first in ecx, last in edx; the 8-byte
//   pair by value and the empty comparator on the stack).
// Callees: __copy_backward 0x005F7E23 (8-byte pairs, `ret 0Ch`), __unguarded_linear_insert 0x005F7CD0.
// Caller: __insertion_sort 0x005F9CDE (AbilityButtons CVectorMap<long,EHeroAbility> sort); byte twin 0x005F9ADB is the
//   pair<long,long> instantiation.
// Label correction: the catalog `std_heap_adjust_pair_element` is behavioural; the body is STLport `__linear_insert`
//   (`if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; }
//   else __unguarded_linear_insert(last, val, comp);`).
// Shape: frame pointer kept in a size TU (`#pragma optimize("y", off)` + `("s", on)`); copy_backward's pointer dispatch is
//   inlined (`__forceinline`: a size TU calls it otherwise) down to `__copy_backward(first, last, result,
//   random_access_iterator_tag(), (int*)0)`, the empty tag
//   temporary homed in the high byte of the dead comparator slot (`lea eax,[ebp+13h]`); the by-value pair for the
//   unguarded insert is built in the argument area by STLport pair's memberwise copy constructor.
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

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __fastcall __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                             const random_access_iterator_tag&, Distance*);   // 0x005F7E23
template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);    // 0x005F7CD0

template <class BidirectionalIter1, class BidirectionalIter2>
__forceinline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template void __fastcall __linear_insert<pair<long, EHeroAbility>*, pair<long, EHeroAbility>,
                                         CKeyPairCompareLess<long, EHeroAbility> >(
    pair<long, EHeroAbility>*, pair<long, EHeroAbility>*, pair<long, EHeroAbility>, CKeyPairCompareLess<long, EHeroAbility>);
}