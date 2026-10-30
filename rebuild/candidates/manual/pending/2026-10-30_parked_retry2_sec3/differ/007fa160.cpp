// std::__push_heap(CThing** first, int holeIndex, int topIndex, CThing* value, CThingCompare_BestForTargeting2 comp) -- sift a targeting candidate up the heap from holeIndex toward topIndex
// Grade: DIFFER(197v197) -- x87 store of valueA scheduled before the posB push; tried (this sweep): compare-expression form, SGI template + explicit instantiation with the landed sibling model (GetPos const, GetCompareValueForPoint const)
// Leaf: __push_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FA160, 197 bytes (`ret 3Ch`). Catalog label
//   `_Push_heap<CThing**,int,CThing*,CThingCompare_BestForTargeting2>` (the Dinkumware spelling of the PDB build).
// Callees: CThingCompare_BestForTargeting2::GetThingCompareMultiplier @ 0x007F9E70, CThing::GetPosition @ 0x004C73D0,
//   CVectorCompare_BestForTargeting::GetCompareValueForPoint @ 0x007F9660 (the comparator inlined).
// Shape: SGI `parent = (holeIndex - 1) / 2; while (holeIndex > topIndex && comp(*(first + parent), value)) { ... }
//   *(first + holeIndex) = value;` (signed halving: `cdq ; sub ; sar`).
// Family: the SGI algorithm instantiations over CThing** with CThingCompare_BestForTargeting2 (/Gr TU: every free
//   template function is `__fastcall`, first in ecx, the second argument in edx, the rest and the 52-byte comparator by
//   value on the stack). Comparator model = the landed siblings 0x007FA290 / 0x007FA980 (C3DVector's user copy ctor
//   makes the implicit comparator copy memberwise) with the landed operator() 0x007F9F00 body inlined.
class CThing;
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };

class CThing {
public:
    const C3DVector& GetPos() const;                             // 0x004C73D0
};

class CVectorCompare_BestForTargeting { public:                        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                                        // +0x00
    C3DVector PreferredLine;                                    // +0x0c
    float     AngleCompare;                                     // +0x18
    float     SqrDistCompare;                                   // +0x1c
    float     OverrideShortDistanceFactor;                      // +0x20
    float     OverrideShortDistanceFalloff;                     // +0x24
    bool      CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point) const;      // 0x007F9660
};

class CThingCompare_BestForTargeting2 { public:                        // Ego_r.pdb, 52 bytes
    CThing* Hero;                                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;              // +0x04
    float EnemyPreferenceFactor;                                // +0x30
    float GetThingCompareMultiplier(CThing* thing);             // 0x007F9E70
    bool operator()(CThing* a, CThing* b)                       // the landed 0x007F9F00 body, inlined here
    {
        float multiplierA = GetThingCompareMultiplier(a);
        float multiplierB = GetThingCompareMultiplier(b);
        const C3DVector& posB = b->GetPos();
        const C3DVector& posA = a->GetPos();
        float valueA = VectorCompare.GetCompareValueForPoint(posA);
        float valueB = VectorCompare.GetCompareValueForPoint(posB);
        if (valueA > valueB)
            return true;
        return false;
    }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __value, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __value)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __value;
}

template void __fastcall __push_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2>(CThing**, int, int, CThing*, CThingCompare_BestForTargeting2);
}
