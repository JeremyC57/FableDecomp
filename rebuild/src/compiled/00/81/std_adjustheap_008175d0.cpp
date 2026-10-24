// std::__adjust_heap(THitLocationIntersectionElement* first, int holeIndex, int len, THitLocationIntersectionElement val, THitLocationIntersectionElementPred comp) -- STLport heap sift-down of the hit-location ray intersections by key, then __push_heap
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe @ 0x008175D0, 186 bytes, `ret 28h`: __fastcall (the algorithm TUs are /Gr), first in ecx -> ebx,
// holeIndex in edx; len, the 32-byte value and the empty 4-byte predicate on the stack (4 + 32 + 4 = 0x28).
// Callers: the landed make_heap 0x00817950, 0x00818390 (pop_heap) and the partial sort 0x00818450 (this chunk).
// Callee: __push_heap 0x008170D0 (landed as _Push_heap, same instantiation).
// Label: catalogued `_Adjust_heap<CDebugBox*,int,CDebugBox>`, a propagated name: CDebugBox is 84 bytes (Ego_r.pdb); the
// 32-byte element keyed by a signed long at +0 is the anonymous-namespace THitLocationIntersectionElement of the
// landed neighbours (long key +0, CHitLocation* hit_location +4, C3DVector intersection_pos +8, outward_normal +0x14).
// Shape: STLport `__adjust_heap` -- topIndex = holeIndex (spilled to the `push ecx` frame word), secondChild =
// 2 * holeIndex + 2, `comp(first[secondChild], first[secondChild - 1])` -> secondChild-- (`cmp esi,edi ; jge ; dec eax`),
// the element moves are the trivial 8-dword `rep movsd` assignment, the value is copied into __push_heap's argument
// area word by word (C3DVector's user copy constructor makes the element's implicit copy constructor memberwise).
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
template <class RandomAccessIterator, class Distance, class Tp, class Compare>
void __fastcall __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, Tp val, Compare comp);   // 0x008170D0

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
template void __fastcall __adjust_heap(THitLocationIntersectionElement*, int, int, THitLocationIntersectionElement,
                                       THitLocationIntersectionElementPred);
}