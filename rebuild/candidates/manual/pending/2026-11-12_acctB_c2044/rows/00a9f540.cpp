// C3DAnimationManager::CalcMeshTransformations(const C3DAnimationBlendState&, const C3DVector*, const C3DMesh2&, CMatrix4x4*, unsigned long, bool) -- evaluate an animation blend state into bone matrices
// Leaf: ?CalcMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@PBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9F540, length 2288 bytes (`ret 18h`), thiscall. Identity: Ego_r.pdb / FableWin.pdb
//   ?CalcMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@PBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z (the
//   only 6-argument CMatrix4x4 overload; its callers 0x00A9FE80 (the CMatrix3x4 overload), 0x00AE136B / 0x00AE2A28 (CParticleProgram)).
// Callees: CSimpleMemoryPool::Allocate 0x00A5C910 / FreeAll 0x00A5C720 (MemoryPool +0x14), CombineBaseAnimations 0x00A9F1D0,
//   CMeshDataBank::PeekAnimation 0x00A242C0, C3DAnimationInfo::GetAnimation 0x00A26C60 (hidden CCountedPointer result),
//   C3DAnimation2::ConvertTimeToFrames 0x00A52650, C3DAnimationSequenceData::GetKeyframeInterpolate 0x00A4C1F0,
//   CQuaternion::operator*= 0x00A88C10, ApplyBoneMorphDisplacement 0x00A9DFA0, ApplyParentTransformations 0x00A9E1E0,
//   ApplyBoneMorphScaling 0x00A9D750, operator delete 0x00BFE9BC (the counted pointer's info block).
// Shape: the keyframe scratch holds bone_count * 5 keyframes (CombineBaseAnimations recurses into `out + count` up to depth 5),
//   behind a 64-bit range check that returns at once (retail `xor eax,eax ; cmp eax,esi ; jl ...`, 0x00A9F55B..0x00A9F58A) and
//   the inline AllocateType 64-bit size check. Each combination blend (count re-read every pass) is sampled through the bank's
//   animation, then applied per bone by four file-static helpers that FableWin.pdb lists right after this function in exactly
//   this order (ApplyRelativeAnimation, ApplyRelativeAnimationAlpha, ApplyPartialAnimation, ApplyPartialAnimationAlpha, with the
//   inline IsIdentity / PostApplyRelativeAnimationAlpha / BlendWith they use); retail keeps uncalled out-of-line copies of three
//   of them with LTCG custom calling conventions (0x00A9D980, 0x00A9D9E0, 0x00A9DC20) and expands all four here. Relative animations
//   (C3DAnimationStats::IsRelative, AnimType bit 2) post-multiply, partial ones (IsPartial, bit 1) overwrite or BlendWith the
//   enabled bones; `alpha == 1.0f` picks the full-weight loops (retail `fucompp ; test ah,44h`). ConvertTimeToFrames' body must be
//   visible in the TU (it holds the asm floor helper, so it stays a call): with only a declaration frame / fraction escape and
//   are reloaded in every bone loop (2249 bytes); retail hoists them into ebx / ebp.
// Layout: C3DAnimationManager MeshBank +4, MemoryPool +0x14 (hdr/engine/C3DAnimationManager.h); C3DAnimationBlendState::CSharedData
//   BaseAnimation +8 / CombinationBlends +0x0C (20-byte C3DAnimationStateAlpha: AnimationIndex +8, AnimationTime +0xC,
//   AlphaWeight +0x10); C3DAnimation2 AnimType +8 (C3DAnimationStats), Sequences +0x5C (8 bytes each), BoneMask +0x68;
//   C3DAnimationSequenceData IdentityFlag = bit 7 of +0x0A; C3DMesh2 BoneCount +0x98; C3DKeyframe 0x30 bytes.
// Inline-budget: FABLE_FORCEINLINE on the four file-static Apply* helpers -- retail expands every one of them in both rows; written
//   static (or static inline) VC7.1 keeps all four as calls (927 / 16 calls in the blended twin, measured with tools/cc.py), forced
//   they expand to retail's exact size. Their bodies hold the forced asm helper GFFloatToLongNear (through GFFastArcCos / GFFastSin).
// Retail-asm: 0x00A9F7CF / 0x00A9F83D / 0x00A9F8A7 and 0x00A9FAF0 / 0x00A9FB5E / 0x00A9FBC8 `fistp dword ptr [esp+..]` -- the shared
//   engine/GFFloatToLongNear.h helper inside the inline GFFastArcCos / GFFastSin (as landed 0x00AA3C50 BlendWith / 0x00AA3560
//   SetSlerpFromIdentity / 0x00A22530 GFFastArcCos), expanded through SetSlerpFromIdentity (relative-alpha loop) and BlendWith
//   (partial-alpha loop); and the TU's out-of-line ConvertTimeToFrames copy carries engine/GFFloatToLongFloor.h (landed 0x00A52650,
//   retail 0x00A52677 `fistp dword ptr [ebp+8]` in `fld ; fsub [half] ; fistp ; fild ; fadd [one] ; fstp ; mov eax ; cmp ; jne ; inc`).
//   Per-site attempts (fragment TU probe/asm/tbl/spell.cpp from gen.py, each spelling fed the site's own input: y = (cosom + 1) *
//   512.5f for the arccos sites 0x00A9F7CF / 0x00A9FAF0, y = (1 - t) * omega * 1024 for 0x00A9F83D / 0x00A9FB5E and omega * t * 1024
//   for 0x00A9F8A7 / 0x00A9FBC8, the six being the same expression on the same inputs; frames = time * SamplesPerSecond for the floor
//   site; cl 13.10.3077 /O2 /Oy /W3, and + /QIfist, /arch:SSE2, /arch:SSE, /Op, /fp:fast; script probe/asm/tbl/run.py; full table
//   in NOTES.md "C++ attempts before helper"): Near sites 38 C++ spellings ((long) / (int) / static_cast / functional casts of
//   y - 0.5f, (long)y, via double / long double / __int64 / unsigned / short, floor / floorf / std::floor / ceil / fmod / fmodf /
//   modf / _copysign, implicit and direct-init conversion, inline ToLong wrappers, sign-split rounding, a homed float through a qualified local,
//   double / float magic-number unions and puns), Floor site 32 (the floor / ceil / fmod / modf families, trunc + compare fix-ups,
//   retail's own -0.5 / +1 / bit-compare algorithm written in C++, magic unions, ...): every one `call __ftol2` (plus the library
//   call) under the row's flags, `fistp qword` under /QIfist, cvttss2si or a `fnstcw ; fldcw ; fistp dword ; fldcw` truncation
//   wrapper under /arch:SSE2, magic-number adds for the bit tricks -- never a bare `fistp dword`; then 15 / 14 intrinsic spellings
//   (_mm_cvtss_si32, _mm_cvt_ss2si, _mm_cvttss_si32, _mm_cvtt_ss2si, _mm_cvtsd_si32, _mm_cvttsd_si32, _mm_cvtps_pi32,
//   _mm_cvttps_pi32, _mm_cvtps_epi32, _mm_cvttps_epi32, _mm_cvtpd_epi32, a biased form, _MM_SET_ROUNDING_MODE + cvtss2si, and
//   _controlfp / _control87 around a cast) emit SSE converts (cvtss2si / cvttss2si / cvtsd2si / cvttsd2si / cvtps2pi / cvttps2pi /
//   cvtps2dq / cvttps2dq / cvtpd2dq, + stmxcsr / ldmxcsr) or CRT calls + __ftol2. So the shared helpers stay (ASM_RULES 3.2).
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

void C3DAnimationManager::CalcMeshTransformations(const C3DAnimationBlendState& blend_state, const C3DVector* bone_morph, const C3DMesh2& mesh,
    CMatrix4x4* matrices, unsigned long matrix_count, bool transpose)
{
    unsigned long bone_count = mesh.GetBoneCount();
    __int64 keyframe_count = bone_count * 5;
    if (keyframe_count < 0 || keyframe_count > 0x7fffffff)
        return;
    C3DKeyframe* keyframes = MemoryPool.AllocateType<C3DKeyframe>((unsigned long)keyframe_count);
    CombineBaseAnimations(blend_state.PeekBaseAnimation(), keyframes, bone_count, 0, mesh);
    for (unsigned long i = 0; i < blend_state.GetCombinationBlendCount(); ++i)
    {
        const C3DAnimationStateAlpha& state = blend_state.PeekCombinationBlend(i);
        const C3DAnimationInfo* info = MeshBank->PeekAnimation(state.AnimationIndex);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        long frame;
        float fraction;
        animation->ConvertTimeToFrames(state.AnimationTime, frame, fraction);
        float alpha = state.GetAlphaWeight();
        if (animation->IsRelative())
        {
            if (alpha == 1.0f)
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
            if (alpha == 1.0f)
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
