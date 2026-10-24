// std::__unguarded_linear_insert(pair<long,EHeroAbility>* last, pair<long,EHeroAbility> val, CKeyPairCompareLess<long,EHeroAbility> comp) -- STLport unguarded insertion step: shift the larger-keyed pairs up one slot and drop val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F7CD0, 44 bytes, `ret 0Ch` (/Gr TU: last in ecx; the 8-byte pair by value
//   and the empty comparator on the stack).
// Callers: __unguarded_insertion_sort_aux 0x005F85AB and __linear_insert 0x005F9A98 -- the sort of
//   CTCInventoryAbilities::AbilityButtons (CVectorMap<long,EHeroAbility>, retail +0x16C; its Sort 0x005FA47D reaches
//   std::sort 0x005FA3DD). Byte twin 0x005F7D15 is the pair<long,long> instantiation (ExpressionButtons).
// Label correction: the catalog `std_HeapSiftDownPair` is behavioural; the body is STLport `__unguarded_linear_insert`
//   (`next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`).
// Shape: size TU (`#pragma optimize("s", on)`); the key compare is signed (`cmp edx,[eax] ; jge`), the pair copies
//   word by word through esi (STLport pair: explicit memberwise copy ctor, implicit assignment).
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
template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template void __fastcall __unguarded_linear_insert<pair<long, EHeroAbility>*, pair<long, EHeroAbility>,
                                                   CKeyPairCompareLess<long, EHeroAbility> >(
    pair<long, EHeroAbility>*, pair<long, EHeroAbility>, CKeyPairCompareLess<long, EHeroAbility>);
}