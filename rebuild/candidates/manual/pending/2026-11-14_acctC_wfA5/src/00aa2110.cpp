// C3DAnimationManager::CalcBlendedBoneTransformation(const C3DAnimationBlendState&, const C3DAnimationBlendState&, float, const C3DMesh2&, unsigned long, CMatrix3x4&, C3DVector&) -- one bone's transform between two blend states: collect the bone and its parent chain, combine the two base animations blended for that chain, apply each combination blend at the interpolated time and weight, apply the start state's skeletal morph scaling (the bone's own scaling returned), then concatenate the chain's keyframe matrices into the output matrix
// Leaf: ?CalcBlendedBoneTransformation@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MABVC3DMesh2@@KAAVCMatrix3x4@@AAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA2110, 4556 bytes (0x00AA2110..0x00AA32D9 `ret 1Ch`; next function 0x00AA32E0 area), thiscall,
//   16-byte realigned frame (`and esp,-16`: the `__declspec(align(16))` CMatrix4x4 locals), no EH frame, no flags.
// Identity: Ego_r.pdb ?CalcBlendedBoneTransformation@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MABVC3DMesh2@@KAAVCMatrix3x4@@
//   AAVC3DVector@@@Z (0x00BE6570, right after CalcBoneTransformation 0x00BE5220 = retail 0x00AA0FA0 -> 0x00AA2110) and FableWin.pdb (same
//   name). Seven stack arguments = `ret 1Ch`. Parameter names are descriptive (follow the landed blended twin 0x00AA0090).
// Callees: vector<unsigned long>::resize(size_type, const unsigned long&) 0x00464931 (out of line: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE),
//   _M_insert_overflow 0x00464ACE (push_back), CSimpleMemoryPool::Allocate 0x00A5C910 / FreeAll 0x00A5C720, CombineBlendedBaseAnimationsBoneList
//   0x00AA0DC0, CMeshDataBank::PeekAnimation 0x00A242C0, C3DAnimationInfo::GetAnimation 0x00A26C60, C3DAnimation2::ConvertTimeToFrames
//   0x00A52650, C3DAnimationSequenceData::GetKeyframeInterpolate 0x00A4C1F0, CQuaternion::operator*= 0x00A88C10, C3DSkeletalMorph::
//   PeekCombinedMorph 0x00A45260, C3DSkeletalMorphTarget::GetBoneScaling 0x00A44CD0, operator delete 0x00BFE9BC.
// Shape: the bone-chain twin of the landed CalcBlendedMeshTransformations 0x00AA0090 (its helper block copied verbatim, one text for the
//   four rows) with CalcBoneTransformation 0x00AA0FA0's chain collection (resize(1), push_back of each parent), the per-chain loops indexed
//   through BoneIndicesBuffer (a named `bone_index` before the keyframe reference in the partial loops), the start state's skeletal morph
//   scaling (SetPosition / GetScaled, unrolled by VC7.1) and a chain concatenation into two aligned matrices swapped with Lionhead's
//   SWAP template before each ToMatrix; the parent matrix is ONE function-scope aligned local (inside the loop its stores schedule early).
// Spelling: the first chain matrix goes straight into matrix_a (`keyframes[0].ToMatrix(matrix_a, false)`; the twin 0x00AA0FA0 writes
//   `*current`): FITTED -- same code, but it alone moves the BlendWith dot / slerp and the x87 AlignedSetMultiply first-column operand
//   orders (c2 record-id hash keys) to retail's under the twins' shared SetPosition text (a manual three-line pointer swap instead of
//   SWAP also matches; SWAP kept, FableWin ??$SWAP@PAVCMatrix4x4@@@@YIXAAPAVCMatrix4x4@@0@Z).
// Inline-budget: as the twins: FABLE_FORCEINLINE Apply* helpers (file-static, retail expands all four), forced CQuaternion::ToMatrix /
//   C3DKeyframe::ToMatrix 4x4 (FABLE_RETAIL_CQUATERNION_MATRIX4_FORCEINLINE / C3DKEYFRAME_TOMATRIX4_INLINE, as 0x00A9E1E0).
// Retail-asm: 0x00AA247F / 0x00AA24F6 / 0x00AA2569 and 0x00AA27E3 / 0x00AA2851 / 0x00AA28BB `fistp dword ptr [esp+..]` -- the shared
//   engine/GFFloatToLongNear.h helper inside the inline GFFastArcCos / GFFastSin, expanded through SetSlerpFromIdentity (relative-alpha
//   loop) and BlendWith (partial-alpha loop): the same expressions on the same inputs as the landed twins 0x00A9F540 / 0x00AA0090, whose
//   per-site table covers them (near 38 C++ + 15 intrinsic spellings: `call __ftol2` / `fistp qword` / SSE converts, never `fistp dword`);
//   and 0x00AA30C8 `movaps xmm0,[edx]` .. `movaps [edi+30h],xmm4` (pointers re-read from the slots just stored) -- the shared
//   engine/CMatrix4x4AlignedSSE.h GF_SSE_MATRIX_MULTIPLY kernel inside the inline CMatrix4x4::AlignedSetMultiply, the same site kind as
//   the landed 0x00A9E1E0 (its per-site table: 35 plain C++ + 13 intrinsic spellings, none re-reads the homed pointers).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DANIMATIONMANAGER_BONETRANSFORM
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SKELETALMORPH
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR_GETSCALED
#define FABLE_RETAIL_C3DKEYFRAME_SETPOSITION
#define FABLE_RETAIL_C3DBONE2_GETNAMECRC
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#define FABLE_RETAIL_CMATRIX4X4_COPYTO
#define FABLE_RETAIL_CMATRIX3X4_TRANSPOSE
#define FABLE_RETAIL_CQUATERNION_MATRIX4
#define FABLE_RETAIL_CQUATERNION_MATRIX4_FORCEINLINE
#define FABLE_RETAIL_CQUATERNION_MATRIX4_TRANSPOSED
#define FABLE_RETAIL_C3DKEYFRAME_TOMATRIX4_INLINE
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_C3DBONE2_PEEK
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
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DSkeletalMorph.h"
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

