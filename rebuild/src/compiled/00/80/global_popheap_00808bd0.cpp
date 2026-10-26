// _global::__pop_heap(PairVectorLong* first, PairVectorLong* last, PairVectorLong* result, PairVectorLong value, CCompareTargetedPositionsForFirstPersonTargeting comp, long*) -- SGI pop_heap core for the first-person targeting position heap: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808BD0, length 165 bytes, `ret 44h` (result, the 16-byte value, the 0x2c-byte
//   comparator and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the manifest name `_Pop_heap<pair<C3DVector,long>*,int,pair<C3DVector,long>,CCompareTargetedPositionsForFirstPersonTargeting>`
//   is right; callers 0x00808E28 (the __pop_heap_aux front end 0x00808DB0) and 0x0080AD16. TU neighbours: __adjust_heap
//   0x008082C0, __unguarded_linear_insert 0x008074A0 (landed, same element and comparator model).
// Callee: __adjust_heap<pair<C3DVector,long>*, int, pair<C3DVector,long>, CCompare...> @ 0x008082C0 (`ret 40h`).
// Layout: pair<C3DVector, long> = 16 bytes; the comparator wraps the 0x2c-byte CVectorCompare_BestForTargeting (PDB:
//   AnchorPos, PreferredLine, four floats, CompareInXYPlane).
// Shape: `*result = *first` is the pair's implicit (trivial) assignment, four dwords through edi; the by-value comparator and
//   value are rebuilt in the argument area member by member through ecx (C3DVector has a memberwise user copy constructor,
//   so the implicit copy constructors of the comparator and STLport's explicit pair copy constructor are memberwise; the
//   trailing bool goes through cl); the count is `(last - first) >> 4`.
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

void __fastcall __adjust_heap(PairVectorLong* first, long holeIndex, long len, PairVectorLong value,
                              CCompareTargetedPositionsForFirstPersonTargeting comp);   // 0x008082C0

void __fastcall __pop_heap(PairVectorLong* first, PairVectorLong* last, PairVectorLong* result, PairVectorLong value,
                           CCompareTargetedPositionsForFirstPersonTargeting comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}