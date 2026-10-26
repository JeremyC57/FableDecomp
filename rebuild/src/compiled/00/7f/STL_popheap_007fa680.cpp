// std::__pop_heap(CThing** first, CThing** last, CThing** result, CThing* value, CThingCompare_BestForTargeting2 comp, int*) -- SGI heap pop step for the targeting candidate heap: move the root into the result slot, then sift `value` down from the root over [first, last)
// Leaf: ??$__pop_heap@PAPAVCThing@@PAV1@VCThingCompare_BestForTargeting2@@H@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FA680, 130 bytes (+ int3 padding), __fastcall (first in ecx, last in edx;
//   result, value, the 52-byte comparator and the distance-type pointer on the stack), `ret 40h`.
// Identity: called by the landed __pop_heap_aux 0x007FA7B0 (which declares it as this instantiation); the catalog label
//   `sort<_Vector_iterator<..CThing*..>, CThingCompare_BestForTargeting2>` is propagated.
// Callees: std::__adjust_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2> @ 0x007FA320 (first in ecx,
//   holeIndex 0 in edx; len, value and the comparator copy on the stack).
// Shape: SGI `*result = *first; __adjust_heap(first, _Distance(0), _Distance(last - first), value, comp);` -- the
//   comparator is copied memberwise into the argument area first (C3DVector's user copy constructor makes the implicit
//   comparator copy member by member; the CompareInXYPlane byte through cl), then `value` and the length are pushed.
// Family: the SGI algorithm instantiations over CThing** with CThingCompare_BestForTargeting2 (/Gr TU); comparator model =
//   the landed siblings 0x007FA710 / 0x007FA7B0 (Ego_r.pdb CThingCompare_BestForTargeting2, 52 bytes).
class CThing;
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };

class CVectorCompare_BestForTargeting { public:                        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                                        // +0x00
    C3DVector PreferredLine;                                    // +0x0c
    float     AngleCompare;                                     // +0x18
    float     SqrDistCompare;                                   // +0x1c
    float     OverrideShortDistanceFactor;                      // +0x20
    float     OverrideShortDistanceFalloff;                     // +0x24
    bool      CompareInXYPlane;                                 // +0x28
};

class CThingCompare_BestForTargeting2 {                         // Ego_r.pdb, 52 bytes
public:
    CThing* Hero;                                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;              // +0x04
    float EnemyPreferenceFactor;                                // +0x30
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);   // 0x007FA320

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __value, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __value, __comp);
}

template void __fastcall __pop_heap<CThing**, CThing*, CThingCompare_BestForTargeting2, int>(CThing**, CThing**, CThing**, CThing*,
                                                                                           CThingCompare_BestForTargeting2, int*);
}