void C3DAnimationManager::CalcBlendedBoneTransformation(const C3DAnimationBlendState& start_blend, const C3DAnimationBlendState& end_blend,
    float blend, const C3DMesh2& mesh, unsigned long bone, CMatrix3x4& transform, C3DVector& scale)
{
    BoneIndicesBuffer.resize(1);
    BoneIndicesBuffer[0] = bone;
    long parent = mesh.GetBone(bone)->GetParent();
    while (parent >= 0)
    {
        BoneIndicesBuffer.push_back(parent);
        parent = mesh.GetBone(parent)->GetParent();
    }
    unsigned long count = BoneIndicesBuffer.size();
    __int64 keyframe_count = count * 5;
    if (keyframe_count < 0 || keyframe_count > 0x7fffffff)
        return;
    C3DKeyframe* keyframes = MemoryPool.AllocateType<C3DKeyframe>((unsigned long)keyframe_count);
    CombineBlendedBaseAnimationsBoneList(start_blend.PeekBaseAnimation(), end_blend.PeekBaseAnimation(), blend, keyframes, count, 0, mesh,
        BoneIndicesBuffer);
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
                for (unsigned long j = 0; j < count; ++j)
                {
                    C3DKeyframe& keyframe = keyframes[j];
                    ApplyRelativeAnimation(keyframe, *animation->PeekSequence(BoneIndicesBuffer[j]), frame, fraction);
                }
            }
            else
            {
                for (unsigned long j = 0; j < count; ++j)
                {
                    C3DKeyframe& keyframe = keyframes[j];
                    ApplyRelativeAnimationAlpha(keyframe, *animation->PeekSequence(BoneIndicesBuffer[j]), frame, fraction, alpha);
                }
            }
        }
        else if (animation->IsPartial())
        {
            if (fabsf(alpha - 1.0f) < 0.0001f)
            {
                for (unsigned long j = 0; j < count; ++j)
                {
                    unsigned long bone_index = BoneIndicesBuffer[j];
                    C3DKeyframe& keyframe = keyframes[j];
                    ApplyPartialAnimation(keyframe, *animation->PeekSequence(bone_index), animation->IsBoneEnabled(bone_index), frame, fraction);
                }
            }
            else
            {
                for (unsigned long j = 0; j < count; ++j)
                {
                    unsigned long bone_index = BoneIndicesBuffer[j];
                    C3DKeyframe& keyframe = keyframes[j];
                    ApplyPartialAnimationAlpha(keyframe, *animation->PeekSequence(bone_index), animation->IsBoneEnabled(bone_index), frame, fraction, alpha);
                }
            }
        }
    }
    if (start_blend.HasSkeletalMorph())
    {
        C3DVector* scales = MemoryPool.AllocateType<C3DVector>(count);
        CCountedPointer<const C3DSkeletalMorphTarget> morph = start_blend.PeekSkeletalMorph().PeekCombinedMorph();
        for (unsigned long j = 0; j < count; ++j)
            morph->GetBoneScaling(scales[j], mesh.GetBone(BoneIndicesBuffer[j])->GetNameCRC());
        for (unsigned long j = 1; j < count; ++j)
        {
            C3DKeyframe& keyframe = keyframes[j - 1];
            keyframe.SetPosition(keyframe.GetPosition().GetScaled(scales[j]));
        }
        scale = scales[0];
    }
    else
        scale.Initialise(1.0f, 1.0f, 1.0f);
    __declspec(align(16)) CMatrix4x4 matrix_a;
    __declspec(align(16)) CMatrix4x4 matrix_b;
    CMatrix4x4* current = &matrix_a;
    CMatrix4x4* other = &matrix_b;
    keyframes[0].ToMatrix(matrix_a, false);
    __declspec(align(16)) CMatrix4x4 parent_matrix;
    for (unsigned long j = 1; j < count; ++j)
    {
        SWAP(current, other);
        keyframes[j].ToMatrix(parent_matrix, false);
        current->AlignedSetMultiply(*other, parent_matrix);
    }
    current->CopyTo(transform);
    MemoryPool.FreeAll();
}
