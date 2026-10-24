// std::__unguarded_insertion_sort_aux(SKeyedBlock40* first, SKeyedBlock40* last, SKeyedBlock40*, CKeyPairCompareLess comp) -- STLport unguarded insertion sort over 40-byte (long key, 36-byte POD) pairs with the sink step inlined
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe @ 0x0081A940, 98 bytes, `ret 8`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in edx;
// the value-type tag pointer and the empty 4-byte predicate on the stack.
// Callers: 0x0081B877 and 0x0081D3D6 (the __final_insertion_sort front ends of this element).
// Label: catalogued `_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>`, a propagated name (CHeroMarriageInfo is 24 bytes in
// Ego_r.pdb; this stride is 0x28). The element is the (long key, 36-byte POD) pair of the landed sort_heap 0x0081C700
// and __adjust_heap 0x0081AB40 (this chunk), signed key compare; its real types are not recoverable from these bytes.
// Shape: STLport `__unguarded_insertion_sort_aux` with `__unguarded_linear_insert(i, Tp(*i), comp)` inlined: the
// value is rebuilt in the frame by the pair's user copy constructor (key, then `rep movsd` x9), the shifts and the
// final drop are the trivial 10-dword `rep movsd` assignment.
struct SPayload36 {                         // 36-byte POD payload (copied as one block)
    unsigned long m_words[9];
};
struct SKeyedBlock40 {                      // std::pair<long, payload> (0x28 bytes)
    long       first;                       // +0x00 sort key (signed compare)
    SPayload36 second;                      // +0x04
    SKeyedBlock40(const SKeyedBlock40& other) : first(other.first), second(other.second) { }
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SKeyedBlock40& a, const SKeyedBlock40& b) const { return a.first < b.first; }
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
template void __fastcall __unguarded_insertion_sort_aux(SKeyedBlock40*, SKeyedBlock40*, SKeyedBlock40*, CKeyPairCompareLess);
}