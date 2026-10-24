// std::__unguarded_partition(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, THitLocationIntersectionElement pivot, THitLocationIntersectionElementPred comp) -- STLport quicksort partition of the hit-location ray intersections around a pivot key
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe @ 0x00817840, 144 bytes, `ret 24h`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in
// edx; the 32-byte pivot and the empty 4-byte predicate on the stack (32 + 4 = 0x24). Returns the cut in eax.
// Caller: the introsort loop 0x00818B00 (this chunk).
// Label: catalogued `std_partition_QuicksortSwap` (descriptive); the 32-byte element keyed by a signed long at +0 is the
// anonymous-namespace THitLocationIntersectionElement of the landed neighbours (Ego_r.pdb: long key +0,
// CHitLocation* hit_location +4, C3DVector intersection_pos +8, C3DVector outward_normal +0x14).
// Shape: STLport `__unguarded_partition` with iter_swap inlined: the swap temporary is built memberwise (C3DVector's
// user copy constructor makes the element's implicit copy constructor memberwise) into the dead pivot slot (only
// pivot.key, held in ebx, is read), the two assignments are the trivial 8-dword `rep movsd`.
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
template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template <class ForwardIter1, class ForwardIter2> inline void iter_swap(ForwardIter1 a, ForwardIter2 b) { swap(*a, *b); }

template <class RandomAccessIter, class Tp, class Compare>
RandomAccessIter __fastcall __unguarded_partition(RandomAccessIter first, RandomAccessIter last, Tp pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}
template THitLocationIntersectionElement* __fastcall __unguarded_partition(THitLocationIntersectionElement*,
    THitLocationIntersectionElement*, THitLocationIntersectionElement, THitLocationIntersectionElementPred);
}