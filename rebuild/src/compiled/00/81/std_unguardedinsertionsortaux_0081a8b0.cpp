// std::__unguarded_insertion_sort_aux(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord*, SKeyLess comp) -- STLport unguarded insertion sort over 24-byte (long key, 20-byte POD) pairs: sink every element of [first, last) with __unguarded_linear_insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe @ 0x0081A8B0, 80 bytes, `ret 8`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in edx;
// the value-type tag pointer and the empty 4-byte predicate on the stack.
// Callers: 0x0081B7F7 and 0x0081D376 (the __final_insertion_sort front ends of this element, landed relative 0x0081D400).
// Callee: __unguarded_linear_insert 0x00819940 (this chunk).
// Label: catalogued `_Copy_impl<CNavigationPosition*,CNavigationPosition*>`, a propagated name (CNavigationPosition is
// 12 bytes in Ego_r.pdb; this stride is 0x18). The element is the 24-byte STLport pair<long, 20-byte POD> of the
// landed 0x0081BDA0 / 0x0081B910 neighbours; its real second type is not recoverable from these bytes.
// Shape: STLport `__unguarded_insertion_sort_aux` -- `for (i = first; i != last; ++i) __unguarded_linear_insert(i,
// Tp(*i), comp);` -- the by-value Tp(*i) is built in the argument area by STLport's explicit pair copy constructor
// (the key, then the POD second as one struct copy through re-based pointers: `add eax,4 ; lea edx,[esi+4]`).
struct SRecordInfo {                              // 20 bytes, POD
    long m_words[5];
};
struct SKeyedRecord {                             // STLport std::pair<long, SRecordInfo>, 24 bytes
    long        first;                            // +0x00 key
    SRecordInfo second;                           // +0x04
    SKeyedRecord(const SKeyedRecord& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};
struct SKeyLess {                                 // CKeyPairCompareLess<long, SRecordInfo>, empty
    bool operator()(const SKeyedRecord& a, const SKeyedRecord& b) const { return a.first < b.first; }
};
namespace std {
template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, Tp val, Compare comp);   // 0x00819940

template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, Tp*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, Tp(*i), comp);
}
template void __fastcall __unguarded_insertion_sort_aux(SKeyedRecord*, SKeyedRecord*, SKeyedRecord*, SKeyLess);
}