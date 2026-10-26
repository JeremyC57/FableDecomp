// std::__median(CThing* const& a, CThing* const& b, CThing* const& c, CThingCompare_BestForTargeting2 comp) -- STLport median-of-three pivot choice of the player targeting candidates (best target first)
// Leaf: ??$__median@PAVCThing@@VCThingCompare_BestForTargeting2@@@std@@YIABQAVCThing@@ABQAV1@00VCThingCompare_BestForTargeting2@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F9F60, 132 bytes (+int3 pad to 144), `__fastcall` `ret 38h` (&a in ecx, &b in
//   edx; &c and the 52-byte comparator by value on the stack); returns the chosen reference in eax.
// Callee: CThingCompare_BestForTargeting2::operator()(const CThing*, const CThing*) const 0x007F9F00 (landed; PDB ??RCThingCompare_BestForTargeting2@@QBE_NPBVCThing@@0@Z).
// Identity (proven): _Tp = CThing*, _Compare = CThingCompare_BestForTargeting2. Its only caller is the landed __introsort_loop 0x007FAC80, which declares __median 0x007F9F60 over CThing* with this comparator (catalog binding ??$__median@PAVCThing@@VCThingCompare_BestForTargeting2@@@std@@..., which this source defines); `ret 38h` is &c plus the 52-byte comparator.
//   Family H005: 7 bodies that differ only in the `ret` immediate (the comparator's size) and, for a comparator returning
//   the nearer pointer, `test eax,eax` for `test al,al`; retail has no identical-code folding, so each address is exactly
//   one instantiation. The manifest label "_Debug_lt_pred<CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest>,CThing*,CThing*>" is descriptive.
// Shape: STLport 4.6 _algo.c __median verbatim (nested if/else returning a, b or c); each compare pushes the two element
//   values and calls the comparator out of line on `lea ecx,[esp+..]`, its own argument slot.
class CThing;
#pragma pack(push, 1)
class C3DVector {                                   // retail: byte-packed
public:
    float X, Y, Z;
};
#pragma pack(pop)
class CVectorCompare_BestForTargeting {             // Ego_r.pdb: 44 bytes
public:
    C3DVector AnchorPos;                            // +0x00
    C3DVector PreferredLine;                        // +0x0c
    float AngleCompare;                             // +0x18
    float SqrDistCompare;                           // +0x1c
    float OverrideShortDistanceFactor;              // +0x20
    float OverrideShortDistanceFalloff;             // +0x24
    bool CompareInXYPlane;                          // +0x28
};
class CThingCompare_BestForTargeting2 {             // Ego_r.pdb: 52 bytes
public:
    CThing* Hero;                                   // +0x00
    CVectorCompare_BestForTargeting VectorCompare;  // +0x04
    float EnemyPreferenceFactor;                    // +0x30
    bool operator()(const CThing* a, const CThing* b) const;   // 0x007F9F00
};

namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template CThing* const& __fastcall __median<CThing*, CThingCompare_BestForTargeting2 >(CThing* const&, CThing* const&, CThing* const&, CThingCompare_BestForTargeting2);
}