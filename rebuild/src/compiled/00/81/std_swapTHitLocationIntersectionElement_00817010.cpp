// std::swap<THitLocationIntersectionElement>(THitLocationIntersectionElement& a, THitLocationIntersectionElement& b) -- STLport swap of two 32-byte hit-location ray intersections
// Leaf: ??$swap@
// Retail: Fable.exe @ 0x00817010, 96 bytes, plain `ret`: __fastcall (the algorithm TUs are /Gr), a in ecx -> edi,
// b in edx. Its only caller is the `jmp` at 0x00817530 (iter_swap(a, b) { swap(*a, *b); } tail-called).
// Label: catalogued swap<CDebugSphere>, a propagated name -- CDebugSphere is 76 bytes (Ego_r.pdb) while this body
// swaps 32 bytes; the element is the anonymous-namespace THitLocationIntersectionElement of the landed neighbours
// (__equal_range 0x00817E30, _Push_heap 0x008170D0, __unguarded_insertion_sort_aux 0x00817540): long key +0,
// CHitLocation* hit_location +4, C3DVector intersection_pos +8, C3DVector outward_normal +0x14.
// Shape: C3DVector has a user copy constructor, so the element's IMPLICIT copy constructor is memberwise -- `tmp = a`
// is eight scheduled dword moves into the 32-byte frame -- while the implicit assignment is a trivial `rep movsd` x8
// (`a = b`, then `b = tmp`).
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
}
namespace std {
template <class T> inline void __fastcall swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
template void __fastcall swap<THitLocationIntersectionElement>(THitLocationIntersectionElement&, THitLocationIntersectionElement&);
}