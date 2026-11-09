// std::__median(const pair<C3DVector, long>& a, const pair<C3DVector, long>& b, const pair<C3DVector, long>& c, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport median-of-three for the first-person targeting position sort (the pivot by best-for-targeting score, larger first)
// Comparator: the shared CVectorCompare_BestForTargeting header with its scoring bodies visible as inline (COMDAT) definitions
//   (FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES, as the landed 0x00808AA0 / 0x00807250 rows of this TU): c2 then knows
//   the comparator is not captured and loads c (`mov ebx,[esp+14h]`, retail 0x008071A5 and 0x008071FD) between the first
//   GetCompareValueForPoint call and the `fstp` of its result; a local model with a declaration-only scorer loads it after.
// Leaf: ??$__median@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807170, 216 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx, b in
//   edx, c and the 0x2c-byte comparator by value on the stack), `ret 30h`; the chosen element's address in eax.
// Name: STLport `__median`; Ego_r.pdb's Dinkumware counterparts are `_Med3`/`_Median<pair<C3DVector,long>*,
//   `anonymous namespace'::CCompareTargetedPositionsForFirstPersonTargeting>` (donor 0x00802C20 / 0x00803660). The
//   catalog's "std_introsort_Med3_TargetPoint" is descriptive.
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (twice per compare,
//   through the shared operator()).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting { CVectorCompare_BestForTargeting VectorCompare; }
//   (0x2c); pair<C3DVector, long> 16 bytes (the landed 0x008074A0 model).
// Shape: STLport `if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; } else if
//   (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;` -- comp(x, y) is VectorCompare(x.first, y.first),
//   i.e. `value(x.first) > value(y.first)`: x's value parked in the `push ecx` frame word, then `fcomp ; fnstsw ; test ah,5 ; jp`.
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES
#include "stlport/fable_containers.h"
#include "engine/CVectorCompare_BestForTargeting.h"

namespace {
class CCompareTargetedPositionsForFirstPersonTargeting {
public:
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        return VectorCompare(a.first, b.first);
    }
};
}

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

template const pair<C3DVector, long>& __fastcall __median(const pair<C3DVector, long>&, const pair<C3DVector, long>&,
                                                          const pair<C3DVector, long>&, CCompareTargetedPositionsForFirstPersonTargeting);
}