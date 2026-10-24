// _STL::__partial_sort(SKeyedBlock40* first, SKeyedBlock40* middle, SKeyedBlock40* last, SKeyedBlock40*, CKeyPairCompareLess comp) -- heap-select the smallest (long key, 36-byte payload) pairs into [first, middle) and sort them
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081D4D0, 175 bytes (+int3 pad), `ret 0Ch` (link-time __fastcall /Gr TU: first in ecx,
//   middle in edx, then last, the value-type tag pointer and the empty comparator on the stack).
// Callees: __make_heap @ 0x0081BB50 (first, middle, comp, 0, 0), __adjust_heap @ 0x0081AB40 (the inlined __pop_heap),
//   sort_heap @ 0x0081C700 (landed as `_Sort_heap`, `ret 4`).
// Label: the catalog's "swap<CDebugSphere>" is propagated; the body is STLport `__partial_sort`, the 40-byte instance of
//   the landed 0x00759420 (the particle-morph pairs of the landed _Sort_heap 0x0081C700 / sort 0x0081E090 family).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, T(*i), comp, DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` with make_heap and __pop_heap inline:
//   T(*i) is a frame temporary (pair copy ctor: key dword, then `rep movsd` x9), `*i = *first` the implicit `rep movsd`
//   x10, the __adjust_heap value argument copied memberwise into the argument area; middle is homed in the frame and
//   reloaded after each pop.
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

void __fastcall __make_heap(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess comp, SKeyedBlock40*, int*);           // 0x0081BB50
void __fastcall __adjust_heap(SKeyedBlock40* first, int holeIndex, int len, SKeyedBlock40 val, CKeyPairCompareLess comp);    // 0x0081AB40
void __fastcall sort_heap(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess comp);                              // 0x0081C700

inline void make_heap(SKeyedBlock40* first, SKeyedBlock40* last, CKeyPairCompareLess comp)
{
    __make_heap(first, last, comp, (SKeyedBlock40*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __fastcall __partial_sort(SKeyedBlock40* first, SKeyedBlock40* middle, SKeyedBlock40* last, SKeyedBlock40*, CKeyPairCompareLess comp)
{
    make_heap(first, middle, comp);
    for (SKeyedBlock40* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SKeyedBlock40(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
}