// std::__unguarded_insertion_sort_aux(std::pair<unsigned long,CSoundPair>* first, std::pair<unsigned long,CSoundPair>* last, std::pair<unsigned long,CSoundPair>*, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- STLport unguarded insertion sort over the 12-byte sound-pair map entries with the sink step inlined
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe @ 0x0081A9E0, 112 bytes, `ret 8`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in edx;
// the value-type tag pointer and the empty 4-byte predicate on the stack.
// Callers: 0x0081B907 and the landed __final_insertion_sort 0x0081D400 (call at 0x0081D435).
// Identity: the element is 12 bytes keyed by an UNSIGNED dword (`cmp edi,ebp ; jae`), std::pair<unsigned long,
// CSoundPair> (Ego_r.pdb CSoundPair: long FirstSound, long LastSound) as named by the neighbouring catalog labels
// (0x0081D340..0x0081D400 make_heap<pair<unsigned long,CSoundPair>,CKeyPairCompareLess<unsigned long,CSoundPair>>);
// the row's own label `_Copy_impl<CNavigationPosition*,CNavigationPosition*>` is propagated.
// Shape: STLport `__unguarded_insertion_sort_aux` with `__unguarded_linear_insert(i, Tp(*i), comp)` inlined (the value
// held in edi / ebx / a frame word; the shifts are the trivial three-dword assignment).
class CSoundPair {
public:
    long FirstSound;                              // +0x0
    long LastSound;                               // +0x4
};
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};
}
template <class K, class V>
class CKeyPairCompareLess {                       // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, Tp val, Compare comp)
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

template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, Tp*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, Tp(*i), comp);
}
template void __fastcall __unguarded_insertion_sort_aux(pair<unsigned long, CSoundPair>*, pair<unsigned long, CSoundPair>*,
    pair<unsigned long, CSoundPair>*, CKeyPairCompareLess<unsigned long, CSoundPair>);
}