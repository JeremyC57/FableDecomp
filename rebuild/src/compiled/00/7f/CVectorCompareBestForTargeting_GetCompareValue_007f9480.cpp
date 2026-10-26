// CVectorCompare_BestForTargeting::GetCompareValue(float sqrDist, float angleDot) const -- the targeting score of a candidate: a distance falloff (plus the weighted short-distance override falloff) times an angle falloff on 1 - dot
// Leaf: ?GetCompareValue@CVectorCompare_BestForTargeting@@QBEMMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9480, 109 bytes (`ret 8`), float in st(0). FableWin.pdb
//   `?GetCompareValue@CVectorCompare_BestForTargeting@@ABEMMM@Z` (private thiscall const; modelled public, hence the QBE
//   leaf); its caller is GetCompareValueForPoint 0x007F9680 (squared distance and the dot with PreferredLine).
//   Catalog label `CVectorCompare_BestForTargeting_ComputeValue` was descriptive.
// Layout: CVectorCompare_BestForTargeting (Ego_r.pdb, 0x2c): AnchorPos +0x00, PreferredLine +0x0c, AngleCompare +0x18,
//   SqrDistCompare +0x1c, OverrideShortDistanceFactor +0x20, OverrideShortDistanceFalloff +0x24, CompareInXYPlane +0x28.
// Constants: 1.0f @ 0x0122DED8. Three intrinsic exps (`fldl2e ; ... f2xm1 ; fscale`).
// Shape: the DOUBLE exp only (declared by hand): <math.h>'s inline `exp(float)` -> `expf` overload keeps one exp result
//   on the x87 stack and spills another (`fld st(1)`, `fst [esp+4]`, 119 v 109).
extern "C" double __cdecl exp(double x);
#pragma intrinsic(exp)

class C3DVector { public: float X, Y, Z; };

class CVectorCompare_BestForTargeting {
public:
    C3DVector AnchorPos;                                        // +0x00
    C3DVector PreferredLine;                                    // +0x0c
    float     AngleCompare;                                     // +0x18
    float     SqrDistCompare;                                   // +0x1c
    float     OverrideShortDistanceFactor;                      // +0x20
    float     OverrideShortDistanceFalloff;                     // +0x24
    bool      CompareInXYPlane;                                 // +0x28
    float GetCompareValue(float sqrDist, float angleDot) const;
};

float CVectorCompare_BestForTargeting::GetCompareValue(float sqrDist, float angleDot) const
{
    return (float)((exp(-(sqrDist * SqrDistCompare)) + OverrideShortDistanceFactor * exp(-(sqrDist * OverrideShortDistanceFalloff)))
                   * exp(-((1.0f - angleDot) * AngleCompare)));
}