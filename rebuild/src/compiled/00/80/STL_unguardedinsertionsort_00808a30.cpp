// std::__unguarded_insertion_sort(PairVectorLong* first, PairVectorLong* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- SGI/STLport front end: forwards the range and the 0x2C-byte comparator to __unguarded_insertion_sort_aux(first, last, (T*)0, comp)
// Leaf: __unguarded_insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808A30, 97 bytes, `ret 2Ch` (__fastcall: first in ecx, last in edx --
// passed through untouched --, the 0x2C-byte comparator by value on the stack; the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x00808120 __unguarded_insertion_sort_aux(first, last, T*, comp) (manifest "_Copy_impl<CNavigationPosition*>"
// is propagated; its loop calls the landed __unguarded_linear_insert 0x008074A0 per element). Caller: 0x00809F02.
// Label: catalogued as `_Insertion_sort<pair<C3DVector,long>*, CCompareTargetedPositionsForFirstPersonTargeting>`; the
// body is SGI's `__unguarded_insertion_sort(first, last, comp) { __unguarded_insertion_sort_aux(first, last,
// __VALUE_TYPE(first), comp); }` emitted out of line.
// Types (Ego_r.pdb): the comparator is one CVectorCompare_BestForTargeting (0x2C), the element std::pair<C3DVector,long>
// -- the models of the landed TU neighbour 0x008074A0 and of 0x00809590.
// Shape: arguments right to left: the comparator's implicit memberwise copy is built in the argument area (`sub esp,2Ch
// ; mov eax,esp`, ten dword moves and the bool byte), then the null value-type pointer is pushed (`push 0`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct PairVectorLong {                                // std::pair<C3DVector, long>, 16 bytes
    C3DVector first;                                   // +0x00
    long      second;                                  // +0x0C
    PairVectorLong(const PairVectorLong& o) : first(o.first), second(o.second) { }
};

class CVectorCompare_BestForTargeting { public:               // Ego_r.pdb, 0x2C bytes (implicit memberwise copy)
    C3DVector AnchorPos;                               // +0x00
    C3DVector PreferredLine;                           // +0x0C
    float     AngleCompare;                            // +0x18
    float     SqrDistCompare;                          // +0x1C
    float     OverrideShortDistanceFactor;             // +0x20
    float     OverrideShortDistanceFalloff;            // +0x24
    bool      CompareInXYPlane;                        // +0x28
};

struct CCompareTargetedPositionsForFirstPersonTargeting {   // anonymous namespace in retail, 0x2C bytes
    CVectorCompare_BestForTargeting VectorCompare;     // +0x00
};

void __fastcall __unguarded_insertion_sort_aux(PairVectorLong* first, PairVectorLong* last, PairVectorLong* valueType, CCompareTargetedPositionsForFirstPersonTargeting comp);   // 0x00808120

void __fastcall __unguarded_insertion_sort(PairVectorLong* first, PairVectorLong* last, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    __unguarded_insertion_sort_aux(first, last, (PairVectorLong*)0, comp);
}