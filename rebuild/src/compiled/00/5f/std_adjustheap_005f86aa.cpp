// std::__adjust_heap(pair<long,long>* first, int holeIndex, int len, pair<long,long> val, CKeyPairCompareLess<long,long> comp) -- STLport heap sift-down: move the larger child up until the hole reaches a leaf, then push val back up from there
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F86AA, 98 bytes, `ret 10h` (/Gr TU: first in ecx, holeIndex in edx; len,
//   the 8-byte pair by value and the empty comparator on the stack).
// Callee: __push_heap 0x005F7DD4 (landed, same instantiation).
// Callers: __pop_heap 0x005F8D82 (landed) and make_heap's loop 0x005F8E12 -- the heap half of the ExpressionButtons
//   (CVectorMap<long,long>) introsort; byte twin 0x005F8648 is the pair<long,EHeroAbility> instantiation.
// Label correction: the catalog `std_PopHeapRebalance` is behavioural; the body is STLport `__adjust_heap`
//   (`secondChild = 2 * holeIndex + 2` loop, `2 * (secondChild + 1)` step, the lone-child `secondChild - 1` case, then
//   `__push_heap(first, holeIndex, topIndex, val, comp)`).
// Shape: frame pointer kept with no locals in a size TU: `#pragma optimize("y", off)` + `("s", on)`; the by-value pair is
//   built in the argument area by STLport pair's memberwise copy constructor (`push ecx ; push ecx ; mov eax,esp`).
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
template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __push_heap(RandomAccessIter first, Distance holeIndex, Distance topIndex, T val, Compare comp);   // 0x005F7DD4

template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T val, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}

template void __fastcall __adjust_heap<pair<long, long>*, int, pair<long, long>,
                                       CKeyPairCompareLess<long, long> >(
    pair<long, long>*, int, int, pair<long, long>, CKeyPairCompareLess<long, long>);
}