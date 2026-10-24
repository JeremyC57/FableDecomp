// std::__unguarded_linear_insert(SKeyedRecord* last, SKeyedRecord val, SKeyLess comp) -- STLport insertion-sort sink step over 24-byte (long key, 20-byte POD) pairs: shift larger keys up one slot, then drop val in the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe @ 0x00819940, 112 bytes, `ret 1Ch`: __fastcall (the algorithm TUs are /Gr), last in ecx; the
// 24-byte value and the empty 4-byte predicate on the stack (24 + 4 = 0x1C).
// Callers: the landed __linear_insert 0x0081BDA0 and __unguarded_insertion_sort_aux 0x0081A8B0 (this chunk).
// Label: catalogued `std_InsertionSort_6Elements` (descriptive). The element is the 24-byte STLport pair<long, 20-byte
// POD> of the landed 0x0081BDA0 / 0x0081B910 neighbours (signed key compare `cmp edx,[eax] ; jge`); its real
// second type is not recoverable from these bytes.
// Shape: STLport `__unguarded_linear_insert` -- `next = last; --next; while (comp(val, *next)) { *last = *next;
// last = next; --next; } *last = val;` -- the moves are the trivial implicit assignment (six dword pairs), the key
// of val stays in edx.
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
template void __fastcall __unguarded_linear_insert(SKeyedRecord*, SKeyedRecord, SKeyLess);
}