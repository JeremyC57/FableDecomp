// _STL::__partial_sort(pair<unsigned long, C3DHelperPoint>* first, pair<unsigned long, C3DHelperPoint>* middle, pair<unsigned long, C3DHelperPoint>* last, pair<unsigned long, C3DHelperPoint>*, CKeyPairCompareLess comp) -- heap-based partial sort of the helper-point table: heapify [first, middle), pop every smaller later entry into it, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A342F0, 110 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx,
//   middle in edx, last, the value-type pointer and the empty comparator on the stack).
// Identity: the depth-limit fallback of the C3DMeshStats helper-point introsort (20-byte family of 0x00A31280 / 0x00A329B0 /
//   0x00A32C20); the catalog's "Heap_PartialSort_Pair" is descriptive. Callees: __make_heap @ 0x00A32C20 (landed),
//   __pop_heap(first, last, result, T value, comp, int*) @ 0x00A32B50, sort_heap @ 0x00A338C0.
// Element: pair<unsigned long, C3DHelperPoint> (Ego_r.pdb C3DHelperPoint = C3DVector Position + long BoneIndex; C3DVector has
//   a user copy constructor, the landed 0x00A32440 / 0x00A32C20 model), so the by-value copy is five dword moves.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- make_heap inlined to __make_heap with null
//   value-type and distance pointers, comp in ebp, the copy built straight in the argument area.
namespace _STL {

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct C3DHelperPoint {                                // 16 bytes
    C3DVector Position;                                // +0x00
    long      BoneIndex;                               // +0x0c
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DHelperPoint> TEntry;    // 20 bytes

struct CKeyPairCompareLess {                           // CKeyPairCompareLess<unsigned long, C3DHelperPoint>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);   // 0x00A32C20

template <class RandomAccessIter, class T, class Compare, class Distance>
void __fastcall __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value, Compare comp,
                           Distance*);                                                        // 0x00A32B50

template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);       // 0x00A338C0

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (TEntry*)0, (int*)0);
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