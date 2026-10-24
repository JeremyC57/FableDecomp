// _STL::__insertion_sort(PairVectorLong* first, PairVectorLong* last, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport insertion sort over the first-person targeting (position, index) pairs with the 0x2C-byte comparator passed by value: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809590, 159 bytes, `ret 2Ch` (__fastcall: first in ecx, last in edx, the
// 0x2C-byte comparator by value on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x00808AA0 __linear_insert(first, last, value, comp) (`ret 3Ch` = the 16-byte value + the comparator).
// Types (Ego_r.pdb): CCompareTargetedPositionsForFirstPersonTargeting (anonymous namespace, 0x2C) = one
// CVectorCompare_BestForTargeting { C3DVector AnchorPos, PreferredLine; float AngleCompare, SqrDistCompare,
// OverrideShortDistanceFactor, OverrideShortDistanceFalloff; bool CompareInXYPlane; }; the element is std::pair<C3DVector,
// long> -- the models of the landed TU neighbour 0x008074A0 and of 0x0080AA60.
// Label: catalogued as `_Insertion_sort1<NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesFirst>` (propagated).
// Shape: arguments are built right to left, both in the argument area: the comparator first -- its implicit memberwise
// copy (C3DVector has a user copy constructor, so ten direct dword moves re-read from the comparator slot each pass and
// the bool byte, hoisted once into bl; the three pad bytes are not copied) -- then STLport's explicit pair copy of `*i`.
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

void __fastcall __linear_insert(PairVectorLong* first, PairVectorLong* last, PairVectorLong value, CCompareTargetedPositionsForFirstPersonTargeting comp);   // 0x00808AA0

void __fastcall __insertion_sort(PairVectorLong* first, PairVectorLong* last, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    if (first == last)
        return;
    for (PairVectorLong* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}