// std::__adjust_heap(CThing** first, int holeIndex, int len, CThing* value, CThingCompare_BestForTargeting2 comp) -- STLport heap sift-down for the targeting candidate heap: move the better child up until the hole reaches a leaf, then push `value` back up from there
// Leaf: ??$__adjust_heap@PAPAVCThing@@HPAV1@VCThingCompare_BestForTargeting2@@@std@@YIXPAPAVCThing@@HHPAV1@VCThingCompare_BestForTargeting2@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FA320, 297 bytes, __fastcall (first in ecx, holeIndex in edx; len, value and
//   the 52-byte comparator on the stack), `ret 3Ch`.
// Identity (proven): its landed callers __pop_heap 0x007FA680 and __make_heap 0x007FA710 call it as this instantiation (the
//   0x007FA680 binding is the template spelling used here); the body is STLport 4.6 __adjust_heap (secondChild = 2*hole+2,
//   the comparator on (first[sc], first[sc-1]), the `secondChild == len` tail, then __push_heap(first, hole, top, value, comp));
//   Ego_r.pdb's Dinkumware twin is ??$_Adjust_heap@PAPAVCThing@@HPAV1@VCThingCompare_BestForTargeting2@@@std@@.
// Label: the catalog's "_Adjust_heap<..>" is the Dinkumware spelling of the same algorithm.
// Callees: CThingCompare_BestForTargeting2::GetThingCompareMultiplier(const CThing*) const 0x007F9E70 (Ego_r private
//   ABEMPBVCThing@@; results discarded), CThing::GetPos() const 0x004C73D0, CVectorCompare_BestForTargeting::
//   GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (the comparator's operator() 0x007F9F00 inlined), std::
//   __push_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2> 0x007FA160 (__fastcall, comparator by value).
// Shape / layout: the landed family model (0x007FA680 / 0x007FA710): Ego_r CThingCompare_BestForTargeting2 (52 bytes) copied
//   member by member through C3DVector's user copy constructor.
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };

class CThing {
public:
    const C3DVector& GetPos() const;                             // 0x004C73D0
};

class CVectorCompare_BestForTargeting { public:                 // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                                        // +0x00
    C3DVector PreferredLine;                                    // +0x0c
    float     AngleCompare;                                     // +0x18
    float     SqrDistCompare;                                   // +0x1c
    float     OverrideShortDistanceFactor;                      // +0x20
    float     OverrideShortDistanceFalloff;                     // +0x24
    bool      CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
};

class CThingCompare_BestForTargeting2 {                         // Ego_r.pdb, 52 bytes
public:
    CThing* Hero;                                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;              // +0x04
    float EnemyPreferenceFactor;                                // +0x30
    bool operator()(const CThing* a, const CThing* b) const     // the landed 0x007F9F00 body, inlined here
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
private:
    float GetThingCompareMultiplier(const CThing* thing) const;  // 0x007F9E70
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);                   // 0x007FA160

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2>(CThing**, int, int, CThing*,
                                                                                             CThingCompare_BestForTargeting2);
}