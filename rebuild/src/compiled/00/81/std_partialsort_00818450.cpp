// std::__partial_sort(THitLocationIntersectionElement* first, THitLocationIntersectionElement* middle, THitLocationIntersectionElement* last, THitLocationIntersectionElement*, THitLocationIntersectionElementPred comp) -- STLport heap-based partial sort of the hit-location ray intersections by key
// Leaf: ??$__partial_sort@
// Retail: Fable.exe @ 0x00818450, 235 bytes, `ret 0Ch`: __fastcall (the algorithm TUs are /Gr), first in ecx, middle in
// edx; last, the value-type tag pointer and the empty 4-byte predicate on the stack.
// Caller: the introsort loop 0x00818B00 (this chunk, the depth-limit fallback `partial_sort(first, last, last)`).
// Callees: __make_heap 0x00817950 (comp, Tp*, Distance* on the stack), __adjust_heap 0x008175D0, sort_heap 0x008182E0
// (all landed or in this chunk over the same element).
// Label: catalogued `std_partial_sort_Heap` (descriptive); the 32-byte element keyed by a signed long at +0 is the
// anonymous-namespace THitLocationIntersectionElement of the landed neighbours (Ego_r.pdb: long key +0,
// CHitLocation* hit_location +4, C3DVector intersection_pos +8, outward_normal +0x14).
// Shape: STLport `__partial_sort` -- make_heap, `for (i = middle; i < last; ++i) if (comp(*i, *first))
// __pop_heap(first, middle, i, Tp(*i), comp, Distance*)` with __pop_heap inlined (`Tp(*i)` is built before
// `*result = *first`, memberwise through registers and the frame, the C3DVector floats through fld/fstp), sort_heap.
// __pop_heap is `__forceinline` as in the landed sort_heap 0x008182E0: plain `inline` leaves it a call (by-value
// element with a non-trivial copy constructor).
struct CHitLocation;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
namespace {
struct THitLocationIntersectionElement {           // 32 bytes (implicit memberwise copy, trivial assignment)
    long          key;                             // +0x00
    CHitLocation* hit_location;                    // +0x04
    C3DVector     intersection_pos;                // +0x08
    C3DVector     outward_normal;                  // +0x14
};
struct THitLocationIntersectionElementPred {       // empty functor, passed in a 4-byte slot
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};
}
namespace std {
template <class RandomAccessIter, class Compare, class Tp, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, Tp*, Distance*);   // 0x00817950
template <class RandomAccessIter, class Distance, class Tp, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, Tp val, Compare comp);  // 0x008175D0
template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);                      // 0x008182E0

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (THitLocationIntersectionElement*)0, (int*)0);
}

template <class RandomAccessIter, class Tp, class Compare, class Distance>
__forceinline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, Tp val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Tp*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, Tp(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
template void __fastcall __partial_sort(THitLocationIntersectionElement*, THitLocationIntersectionElement*,
    THitLocationIntersectionElement*, THitLocationIntersectionElement*, THitLocationIntersectionElementPred);
}