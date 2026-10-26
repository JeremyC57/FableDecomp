// std::__unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing**, CThingCompare_BestForTargeting2 comp) -- unguarded insertion sort of targeting candidates under the 52-byte best-for-targeting comparator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FA290, length 130 bytes (padded to 144).
// Leaf: __unguarded_insertion_sort_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is SGI/STLport
// `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` calling `__unguarded_linear_insert(i, *i, comp)` @ 0x007FA0E0
// (`ret 34h` = the comparator only: `i` in ecx, the CThing* value in edx). Its caller 0x007FA676 is the landed-family
// _Insertion_sort<CThing**, CThingCompare_BestForTargeting2>, and the comparator layout is Ego_r.pdb's (52 bytes):
// Hero +0x00, VectorCompare (CVectorCompare_BestForTargeting, 44 bytes) +0x04, EnemyPreferenceFactor +0x30.
// Convention: LTCG register form -- first in ecx, last in edx; the unused value-type pointer and the comparator by value
// on the stack (`ret 38h`).
// Callees: std::__unguarded_linear_insert<CThing**, CThing*, CThingCompare_BestForTargeting2> @ 0x007FA0E0.
// Shape: the comparator is copied into the argument area member by member (dwords through ecx/edx, the
// CompareInXYPlane byte from a hoisted bl, Hero from a hoisted ebp), which needs C3DVector's user copy constructor:
// with an all-POD comparator the implicit copy is a `rep movsd` of 13 dwords (55 v 130 bytes).
class CThing;
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CVectorCompare_BestForTargeting { public:        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                        // +0x00
    C3DVector PreferredLine;                    // +0x0c
    float     AngleCompare;                     // +0x18
    float     SqrDistCompare;                   // +0x1c
    float     OverrideShortDistanceFactor;      // +0x20
    float     OverrideShortDistanceFalloff;     // +0x24
    bool      CompareInXYPlane;                 // +0x28
};
class CThingCompare_BestForTargeting2 { public:        // Ego_r.pdb, 52 bytes
    CThing* Hero;                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;  // +0x04
    float EnemyPreferenceFactor;                // +0x30
};
namespace std {
void __fastcall __unguarded_linear_insert(CThing** last, CThing* val, CThingCompare_BestForTargeting2 comp);   // 0x007FA0E0
void __fastcall __unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing**, CThingCompare_BestForTargeting2 comp)
{
    for (CThing** i = first; i != last; ++i)
        __unguarded_linear_insert(i, *i, comp);
}
}