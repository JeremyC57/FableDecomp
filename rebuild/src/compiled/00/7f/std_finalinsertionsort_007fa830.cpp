// std::__final_insertion_sort(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp) -- the closing pass of the targeting candidate sort: a guarded insertion sort of the first 16, then an unguarded one of the rest
// Leaf: ?__final_insertion_sort@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FA830, 96 bytes (`ret 34h`); called by sort 0x007FAE60. Catalog label
//   `std_IntroSort_Things`.
// Callees: the comparator's implicit copy constructor @ 0x007F9730 (out of line here: three copies), std::__insertion_sort
//   @ 0x007FA450, std::__unguarded_insertion_sort @ 0x007FA610.
// Shape: SGI `if (last - first > 16) { __insertion_sort(first, first + 16, comp); __unguarded_insertion_sort(first + 16,
//   last, comp); } else __insertion_sort(first, last, comp);` (`(last - first) & ~3` compared with 0x40).
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
void __fastcall __insertion_sort(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp);             // 0x007FA450
void __fastcall __unguarded_insertion_sort(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp);   // 0x007FA610

void __fastcall __final_insertion_sort(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp)
{
    if (last - first > 16) {
        __insertion_sort(first, first + 16, comp);
        __unguarded_insertion_sort(first + 16, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}
}