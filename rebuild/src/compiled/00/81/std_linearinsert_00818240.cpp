// std::__linear_insert(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, THitLocationIntersectionElement val, THitLocationIntersectionElementPred comp) -- STLport insertion step for the hit-location ray intersections: shift the run up a slot and drop val at the front, or hand it to __unguarded_linear_insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe @ 0x00818240, 160 bytes, `ret 24h`: __fastcall (the algorithm TUs are /Gr), first in ecx -> ebp,
// last in edx; the 32-byte value and the empty 4-byte predicate on the stack (32 + 4 = 0x24).
// Callee: __unguarded_linear_insert 0x00817070 (__fastcall: last in ecx, the value copied into its argument area).
// Label: catalogued `std_copy_BackwardLarge` (descriptive, it names the inlined copy_backward); the 32-byte element
// keyed by a signed long at +0 is the anonymous-namespace THitLocationIntersectionElement of the landed neighbours
// (Ego_r.pdb: long key +0, CHitLocation* hit_location +4, C3DVector intersection_pos +8, outward_normal +0x14).
// Shape: STLport `__linear_insert` -- `comp(val, *first)` (`cmp ecx,[ebp] ; jge`), the random-access copy_backward
// `for (n = last - first; n > 0; --n) *--result = *--last;` inlined (`sar edx,5`, trivial 8-dword `rep movsd`),
// `*first = val`; the else arm copies val memberwise into the callee's argument area (C3DVector's user copy
// constructor makes the element's implicit copy constructor memberwise). Plain `inline` leaves copy_backward an
// out-of-line call here (measured, with and without the SGI Distance* tag level); `__forceinline` restores retail's body.
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
template <class BidirectionalIter1, class BidirectionalIter2>
__forceinline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, Tp val, Compare comp);   // 0x00817070

template <class RandomAccessIter, class Tp, class Compare>
void __fastcall __linear_insert(RandomAccessIter first, RandomAccessIter last, Tp val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
template void __fastcall __linear_insert(THitLocationIntersectionElement*, THitLocationIntersectionElement*,
    THitLocationIntersectionElement, THitLocationIntersectionElementPred);
}