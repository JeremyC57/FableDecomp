// std::__pop_heap(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, THitLocationIntersectionElement* result, THitLocationIntersectionElement val, THitLocationIntersectionElementPred comp, int*) -- STLport pop_heap core for the hit-location ray intersections: move the root into the result slot and sift the saved value down from the root
// Leaf: ??$__pop_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008178E0, 104 bytes, `ret 2Ch` (/Gr __fastcall: first in ecx, last in edx; result,
//   the 32-byte value, the empty predicate and the null distance pointer on the stack: 4 + 0x20 + 4 + 4 = 0x2C).
// Identity: callers are the two __pop_heap_aux / pop_heap front ends 0x00817A00 (`ret 8`) and 0x00817F20 (`ret 4`), both
//   `push 0 ; push comp ; <value copied into the argument area> ; push last-1 ; call` = STLport
//   `__pop_heap(first, last - 1, last - 1, _Tp(*(last - 1)), comp, _STLP_DISTANCE_TYPE(first))`; the callee is the
//   landed __adjust_heap<THitLocationIntersectionElement*,int,...> 0x008175D0 (same anonymous-namespace element of the
//   Ego_r.pdb hit-location code: long key, CHitLocation*, two C3DVectors; 32 bytes, `sar edx,5`). Catalog row was the
//   placeholder `_gapscan::sub_008178e0`.
// Shape: `*__result = *__first` is the trivial 8-dword `rep movsd`; the by-value value is copied word by word into
//   __adjust_heap's argument area; the distance is `(last - first)` as `sub ; sar 5`, holeIndex 0 in edx.
// Callees: std::__adjust_heap 0x008175D0.
struct CHitLocation;
class C3DVector { public:
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
void __fastcall __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, Tp val, Compare comp);   // 0x008175D0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template void __fastcall __pop_heap(THitLocationIntersectionElement*, THitLocationIntersectionElement*,
                                    THitLocationIntersectionElement*, THitLocationIntersectionElement,
                                    THitLocationIntersectionElementPred, int*);
}