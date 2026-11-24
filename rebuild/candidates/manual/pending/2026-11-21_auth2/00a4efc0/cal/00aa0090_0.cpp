// C3DAnimationManager::CalcBlendedMeshTransformations(const C3DAnimationBlendState&, const C3DAnimationBlendState&, float, const C3DVector*, const C3DMesh2&, CMatrix4x4*, unsigned long, bool) -- blend two animation blend states into bone matrices
// Leaf: ?CalcBlendedMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MPBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA0090, length 2390 bytes (`ret 20h`), thiscall. Identity: Ego_r.pdb / FableWin.pdb
//   ?CalcBlendedMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MPBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z
//   (8 arguments; callers 0x00AA0A4A = the CMatrix3x4 overload 0x00AA09F0, 0x00BD2E35).
// Callees: as the twin 0x00A9F540 (src/00a9f540.cpp, RELOCATION_MATCH) with CombineBlendedBaseAnimations 0x00A9F2F0 for the base.
// Shape: the blended twin of CalcMeshTransformations: the combination count is read once (down-counting loop), each blend pair's
//   time and alpha are interpolated with the shared inline GFGetInterpolatedValue (FableWin ?GFGetInterpolatedValue@@YIMMMM@Z: the
//   `from` operand kept on the FPU stack, `fsub st(1) ; fadd st(1)`), the animation info is a named local (PeekAnimation is called
//   before GetAnimation's arguments are pushed), and full weight is `fabsf(alpha - 1.0f) < 0.0001f` (`fsub ; fabs ; fcomp`).
// Helper bodies (shared with the twin 0x00A9F540, one text for both rows; FableWin /Od sizes from /opt/fable_lane/tools/fwsize.py):
//   GFFastSin names the two table values and steps the index in two statements (`i -= 256; i &= 0x3ff; float a = g_CosTable[i];
//   float b = g_CosTable[i + 1];`): VC7.1 /Od 35i/120b v FableWin ?GFFastSin@@YIMM@Z 37i/121b, and the same naming on the sibling
//   GFFastCos (landed 0x00A14840) gives 32i/108b v FableWin 34i/109b -- the same +2i/+1b residual on both, and Sin - Cos = 3i/12b in
//   both builds (PROVEN-by-size relative to the sibling); release bytes unchanged (landed out-of-line GFFastSin 0x00A147E0 still
//   RELOCATION_MATCH with this body). This body supplies the IL that puts the expanded BlendWith's q.Y/q.Z/q.W products in retail's
//   order (with the plain one-expression body: DIFFER 2390v2390 at 0x00AA07E8/0x00AA0808/0x00AA0828). BlendWith = a CQuaternion
//   temporary + forced-inline SetSlerp + Interpolate (chunk 3022: the text whose out-of-line compile RELOCATION_MATCHes retail
//   0x00AA3C50; chunk 2044's extra named `from` reference, FableWin 48i, does not), PostApplyRelativeAnimationAlpha with
//   `other_position` named (32i/88b SAME), GFGetInterpolatedValue with a named delta (12i/28b SAME): FITTED among size-equal texts.
//   GFFastArcCos names its table values (chunk 3022, one helper set with 0x00A9EB90 / 0x00A9F540; see the comment above the block).
// Inline-budget: FABLE_FORCEINLINE on the four file-static Apply* helpers -- retail expands every one of them in both rows; written
//   static (or static inline) VC7.1 keeps all four as calls (927 / 16 calls in the blended twin, measured with tools/cc.py), forced
//   they expand to retail's exact size. Their bodies hold the forced asm helper GFFloatToLongNear (through GFFastArcCos / GFFastSin).
// Retail-asm: 0x00AA036F / 0x00AA03DD / 0x00AA0447 and 0x00AA06A0 / 0x00AA070E / 0x00AA0778 `fistp dword ptr [esp+..]` -- the shared
//   engine/GFFloatToLongNear.h helper inside the inline GFFastArcCos / GFFastSin (as landed 0x00AA3C50 BlendWith / 0x00AA3560
//   SetSlerpFromIdentity / 0x00A22530 GFFastArcCos), expanded through SetSlerpFromIdentity (relative-alpha loop) and BlendWith
//   (partial-alpha loop); and the TU's out-of-line ConvertTimeToFrames copy carries engine/GFFloatToLongFloor.h (landed 0x00A52650,
//   retail 0x00A52677 `fistp dword ptr [ebp+8]`). Per-site attempts: the six sites are the same expressions on the same inputs as the
//   twin 0x00A9F540's (arccos 0x00AA036F / 0x00AA06A0 `fmul [512.5] ; fst ; fsub [0.5] ; fstp [esp+1Ch] ; fld ; fistp`, sin
//   0x00AA03DD / 0x00AA0447 / 0x00AA070E / 0x00AA0778 `fmul [1024.0] ; fst ; fsub [0.5] ; fstp ; fld ; fistp`), so the twin's table
//   covers them: NOTES.md "C++ attempts before helper" (probe/asm/tbl: near 38 C++ + 15 intrinsic spellings, floor 32 + 14; all
//   `call __ftol2` / `fistp qword` under /QIfist / SSE converts, never a bare `fistp dword`).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_FREEALL
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DANIMATIONSTATEALPHA
#define FABLE_RETAIL_C3DANIMATIONSTATE_ACCESSORS
#define FABLE_RETAIL_C3DANIMATIONSTATE
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_ACCESSORS
#define FABLE_RETAIL_C3DANIMATIONINFO_GETANIMATION
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
#define FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY
#define FABLE_RETAIL_C3DANIMATION2_MANAGER
#define FABLE_RETAIL_C3DANIMATIONSTATS_ISRELATIVE
#define FABLE_RETAIL_C3DANIMATIONSEQUENCE_FRAMEKEYFRAME
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_ISIDENTITY
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_C3DMESH2_BONECOUNT
#define FABLE_RETAIL_CQUATERNION_SLERP
#define FABLE_RETAIL_CQUATERNION_SLERPFROMIDENTITY
#define FABLE_RETAIL_CQUATERNION_MULASSIGN
#define FABLE_RETAIL_C4DVECTOR_ADDASSIGN
#define FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY
#define FABLE_RETAIL_C3DKEYFRAME_BLEND
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED
#include "engine/C3DMesh2.h"
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimationSequenceData.h"
#include "engine/C3DAnimationSequence.h"
#include "engine/C3DAnimationStats.h"
#include "engine/C3DAnimation2.h"
#include "engine/C3DAnimationStateAlpha.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationManager.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFGetInterpolatedValue.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

