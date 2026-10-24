// std::make_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp) -- make_heap's front end over the targeting candidates
// Leaf: ?make_heap@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FA890, 113 bytes (`ret 34h` + int3 padding). Catalog label `std_MakeHeap_Wrapper`.
// Callees: std::__make_heap<CThing**, CThingCompare_BestForTargeting2, CThing*, int> @ 0x007FA710.
// Shape: SGI `__make_heap(first, last, comp, __VALUE_TYPE(first), __DISTANCE_TYPE(first))` = two null type pointers
//   pushed, then the memberwise comparator copy.
// Family: the SGI algorithm instantiations over CThing** with CThingCompare_BestForTargeting2 (/Gr TU: every free
//   template function is `__fastcall`, first in ecx, the second argument in edx, the rest and the 52-byte comparator by
//   value on the stack). Comparator model = the landed siblings 0x007FA290 / 0x007FA980 (C3DVector's user copy ctor
//   makes the implicit comparator copy memberwise) with the landed operator() 0x007F9F00 body inlined.
class CThing;
struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };

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

struct CThingCompare_BestForTargeting2 {                        // Ego_r.pdb, 52 bytes
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
void __fastcall __make_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp, CThing**, int*);   // 0x007FA710

void __fastcall make_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp)
{
    __make_heap(first, last, comp, (CThing**)0, (int*)0);
}
}