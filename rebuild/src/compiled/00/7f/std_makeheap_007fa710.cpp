// std::__make_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp, CThing**, int*) -- build a heap of targeting candidates: adjust every parent from the last one down to the root
// Leaf: ??$__make_heap@PAPAVCThing@@VCThingCompare_BestForTargeting2@@PAV1@H@std@@YIXPAPAVCThing@@0VCThingCompare_BestForTargeting2@@0PAH@Z
// Fix (rename1): written as the STLport function template and its explicit instantiation, so the graded symbol
//   mangles as retail's template instance (it was a plain `__fastcall` function in namespace std); the comparator
//   and C3DVector are classes as in every PDB symbol. Bytes unchanged.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FA710, 151 bytes (`ret 3Ch` + int3 padding); called by make_heap 0x007FA890.
//   Catalog label `std_MakeHeap_CThingCompare`.
// Callees: std::__adjust_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2> @ 0x007FA320 (first in ecx,
//   holeIndex in edx; len, value and the comparator on the stack).
// Shape: SGI `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; while (true) {
//   __adjust_heap(first, parent, len, *(first + parent), comp); if (parent == 0) return; parent--; }` -- the comparator
//   copied member by member each iteration (its CompareInXYPlane byte hoisted into bl).
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);   // 0x007FA320

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    while (true) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template void __fastcall __make_heap<CThing**, CThingCompare_BestForTargeting2, CThing*, int>(CThing**, CThing**, CThingCompare_BestForTargeting2, CThing**, int*);
}
