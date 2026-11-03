// ApplyPartialAnimationAlpha (file static, C3DAnimationManager TU) -- blend one bone's keyframe toward a partial animation's keyframe by alpha
// Leaf: ?ApplyPartialAnimationAlpha@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9DC20, length 706 bytes; a same-TU static with VC7.1's custom register
//   convention: apply in al, keyframe in ecx, bone in esi, time / animation / alpha on the stack, callee-pop `ret 0Ch`.
// Identity: Ego_r.pdb undecorated static ApplyPartialAnimationAlpha; the Ego_r order of the TU's statics
//   (ApplyRelativeAnimationAlpha, ApplyPartialAnimationAlpha, IsAnimationLoaded, BeginLoadingAnimation,
//   ApplyBoneMorphDisplacement, ApplyParentTransformations) is the retail address order.
// Grade: DIFFER(ours v retail) -- best dl=23, 709 v 706 bytes: the inlined slerp dot product sums Z,Y,X,W where retail sums
//   Y,X,Z,W, and the interpolation tail loads `fld st(1) ; fmul [b.Z]` in another operand order. x87 reassociation: no source
//   lever moved it in 40+ grades (operand order, parentheses, extra locals, inline depth, extern probes).
// Callees: C3DAnimationSequenceData::GetKeyframeInterpolate 0x00A4C1F0; CQuaternion::SetSlerp (inline, the landed body with
//   GFFastArcCos / GFFastSin over g_ArcCosTable 0x013CB530 / g_CosTable 0x013CD550), C4DVector::Interpolate (inline).
// Lever found: `static void __stdcall` gives the register arguments and the callee-pop; a same-TU caller
//   (ApplyPartialAnimationAlphaCaller, a probe) makes the static emitted. A CQuaternion temporary for the slerp result.
#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif
#include "engine/C4DVector.h"
#include "engine/GFFloatToLongNear.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

__forceinline float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

class CQuaternion {
public:
    float X, Y, Z, W;
    __forceinline void SetSlerp(const CQuaternion& a, const CQuaternion& b, float t)
    {
        float cosom = a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
        float sign = 1.0f;
        if (cosom < 0.0f) {
            sign = -1.0f;
            cosom = -cosom;
        }
        float scale0;
        float scale1;
        if (1.0f - cosom > 0.05f) {
            float omega = GFFastArcCos(cosom);
            float sinom = (float)sqrt(1.0f - cosom * cosom);
            scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
            scale1 = GFFastSin(omega * t) * (1.0f / sinom);
        }
        else {
            scale0 = 1.0f - t;
            scale1 = t;
        }
        scale1 *= sign;
        X = scale0 * a.X + scale1 * b.X;
        Y = scale0 * a.Y + scale1 * b.Y;
        Z = scale0 * a.Z + scale1 * b.Z;
        W = scale0 * a.W + scale1 * b.W;
    }
};
class C3DKeyframe {
public:
    CQuaternion Rotation;                   // +0x00
    C4DVector   Position;                   // +0x10
    C4DVector   Scaling;                    // +0x20
};
class C3DAnimationSequenceData {
public:
    void GetKeyframeInterpolate(C3DKeyframe* keyframe, long bone, float time) const;   // 0x00A4C1F0
};
template <class T> class CCountedPointer {
public:
    T* Object;
    void* Info;
    T* operator->() const { return Object; }
};

static void __stdcall ApplyPartialAnimationAlpha(C3DKeyframe* keyframe, long bone, float time, const CCountedPointer<const C3DAnimationSequenceData>& animation, float alpha, bool apply)
{
    if (apply)
    {
        C3DKeyframe partial;
        animation->GetKeyframeInterpolate(&partial, bone, time);
        CQuaternion rotation;
        rotation.SetSlerp(keyframe->Rotation, partial.Rotation, alpha);
        keyframe->Rotation = rotation;
        keyframe->Position = keyframe->Position.Interpolate(partial.Position, alpha);
    }
}

void ApplyPartialAnimationAlphaCaller(C3DKeyframe* keyframes, long count, float time, const CCountedPointer<const C3DAnimationSequenceData>& animation, float alpha, const bool* apply)
{
    for (long i = 0; i < count; ++i)
        ApplyPartialAnimationAlpha(&keyframes[i], i, time, animation, alpha, apply[i]);
}
