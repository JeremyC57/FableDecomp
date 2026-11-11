// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_CVECTORCOMPARE_BESTFORTARGETING_H
#define FABLE_ENGINE_CVECTORCOMPARE_BESTFORTARGETING_H
// CVectorCompare_BestForTargeting (Ego_r.pdb, 0x2c bytes, class key V): the first-person targeting score of a candidate
// position -- a falloff in the squared distance from AnchorPos (plus a weighted short-distance override falloff) times a falloff
// in the angle between the direction to the point and PreferredLine; operator() orders two points best first.
// Names: Ego_r.pdb members; FableWin.pdb ?GetCompareValueForPoint@...@@QBEMABVC3DVector@@@Z (public),
// ?GetCompareValue@...@@ABEMMM@Z (private), ??R...@@QBE_NABVC3DVector@@0@Z (operator(), FableWin 0x024DF1C0; inline in retail).
// Retail: GetCompareValueForPoint 0x007F9660, GetCompareValue 0x007F9480 (landed rows).
#include "engine/C3DVector.h"

class CVectorCompare_BestForTargeting {
public:
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    // Original copy constructor is compiler-generated (__that).
    CVectorCompare_BestForTargeting(const C3DVector& pos,const C3DVector& line,float angle,float dist,float short_factor,float short_falloff):AnchorPos(pos),PreferredLine(line),AngleCompare(angle),SqrDistCompare(dist),OverrideShortDistanceFactor(short_factor),OverrideShortDistanceFalloff(short_falloff),CompareInXYPlane(false){}
    void SetAsTargetCompareInXYPlane(){CompareInXYPlane=true;}
private:
#endif
    C3DVector AnchorPos;                                   // +0x00
    C3DVector PreferredLine;                               // +0x0c
    float     AngleCompare;                                // +0x18
    float     SqrDistCompare;                              // +0x1c
    float     OverrideShortDistanceFactor;                 // +0x20
    float     OverrideShortDistanceFalloff;                // +0x24
    bool      CompareInXYPlane;                            // +0x28
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
public:
#endif
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
#ifdef FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING
    // Per-TU opt-in (chunk 892, rows 0x00807250 / 0x00808AA0, the first-person targeting sort TU): operator() as retail expands
    // it in the sort comparator. The two values are named locals with parenthesised initialisers: the parentheses keep c2's
    // no-code PAREN tuple on the first call's result, so the pivot `lea eax,[esp+24h]` issues between the call and the `fstp`
    // (0x0080726A) as in retail; the named locals give 0x00808AA0's `fld [tmp] ; fcomp st(1)` compare (0x00808AC3).
    bool operator()(const C3DVector& a, const C3DVector& b) const
    {
        float aValue = (GetCompareValueForPoint(a));
        float bValue = (GetCompareValueForPoint(b));
        return aValue > bValue;
    }
#endif
private:
    float GetCompareValue(float sqrDist, float angleDot) const;    // 0x007F9480
};

#ifdef FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES
// Per-TU opt-in (chunk 892, row 0x00808AA0 __linear_insert): the two scoring bodies visible as inline (COMDAT) definitions,
// as retail's LTCG build saw them: c2 then knows the comparator passed as `this` is never captured (the by-value comparator
// copy in the else arm pipelines its loads one store ahead, 0x00808B07..0x00808B5C), while it still assumes the standard
// scratch-register kills at the out-of-line calls (`last` stays in edi, the second call reloads ecx, 0x00808ABA). Plain
// out-of-line definitions in the TU give c2 their register usage as well (last in edx, 218 v 229 bytes). The logic is the
// landed 0x007F9480 / 0x007F9660 rows' (three intrinsic DOUBLE exps -- the float arguments are widened explicitly, as the
// landed row's hand declaration does implicitly, so <math.h>'s exp(float) overload is not chosen; `1.0f / sqrt` normalisation
// above 1e-4).
#if defined(_MSC_VER)
#pragma intrinsic(exp, sqrt)
#endif
inline float CVectorCompare_BestForTargeting::GetCompareValue(float sqrDist, float angleDot) const
{
    return (float)((exp((double)-(sqrDist * SqrDistCompare)) + OverrideShortDistanceFactor * exp((double)-(sqrDist * OverrideShortDistanceFalloff)))
                   * exp((double)-((1.0f - angleDot) * AngleCompare)));
}
inline float CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector& point) const
{
    C3DVector d = point - AnchorPos;
    if (CompareInXYPlane)
        d.Z = 0.0f;
    float sqrDist = d.Y * d.Y;
    sqrDist += d.X * d.X;
    sqrDist += d.Z * d.Z;
    if (sqrDist > 1e-4f)
        d *= 1.0f / (float)sqrt(sqrDist);
    return GetCompareValue(sqrDist, d.Dot(PreferredLine));
}
#endif

#endif // FABLE_ENGINE_CVECTORCOMPARE_BESTFORTARGETING_H
