// _STL::__median(const pair<C3DVector, long>& a, const pair<C3DVector, long>& b, const pair<C3DVector, long>& c, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport median-of-three for the first-person targeting position sort (the pivot by best-for-targeting score, larger first)
// Grade: DIFFER(216v216) -- scheduling only: in the two compares that first touch c, retail loads c (`mov ebx,[esp+14h]`) between the first GetCompareValueForPoint call and the `fstp` of its result, ours after the `fstp`; tried a nested CVectorCompare operator(), one and two named compare values (two named values is retail's comparator form, see 0x00808AA0)
// Leaf: ??$__median@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807170, 216 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx, b in
//   edx, c and the 0x2c-byte comparator by value on the stack), `ret 30h`; the chosen element's address in eax.
// Name: STLport `__median`; Ego_r.pdb's Dinkumware counterparts are `_Med3`/`_Median<pair<C3DVector,long>*,
//   `anonymous namespace'::CCompareTargetedPositionsForFirstPersonTargeting>` (donor 0x00802C20 / 0x00803660). The
//   catalog's "std_introsort_Med3_TargetPoint" is descriptive.
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (twice per compare).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting { CVectorCompare_BestForTargeting VectorCompare; }
//   (0x2c); pair<C3DVector, long> 16 bytes (the landed 0x008074A0 model).
// Shape: STLport `if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; } else if
//   (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;` -- comp(x, y) is `value(x.first) >
//   value(y.first)`: x's value parked in the `push ecx` frame word, then `fcomp ; fnstsw ; test ah,5 ; jp`.
struct C3DVector {
    float X, Y, Z;
};

struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;                                   // +0x00
    C3DVector PreferredLine;                               // +0x0c
    float AngleCompare;                                    // +0x18
    float SqrDistCompare;                                  // +0x1c
    float OverrideShortDistanceFactor;                     // +0x20
    float OverrideShortDistanceFalloff;                    // +0x24
    bool CompareInXYPlane;                                 // +0x28
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

namespace {
struct CCompareTargetedPositionsForFirstPersonTargeting {
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const _STL::pair<C3DVector, long>& a, const _STL::pair<C3DVector, long>& b) const
    {
        const float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        const float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}

namespace _STL {
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

template const pair<C3DVector, long>& __fastcall __median(const pair<C3DVector, long>&, const pair<C3DVector, long>&,
                                                          const pair<C3DVector, long>&, CCompareTargetedPositionsForFirstPersonTargeting);
}
