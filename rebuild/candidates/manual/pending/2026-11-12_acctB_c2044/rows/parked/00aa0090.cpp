// C3DAnimationManager::CalcBlendedMeshTransformations(const C3DAnimationBlendState&, const C3DAnimationBlendState&, float, const C3DVector*, const C3DMesh2&, CMatrix4x4*, unsigned long, bool) -- blend two animation blend states into bone matrices
// Grade: DIFFER(2390v2390) -- 604/622 instructions equal unmasked (all frame offsets and every other x87 order equal): only the q.Y / q.Z / q.W product order of the expanded BlendWith (partial-alpha loop): 0x00AA07E8 / 0x00AA0808 / 0x00AA0828 retail `fld [esp+10h] ; fmul [esi-14h|10h|0Ch]` (scale0 * Rotation) first then `fld [kf] ; fmul st(2)`, ours `fld [esp+0A0h|0A4h|0A8h] ; fmul st(1)` (scale1 * other) first
// Leaf: ?CalcBlendedMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MPBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA0090, length 2390 bytes (`ret 20h`), thiscall. Identity: Ego_r.pdb / FableWin.pdb
//   ?CalcBlendedMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MPBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z
//   (8 arguments; callers 0x00AA0A4A = the CMatrix3x4 overload 0x00AA09F0, 0x00BD2E35).
// Callees: as the twin 0x00A9F540 (src/00a9f540.cpp, RELOCATION_MATCH) with CombineBlendedBaseAnimations 0x00A9F2F0 for the base.
// Shape: the blended twin of CalcMeshTransformations: the combination count is read once (down-counting loop), each blend pair's
//   time and alpha are interpolated with the shared inline GFGetInterpolatedValue (FableWin ?GFGetInterpolatedValue@@YIMMMM@Z: the
//   `from` operand kept on the FPU stack, `fsub st(1) ; fadd st(1)`), the animation info is a named local (PeekAnimation is called
//   before GetAnimation's arguments are pushed), and full weight is `fabsf(alpha - 1.0f) < 0.0001f` (`fsub ; fabs ; fcomp`).
// Measured lever for the residue (diagnostic only, see NOTES.md): the two orders follow the amount of IL the function carries
//   (c2 symbol-record ids, hash16 = rotl16(id,5)): 15..31 dead `float ddN = blend * N.0f;` locals at the top flip the q sums to retail's,
//   and `fabsf` (not the double fabs) puts the dot product in retail's order; fabsf + 16..32 dead float locals = RELOCATION_MATCH (diagnostic).
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
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
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

FABLE_FORCEINLINE float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
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
    Position += other.Position * alpha;
}
inline float CQuaternion::Dot(const CQuaternion& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }
FABLE_FORCEINLINE void C3DKeyframe::BlendWith(const C3DKeyframe& other, float t)
{
    float cosom = other.Rotation.Dot(Rotation);
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
    CQuaternion q;
    q.X = scale1 * other.Rotation.X + scale0 * Rotation.X;
    q.Y = scale1 * other.Rotation.Y + scale0 * Rotation.Y;
    q.Z = scale1 * other.Rotation.Z + scale0 * Rotation.Z;
    q.W = scale0 * Rotation.W + scale1 * other.Rotation.W;
    Rotation = q;
    Position = Position.Interpolate(other.Position, t);
}

static FABLE_FORCEINLINE void ApplyRelativeAnimation(C3DKeyframe& keyframe, const C3DAnimationSequence& sequence, long frame, float fraction)
{
    if (!sequence.IsIdentity())
    {
        C3DKeyframe relative;
        sequence.GetKeyframeInterpolate(relative, frame, fraction);
        keyframe.PostApplyRelativeAnimation(relative);
    }
}
static FABLE_FORCEINLINE void ApplyRelativeAnimationAlpha(C3DKeyframe& keyframe, const C3DAnimationSequence& sequence, long frame, float fraction, float alpha)
{
    if (!sequence.IsIdentity())
    {
        C3DKeyframe relative;
        sequence.GetKeyframeInterpolate(relative, frame, fraction);
        keyframe.PostApplyRelativeAnimationAlpha(relative, alpha);
    }
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
        const C3DAnimationInfo* info = MeshBank->PeekAnimation(start_state.AnimationIndex);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        long frame;
        float fraction;
        animation->ConvertTimeToFrames(GFGetInterpolatedValue(start_state.AnimationTime, end_state.AnimationTime, blend), frame, fraction);
        float alpha = GFGetInterpolatedValue(start_state.GetAlphaWeight(), end_state.GetAlphaWeight(), blend);
        if (animation->IsRelative())
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
