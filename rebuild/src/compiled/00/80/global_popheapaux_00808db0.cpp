// _global::__pop_heap_aux(PairVectorLong* first, PairVectorLong* last, PairVectorLong*, CCompareTargetedPositionsForFirstPersonTargeting comp) -- SGI pop_heap front end for the first-person targeting position heap: move the back element out and sift
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808DB0, length 129 bytes (+ int3 padding), `ret 30h` (the unused T* and the
//   0x2c-byte comparator on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's `_Pop_heap_0<pair<unsigned long,CSoundPair>*,...>` is propagated; the callee is the
//   pair<C3DVector,long> / CCompareTargetedPositionsForFirstPersonTargeting __pop_heap 0x00808BD0 (`ret 44h`), so this is
//     __pop_heap_aux(first, last, T*, comp) { __pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp, __DISTANCE_TYPE(first)); }
//   of that instantiation (16-byte element: `add edx,-10h`).
// Callee: __pop_heap @ 0x00808BD0.
// Shape: arguments right to left: the null distance pointer (`push 0`), the comparator copied member by member into the
//   argument area (trailing bool through cl), the element copy built in place from *(last - 1) by STLport's explicit pair copy
//   constructor, then result = last - 1 pushed and also passed in edx, first in ecx.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

struct PairVectorLong {                                // std::pair<C3DVector, long>, 16 bytes
    C3DVector first;                                   // +0x00
    long      second;                                  // +0x0c
    PairVectorLong(const PairVectorLong& other) : first(other.first), second(other.second) { }
};

class CVectorCompare_BestForTargeting { public:               // 0x2c bytes (Ego_r.pdb, matches retail)
    C3DVector AnchorPos;                               // +0x00
    C3DVector PreferredLine;                           // +0x0c
    float     AngleCompare;                            // +0x18
    float     SqrDistCompare;                          // +0x1c
    float     OverrideShortDistanceFactor;             // +0x20
    float     OverrideShortDistanceFalloff;            // +0x24
    bool      CompareInXYPlane;                        // +0x28
};

struct CCompareTargetedPositionsForFirstPersonTargeting {      // anonymous namespace in retail, 0x2c bytes
    CVectorCompare_BestForTargeting VectorCompare;             // +0x00
};

void __fastcall __pop_heap(PairVectorLong* first, PairVectorLong* last, PairVectorLong* result, PairVectorLong value,
                           CCompareTargetedPositionsForFirstPersonTargeting comp, long* distance);   // 0x00808BD0

void __fastcall __pop_heap_aux(PairVectorLong* first, PairVectorLong* last, PairVectorLong*,
                               CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    __pop_heap(first, last - 1, last - 1, PairVectorLong(*(last - 1)), comp, (long*)0);
}