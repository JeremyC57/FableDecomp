// _STL::__partial_sort(pair<unsigned long, C3DDummyObject>* first, pair<unsigned long, C3DDummyObject>* middle, pair<unsigned long, C3DDummyObject>* last, pair<unsigned long, C3DDummyObject>*, CKeyPairCompareLess comp) -- heap-based partial sort of the dummy-object table: heapify [first, middle), pop every smaller later entry into it, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34360, 177 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx,
//   middle in edx, last, the value-type pointer and the empty comparator on the stack).
// Identity: the depth-limit fallback of the C3DMeshStats dummy-object introsort (0x38-byte family of 0x00A31320 /
//   0x00A32500 / 0x00A32CC0); the catalog's "swap<CDebugSphere>" is propagated. Callees: __make_heap @ 0x00A32CC0,
//   __adjust_heap(first, holeIndex, len, T value, comp) @ 0x00A32500, sort_heap @ 0x00A339A0.
// Element: pair<unsigned long, C3DDummyObject> (0x38 bytes; Ego_r.pdb C3DDummyObject = CMatrix3x4 Transform + long
//   BoneIndex), ordered by the unsigned key.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- here __pop_heap is inlined: its by-value T(*i)
//   is a frame local (pair copy constructor: key, `rep movsd` x13), `*result = *first` the 14-dword assignment, then
//   __adjust_heap(first, 0, middle - first, value, comp) re-copies the local into the argument area (divide by 56 via
//   0x92492493); middle spilled to the frame.
namespace _STL {

struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject {                                // 0x34 bytes
    CMatrix3x4 Transform;                              // +0x00
    long       BoneIndex;                              // +0x30
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DDummyObject> TEntry;    // 0x38 bytes

struct CKeyPairCompareLess {                           // CKeyPairCompareLess<unsigned long, C3DDummyObject>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);   // 0x00A32CC0

template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);   // 0x00A32500

template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);       // 0x00A339A0

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (TEntry*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value, Compare comp,
                       Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

template void __fastcall __partial_sort<TEntry*, TEntry, CKeyPairCompareLess>(TEntry*, TEntry*, TEntry*, TEntry*, CKeyPairCompareLess);

}