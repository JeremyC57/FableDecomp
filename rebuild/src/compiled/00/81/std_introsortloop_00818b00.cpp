// std::__introsort_loop(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, THitLocationIntersectionElement*, int depth_limit, THitLocationIntersectionElementPred comp) -- STLport introsort recursion over the hit-location ray intersections: median-of-3 partition until 16 remain, heap partial_sort at the depth limit
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe @ 0x00818B00, 234 bytes, `ret 0Ch`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in
// edx; the value-type tag pointer, the depth limit and the empty 4-byte predicate on the stack. Recurses on itself.
// Callees: __unguarded_partition 0x00817840 and __partial_sort 0x00818450 (this chunk, same element).
// Label: catalogued `std_introsort_Quicksort` (descriptive); the 32-byte element keyed by a signed long at +0 is the
// anonymous-namespace THitLocationIntersectionElement of the landed neighbours (Ego_r.pdb: long key +0,
// CHitLocation* hit_location +4, C3DVector intersection_pos +8, outward_normal +0x14).
// Shape: STLport `__introsort_loop` -- `while (last - first > __stl_threshold)` (16: `and eax,-20h ; cmp eax,200h`),
// depth 0 -> `partial_sort(first, last, last, comp)`, else the `__median` of first / first + (last - first) / 2 /
// last - 1 copied memberwise into __unguarded_partition's argument area (C3DVector's user copy constructor),
// recursion on [cut, last), then last = cut.
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
template <class Tp, class Compare>
inline const Tp& __median(const Tp& a, const Tp& b, const Tp& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

template <class RandomAccessIter, class Tp, class Compare>
RandomAccessIter __fastcall __unguarded_partition(RandomAccessIter first, RandomAccessIter last, Tp pivot, Compare comp);  // 0x00817840
template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Tp*, Compare comp);  // 0x00818450

template <class RandomAccessIter, class Compare>
inline void partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Compare comp)
{
    __partial_sort(first, middle, last, (THitLocationIntersectionElement*)0, comp);
}

template <class RandomAccessIter, class Tp, class Size, class Compare>
void __fastcall __introsort_loop(RandomAccessIter first, RandomAccessIter last, Tp*, Size depth_limit, Compare comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        RandomAccessIter cut =
            __unguarded_partition(first, last,
                                  Tp(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                  comp);
        __introsort_loop(cut, last, (Tp*)0, depth_limit, comp);
        last = cut;
    }
}
template void __fastcall __introsort_loop(THitLocationIntersectionElement*, THitLocationIntersectionElement*,
    THitLocationIntersectionElement*, int, THitLocationIntersectionElementPred);
}