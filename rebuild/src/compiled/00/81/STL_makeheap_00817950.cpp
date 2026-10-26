// std::__make_heap(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, THitLocationIntersectionElementPred comp, THitLocationIntersectionElement*, int*) -- SGI make_heap of the hit-location ray intersections by key: sift every parent down from (len-2)/2 to the root with __adjust_heap, each fed a by-value copy of the parent
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00817950, 120 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx, the
// empty comparator's slot, the value-type pointer and the Distance* on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: std::__adjust_heap(first, holeIndex, len, T value, comp) @ 0x008175D0 (same instantiation; /Gr: first in ecx,
// holeIndex in edx, then len, the 32-byte value and the comparator slot).
// Types (Ego_r.pdb, anonymous namespace 0x5EBD2A70): THitLocationIntersectionElement (32 bytes) = long key +0,
// CHitLocation* hit_location +4, C3DVector intersection_pos +8, C3DVector outward_normal +0x14; the predicate is empty.
// Shape: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
// parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the element's implicit copy is
// memberwise (C3DVector has a user copy constructor), built straight into the argument area (`sub esp,20h ; mov eax,esp`)
// from a strength-reduced `first + parent*32 + 10h` pointer. Same element model as the matched 0x00817540 (chunk 356).
// Label: catalogued as Dinkumware `_Make_heap<THitLocationIntersectionElement*, int, ..., THitLocationIntersectionElementPred>`.
struct CHitLocation;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct THitLocationIntersectionElement {           // 32 bytes (implicit memberwise copy, trivial assignment)
    long          key;                             // +0x00
    CHitLocation* hit_location;                    // +0x04
    C3DVector     intersection_pos;                // +0x08
    C3DVector     outward_normal;                  // +0x14
};

struct THitLocationIntersectionElementPred {       // empty functor, passed in a 4-byte slot
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};

void __fastcall __adjust_heap(THitLocationIntersectionElement* first, int holeIndex, int len,
                              THitLocationIntersectionElement value, THitLocationIntersectionElementPred comp);   // 0x008175D0

void __fastcall __make_heap(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last,
                            THitLocationIntersectionElementPred comp, THitLocationIntersectionElement*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, THitLocationIntersectionElement(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}