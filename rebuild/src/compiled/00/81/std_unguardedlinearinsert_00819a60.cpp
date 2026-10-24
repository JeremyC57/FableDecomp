// std::__unguarded_linear_insert(std::pair<unsigned long,CSoundPair>* last, std::pair<unsigned long,CSoundPair> val, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- STLport insertion-sort sink step over the 12-byte sound-pair map entries: shift larger keys up one slot, then drop val in the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe @ 0x00819A60, 67 bytes, `ret 10h`: __fastcall (the algorithm TUs are /Gr), last in ecx; the 12-byte
// value and the empty 4-byte predicate on the stack (12 + 4 = 0x10).
// Callers: 0x0081BED1 (__linear_insert) and 0x0081C650 (__insertion_sort 0x0081C5F0, landed relative 0x0081D400).
// Identity: the element is 12 bytes keyed by an UNSIGNED dword (`cmp edx,[eax] ; jae`), which is exactly
// std::pair<unsigned long, CSoundPair> (Ego_r.pdb CSoundPair: long FirstSound, long LastSound = 8 bytes) named by the
// neighbouring catalog labels (0x0081B910 _Pop_heap / 0x0081D340..0x0081D400 make_heap<pair<unsigned long,CSoundPair>,
// CKeyPairCompareLess<unsigned long,CSoundPair>>); the row's own label `std_InsertionSort_3Elements` is descriptive.
// Shape: STLport `__unguarded_linear_insert`; the moves are the trivial implicit assignment (three dword pairs).
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
template void __fastcall __unguarded_linear_insert(pair<unsigned long, CSoundPair>*, pair<unsigned long, CSoundPair>,
                                                   CKeyPairCompareLess<unsigned long, CSoundPair>);
}