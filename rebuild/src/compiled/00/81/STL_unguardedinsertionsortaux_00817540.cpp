// std::__unguarded_insertion_sort_aux(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, THitLocationIntersectionElement*, THitLocationIntersectionElementPred comp) -- SGI unguarded insertion sort of the hit-location ray intersections by key: every element sinks below its larger predecessors (no lower-bound test), the linear insert inlined
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00817540, 133 bytes, `ret 8` (__fastcall: first in ecx, last in edx, the
// unused value-type pointer and the empty comparator on the stack -- the SGI/STLport algorithm TUs are /Gr). No callees.
// Types (Ego_r.pdb, anonymous namespace 0x5EBD2A70): THitLocationIntersectionElement (32 bytes) = long key +0,
// CHitLocation* hit_location +4, C3DVector intersection_pos +8, C3DVector outward_normal +0x14;
// THitLocationIntersectionElementPred is empty and orders by the signed key (`cmp ebp,[ebx] ; jge`).
// Label: catalogued as `_Insertion_sort1<THitLocationIntersectionElement*, THitLocationIntersectionElementPred>`; the
// loop starts AT first and has no `comp(val, *first)` test, i.e. SGI's `__unguarded_insertion_sort_aux`:
// `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`.
// Shape: C3DVector has a user copy constructor, so the element's IMPLICIT copy constructor is memberwise -- eight dword
// moves into the value's 32-byte frame home, the key also kept in ebp -- while the implicit assignment stays a trivial
// 8-dword `rep movsd` (each shift and the final `*last = val`); the loop pointer is strength-reduced to `i + 10h`.
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

inline void __unguarded_linear_insert(THitLocationIntersectionElement* last, THitLocationIntersectionElement val, THitLocationIntersectionElementPred comp)
{
    THitLocationIntersectionElement* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall __unguarded_insertion_sort_aux(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last,
                                               THitLocationIntersectionElement*, THitLocationIntersectionElementPred comp)
{
    for (THitLocationIntersectionElement* i = first; i != last; ++i)
        __unguarded_linear_insert(i, THitLocationIntersectionElement(*i), comp);
}