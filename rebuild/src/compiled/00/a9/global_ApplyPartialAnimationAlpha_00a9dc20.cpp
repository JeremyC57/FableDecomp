// 00a9dc20 ApplyPartialAnimationAlpha
// Leaf: ?ApplyPartialAnimationAlpha@@YIXAAVC3DKeyframe@@_NJMPBVC3DAnimationSequence@@M@Z
// Flags: none
// Retail-asm-helper: GFFloatToLongNear (engine/GFFloatToLongNear.h)
// Retail-asm: 0x00A9DCCE `fistp dword ptr [esp]`, 0x00A9DD37 `fistp dword ptr [esp+68h]`, 0x00A9DDA0 `fistp dword ptr [esp]` -- the shared GFFloatToLongNear dword conversion.
// Inline-budget: the partial animation helper contains the expanded SetSlerp and BlendWith bodies, also present in the landed bone transformation TU.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DANIMATIONSEQUENCE_FRAMEKEYFRAME
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_ISIDENTITY
#define FABLE_RETAIL_C3DKEYFRAME_BLEND
#define FABLE_RETAIL_CQUATERNION_SLERP
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C3DKEYFRAME_CTOR
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimationSequence.h"
#include "engine/GFFloatToLongNear.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
extern float GFastArcCosTable[1026];
extern float GFastCosTable[1025];
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    float a = GFastArcCosTable[i];
    float b = GFastArcCosTable[i + 1];
    return ((1.0f - frac) * a + frac * b) * 0.15915494f;
}
// FableWin ?GFFastSin@@YIMM@Z 37i/121b (this text 35i/120b at /Od; GFFastCos sibling the same +2i/+1b): named table values.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    float a = GFastCosTable[i];
    float b = GFastCosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
}
inline float CQuaternion::Dot(const CQuaternion& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }
FABLE_FORCEINLINE void CQuaternion::SetSlerp(const CQuaternion& from, const CQuaternion& to, float t)
{
    float cosom = from.Dot(to);
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    float scale0, scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        scale1 = GFFastSin(omega * t) * (1.0f / sinom);
    } else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    X = scale0 * from.X + scale1 * to.X;
    Y = scale0 * from.Y + scale1 * to.Y;
    Z = scale0 * from.Z + scale1 * to.Z;
    W = scale0 * from.W + scale1 * to.W;
}
FABLE_FORCEINLINE void C3DKeyframe::BlendWith(const C3DKeyframe& other, float t)
{
    CQuaternion rotation;
    rotation.SetSlerp(Rotation, other.Rotation, t);
    Rotation = rotation;
    Position = Position.Interpolate(other.Position, t);
}

static void FABLE_FASTCALL ApplyPartialAnimationAlpha(C3DKeyframe& dest, bool enable, long frame, float fraction, const C3DAnimationSequence* sequence, float alpha)
{
    if (enable) {
        C3DKeyframe keyframe;
        sequence->GetKeyframeInterpolate(keyframe, frame, fraction);
        dest.BlendWith(keyframe, alpha);
    }
}

// Fidelity: scaffolding, not retail. A local compilation fixture keeps the original
// file-local function in the object when the rest of the animation TU is absent.
namespace {
class CPartialAnimationCompileContext {
public:
    void Evaluate(C3DKeyframe* frames, long count, const bool* enabled, const C3DAnimationSequence* sequence, float fraction, float alpha);
};
void CPartialAnimationCompileContext::Evaluate(C3DKeyframe* frames, long count, const bool* enabled, const C3DAnimationSequence* sequence, float fraction, float alpha)
{
    for (long i = 0; i < count; ++i)
        ApplyPartialAnimationAlpha(frames[i], enabled[i], i, fraction, sequence, alpha);
}
}