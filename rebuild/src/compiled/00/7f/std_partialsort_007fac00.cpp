// std::partial_sort(CThing** first, CThing** middle, CThing** last, CThingCompare_BestForTargeting2 comp) -- partial_sort's front end over the targeting candidates
// Leaf: ??$partial_sort@PAPAVCThing@@VCThingCompare_BestForTargeting2@@@std@@YIXPAPAVCThing@@00VCThingCompare_BestForTargeting2@@@Z
// Fix (rename1): written as the STLport function template and its explicit instantiation, so the graded symbol
//   mangles as retail's template instance (it was a plain `__fastcall` function in namespace std); the comparator
//   and C3DVector are classes as in every PDB symbol. Bytes unchanged.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FAC00, 116 bytes (`ret 38h` + int3 padding). Catalog label `Resize_PassToImpl`
//   was a guess.
// Callees: std::__partial_sort<CThing**, CThing*, CThingCompare_BestForTargeting2> @ 0x007FAA20 (first in ecx, middle in
//   edx; last, the value-type pointer and the comparator on the stack).
// Shape: SGI `__partial_sort(first, middle, last, __VALUE_TYPE(first), comp)`; the comparator copied memberwise.
// Family: the SGI algorithm instantiations over CThing** with CThingCompare_BestForTargeting2 (/Gr TU: every free
//   template function is `__fastcall`, first in ecx, the second argument in edx, the rest and the 52-byte comparator by
//   value on the stack). Comparator model = the landed siblings 0x007FA290 / 0x007FA980 (C3DVector's user copy ctor
//   makes the implicit comparator copy memberwise) with the landed operator() 0x007F9F00 body inlined.
class CThing;
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };

class CThing {
public:
    const C3DVector& GetPosition();                             // 0x004C73D0
};

struct CVectorCompare_BestForTargeting {                        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                                        // +0x00
    C3DVector PreferredLine;                                    // +0x0c
    float     AngleCompare;                                     // +0x18
    float     SqrDistCompare;                                   // +0x1c
    float     OverrideShortDistanceFactor;                      // +0x20
    float     OverrideShortDistanceFalloff;                     // +0x24
    bool      CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point);      // 0x007F9660
};

class CThingCompare_BestForTargeting2 {                         // Ego_r.pdb, 52 bytes (a class: V in every PDB symbol)
public:
    CThing* Hero;                                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;              // +0x04
    float EnemyPreferenceFactor;                                // +0x30
    float GetThingCompareMultiplier(CThing* thing);             // 0x007F9E70
    bool operator()(CThing* a, CThing* b)                       // the landed 0x007F9F00 body, inlined here
    {
        float multiplierA = GetThingCompareMultiplier(a);
        float multiplierB = GetThingCompareMultiplier(b);
        const C3DVector& posB = b->GetPosition();
        const C3DVector& posA = a->GetPosition();
        float valueA = VectorCompare.GetCompareValueForPoint(posA);
        float valueB = VectorCompare.GetCompareValueForPoint(posB);
        if (valueA > valueB)
            return true;
        return false;
    }
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x007FAA20

template <class _RandomAccessIter, class _Compare>
void __fastcall partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CThing**)0, __comp);
}

template void __fastcall partial_sort<CThing**, CThingCompare_BestForTargeting2>(CThing**, CThing**, CThing**, CThingCompare_BestForTargeting2);
}
