// _STL::__pop_heap_aux(SKeyedBlock40* first, SKeyedBlock40* last, SKeyedBlock40*, CKeyPairCompareLess comp) -- pop the heap root of the (long key, 36-byte payload) pairs into the last slot and sift the old last element down from the root
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081BC90, 113 bytes (+int3 pad), `ret 8` (link-time __fastcall /Gr TU: first in ecx,
//   last in edx, then the value-type tag pointer and the empty comparator on the stack).
// Callee: __adjust_heap(first, 0, len, value, comp) @ 0x0081AB40 (__fastcall, `ret 30h`; manifest label
//   `_Adjust_heap<CDebugCone*,int,CDebugCone>` propagated).
// Label: the catalog's "_Pop_heap_0<CDebugSphere*,CDebugSphere>" is propagated; the body is STLport `__pop_heap_aux`
//   (the out-of-line copy behind `pop_heap(first, last, comp)`) for the 40-byte pairs of the landed _Sort_heap 0x0081C700.
// Shape: `__pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp, DISTANCE_TYPE(first))` with __pop_heap inline:
//   the by-value T(*(last - 1)) is a frame temporary (pair copy ctor: key dword, then `rep movsd` x9), `*result = *first`
//   the implicit `rep movsd` x10 straight into the last slot, the __adjust_heap value argument copied memberwise from the
//   temporary into the argument area, len = (last - 1) - first.
struct SPayload36 {                         // 36-byte POD payload (copied as one block)
    unsigned long m_words[9];
};

namespace _STL {
template <class A, class B> struct pair {
    A first;                                // +0x00
    B second;                               // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<long, SPayload36> SKeyedBlock40;

struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SKeyedBlock40& a, const SKeyedBlock40& b) const { return a.first < b.first; }
};

void __fastcall __adjust_heap(SKeyedBlock40* first, int holeIndex, int len, SKeyedBlock40 val, CKeyPairCompareLess comp);   // 0x0081AB40

template <class RandomAccessIter, class T, class Compare, class Distance>
__forceinline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __fastcall __pop_heap_aux(SKeyedBlock40* first, SKeyedBlock40* last, SKeyedBlock40*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SKeyedBlock40(*(last - 1)), comp, (int*)0);
}
}