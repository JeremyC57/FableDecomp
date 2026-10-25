// std::pop_heap(PairVectorLong* first, PairVectorLong* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- SGI/STLport front end: forwards the range and the 0x2C-byte comparator to __pop_heap_aux(first, last, (T*)0, comp)
// Leaf: pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809840, 97 bytes, `ret 2Ch` (__fastcall: first in ecx, last in edx --
// passed through untouched --, the 0x2C-byte comparator by value on the stack; the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x00808DB0 __pop_heap_aux(first, last, T*, comp) (`ret 30h`; manifest "_Pop_heap_0<pair<unsigned long,
// CSoundPair>*>" is propagated; it builds T(*(last - 1)) and calls __pop_heap 0x00808BD0). No direct caller is
// recorded for this body (an out-of-line COMDAT; the twin 0x00808A30 is the __unguarded_insertion_sort front end).
// Label: catalogued as `_Insertion_sort<pair<C3DVector,long>*, CCompareTargetedPositionsForFirstPersonTargeting>`
// (propagated over the byte-identical 0x00808A30); the body is SGI's `pop_heap(first, last, comp) {
// __pop_heap_aux(first, last, __VALUE_TYPE(first), comp); }` emitted out of line.
// Types (Ego_r.pdb): the comparator is one CVectorCompare_BestForTargeting (0x2C), the element std::pair<C3DVector,long>
// -- the models of the landed TU neighbour 0x008074A0 and of 0x00809590.
// Shape: arguments right to left: the comparator's implicit memberwise copy is built in the argument area (`sub esp,2Ch
// ; mov eax,esp`, ten dword moves and the bool byte), then the null value-type pointer is pushed (`push 0`).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct PairVectorLong {                                // std::pair<C3DVector, long>, 16 bytes
    C3DVector first;                                   // +0x00
    long      second;                                  // +0x0C
    PairVectorLong(const PairVectorLong& o) : first(o.first), second(o.second) { }
};

struct CVectorCompare_BestForTargeting {               // Ego_r.pdb, 0x2C bytes (implicit memberwise copy)
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

void __fastcall __pop_heap_aux(PairVectorLong* first, PairVectorLong* last, PairVectorLong* valueType, CCompareTargetedPositionsForFirstPersonTargeting comp);   // 0x00808DB0

void __fastcall pop_heap(PairVectorLong* first, PairVectorLong* last, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    __pop_heap_aux(first, last, (PairVectorLong*)0, comp);
}