// Shared helper set (chunk 3022): this block from GFFastArcCos to BlendWith is textually IDENTICAL in 0x00A9EB90, 0x00A9F540 and
// 0x00AA0090, and each body compiled out of line reproduces its retail copy under VC7.1 (GFFastArcCos 0x00A22530, SetSlerp 0x00A22590,
// CalculateSlerpWeights 0x00AA3390, SetSlerpFromIdentity 0x00AA3560, BlendWith 0x00AA3C50: all RELOCATION_MATCH, probe/users/).
// GFFastArcCos names its table values like GFFastSin (FITTED: FableWin's copy carries asserts); SetSlerp stores directly (FableWin 323b
// SAME); BlendWith = temporary + SetSlerp + Interpolate with no named reference (the only text matching retail's out-of-line 0x00AA3C50).
FABLE_FORCEINLINE float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    float a = g_ArcCosTable[i];
    float b = g_ArcCosTable[i + 1];
    return ((1.0f - frac) * a + frac * b) * 0.15915494f;
}
// FableWin ?GFFastSin@@YIMM@Z 37i/121b (this text 35i/120b at /Od; GFFastCos sibling the same +2i/+1b): named table values.
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    float a = g_CosTable[i];
    float b = g_CosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
}
FABLE_FORCEINLINE void CQuaternion::SetSlerpFromIdentity(const CQuaternion& q, float t)
{
    float cosom = q.W;
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
    X = scale1 * q.X;
    Y = scale1 * q.Y;
    Z = scale1 * q.Z;
    W = scale1 * q.W + scale0;
}
FABLE_FORCEINLINE void C3DKeyframe::PostApplyRelativeAnimationAlpha(const C3DKeyframe& other, float alpha)
{
    CQuaternion rotation;
    rotation.SetSlerpFromIdentity(other.Rotation, alpha);
    Rotation *= rotation;
    const C4DVector& other_position = other.Position;
    Position += other_position * alpha;
}
inline float CQuaternion::Dot(const CQuaternion& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }
FABLE_FORCEINLINE void FABLE_FASTCALL CQuaternion::CalculateSlerpWeights(CQuaternion& weights, const CQuaternion& from, const CQuaternion& to, float t)
{
    float cosom = to.Dot(from);
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        weights.X = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        weights.Y = GFFastSin(omega * t) * (1.0f / sinom);
    }
    else {
        weights.X = 1.0f - t;
        weights.Y = t;
    }
    weights.Y = sign * weights.Y;
}
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

