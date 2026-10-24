// std::__adjust_heap(SKeyedBlock40* first, int holeIndex, int len, SKeyedBlock40 val, CKeyPairCompareLess comp) -- STLport heap sift-down of a (long key, 36-byte payload) pair heap, then __push_heap
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe @ 0x0081AB40, 154 bytes, `ret 30h`: __fastcall (the algorithm TUs are /Gr), first in ecx -> ebx,
// holeIndex in edx; len, the 40-byte value and the empty 4-byte predicate on the stack (4 + 40 + 4 = 0x30).
// Callers: the landed sort_heap 0x0081C700 (and its pop_heap/make_heap relatives over the same 40-byte element).
// Callee: __push_heap 0x00819BC0 (landed as _Push_heap over the same 40-byte record).
// Label: catalogued `_Adjust_heap<CDebugCone*,int,CDebugCone>`, a propagated name: Ego_r.pdb CDebugCone is 40 bytes but
// starts with a float vector and sorts by a float at +36, while this row compares the first dword SIGNED
// (`cmp esi,edi ; jge`). The element is a pair of a long key and a 36-byte POD payload (the landed sort_heap
// 0x0081C700 models it the same way, SKeyedBlock40); its real type is not recoverable from these bytes.
// Shape: the 0x008175D0 STLport `__adjust_heap`; the element moves are the trivial 10-dword `rep movsd`
// assignment, the value is copied into __push_heap's argument area by the pair's user copy constructor
// (`mov [eax],ecx ; lea edi,[eax+4] ; rep movsd` x9: key, then the payload block).
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
template <class RandomAccessIterator, class Distance, class Tp, class Compare>
void __fastcall __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, Tp val, Compare comp);   // 0x00819BC0

template <class RandomAccessIterator, class Distance, class Tp, class Compare>
void __fastcall __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, Tp val, Compare comp)
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
template void __fastcall __adjust_heap(SKeyedBlock40*, int, int, SKeyedBlock40, CKeyPairCompareLess);
}