// std::__unguarded_partition(CThing** first, CThing** last, CThing* pivot, CThingCompare_BestForTargeting2 comp) -- the introsort partition step of the targeting candidate sort: advance first past candidates ranked above the pivot, retreat last past those ranked below, swap, repeat
// Leaf: ??$__unguarded_partition@PAPAVCThing@@PAV1@VCThingCompare_BestForTargeting2@@@std@@YIPAPAVCThing@@PAPAV1@0PAV1@VCThingCompare_BestForTargeting2@@@Z
// Fix (rename1): written as the STLport function template and its explicit instantiation, so the graded symbol
//   mangles as retail's template instance (it was a plain `__fastcall` function in namespace std); the comparator
//   and C3DVector are classes as in every PDB symbol. Bytes unchanged.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9FF0, 231 bytes (`ret 38h` + int3 padding). Catalog label
//   `std_Partition_Quicksort_Things` was descriptive.
// Callees: CThingCompare_BestForTargeting2::GetThingCompareMultiplier @ 0x007F9E70, CThing::GetPosition @ 0x004C73D0,
//   CVectorCompare_BestForTargeting::GetCompareValueForPoint @ 0x007F9660 (the comparator inlined, twice).
// Shape: SGI `while (true) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`.
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
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    CThing* __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    while (true) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template CThing** __fastcall __unguarded_partition<CThing**, CThing*, CThingCompare_BestForTargeting2>(CThing**, CThing**, CThing*, CThingCompare_BestForTargeting2);
}