static FABLE_FORCEINLINE void ApplyRelativeAnimation(C3DKeyframe& keyframe, const C3DAnimationSequence& sequence, long frame, float fraction)
{
    if (sequence.IsIdentity())
        return;
    C3DKeyframe relative;
    sequence.GetKeyframeInterpolate(relative, frame, fraction);
    keyframe.PostApplyRelativeAnimation(relative);
}
static FABLE_FORCEINLINE void ApplyRelativeAnimationAlpha(C3DKeyframe& keyframe, const C3DAnimationSequence& sequence, long frame, float fraction, float alpha)
{
    if (sequence.IsIdentity())
        return;
    C3DKeyframe relative;
    sequence.GetKeyframeInterpolate(relative, frame, fraction);
    keyframe.PostApplyRelativeAnimationAlpha(relative, alpha);
}
static FABLE_FORCEINLINE void ApplyPartialAnimation(C3DKeyframe& keyframe, const C3DAnimationSequence& sequence, bool enabled, long frame, float fraction)
{
    if (enabled)
        sequence.GetKeyframeInterpolate(keyframe, frame, fraction);
}
static FABLE_FORCEINLINE void ApplyPartialAnimationAlpha(C3DKeyframe& keyframe, const C3DAnimationSequence& sequence, bool enabled, long frame, float fraction, float alpha)
{
    if (enabled)
    {
        C3DKeyframe partial;
        sequence.GetKeyframeInterpolate(partial, frame, fraction);
        keyframe.BlendWith(partial, alpha);
    }
}

void C3DAnimationManager::CalcBlendedMeshTransformations(const C3DAnimationBlendState& start_blend, const C3DAnimationBlendState& end_blend,
    float blend, const C3DVector* bone_morph, const C3DMesh2& mesh, CMatrix4x4* matrices, unsigned long matrix_count, bool transpose)
{
    unsigned long bone_count = mesh.GetBoneCount();
    __int64 keyframe_count = bone_count * 5;
    if (keyframe_count < 0 || keyframe_count > 0x7fffffff)
        return;
    C3DKeyframe* keyframes = MemoryPool.AllocateType<C3DKeyframe>((unsigned long)keyframe_count);
    CombineBlendedBaseAnimations(start_blend.PeekBaseAnimation(), end_blend.PeekBaseAnimation(), blend, keyframes, bone_count, 0, mesh);
    unsigned long blend_count = start_blend.GetCombinationBlendCount();
    for (unsigned long i = 0; i < blend_count; ++i)
    {
        const C3DAnimationStateAlpha& start_state = start_blend.PeekCombinationBlend(i);
        const C3DAnimationStateAlpha& end_state = end_blend.PeekCombinationBlend(i);
        unsigned long index = start_state.GetAnimationIndex();
        const C3DAnimationInfo* info = MeshBank->PeekAnimation(index);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        long frame;
        float fraction;
        float start_time = start_state.GetAnimationTime();
        float end_time = end_state.GetAnimationTime();
        float time = GFGetInterpolatedValue(start_time, end_time, blend);
        animation->ConvertTimeToFrames(time, frame, fraction);
        float start_alpha = start_state.GetAlphaWeight();
        float end_alpha = end_state.GetAlphaWeight();
        float alpha = GFGetInterpolatedValue(start_alpha, end_alpha, blend);
        bool relative = animation->IsRelative();
        if (relative)
        {
            if (fabsf(alpha - 1.0f) < 0.0001f)
            {
                for (unsigned long bone = 0; bone < bone_count; ++bone)
                    ApplyRelativeAnimation(keyframes[bone], *animation->PeekSequence(bone), frame, fraction);
            }
            else
            {
                for (unsigned long bone = 0; bone < bone_count; ++bone)
                    ApplyRelativeAnimationAlpha(keyframes[bone], *animation->PeekSequence(bone), frame, fraction, alpha);
            }
        }
        else if (animation->IsPartial())
        {
            if (fabsf(alpha - 1.0f) < 0.0001f)
            {
                for (unsigned long bone = 0; bone < bone_count; ++bone)
                    ApplyPartialAnimation(keyframes[bone], *animation->PeekSequence(bone), animation->IsBoneEnabled(bone), frame, fraction);
            }
            else
            {
                for (unsigned long bone = 0; bone < bone_count; ++bone)
                    ApplyPartialAnimationAlpha(keyframes[bone], *animation->PeekSequence(bone), animation->IsBoneEnabled(bone), frame, fraction, alpha);
            }
        }
    }
    if (bone_morph)
        ApplyBoneMorphDisplacement(keyframes, bone_morph, mesh);
    ApplyParentTransformations(keyframes, matrices, mesh, transpose);
    if (bone_morph)
        ApplyBoneMorphScaling(bone_morph, matrices, bone_count, transpose);
    MemoryPool.FreeAll();
}