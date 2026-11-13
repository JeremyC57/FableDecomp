// C3DAnimation2::CalcLocalBoneTransformationRelative(C3DKeyframe& keyframe, unsigned long bone, float time, const C3DMesh2& mesh) const -- the bone's local keyframe at `time` (CalcLocalBoneTransformation: slerped rotation, interpolated position, rigid-bone override) applied on top of the mesh's rest keyframe (C3DKeyframe::PostApplyRelativeAnimation)
// Leaf: ?CalcLocalBoneTransformationRelative@C3DAnimation2@@QBEXAAVC3DKeyframe@@KMABVC3DMesh2@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4D920, 1341 bytes, thiscall, `ret 10h`.
// Identity: Ego_r.pdb / FableWin.pdb ?CalcLocalBoneTransformationRelative@C3DAnimation2@@QBEXAAVC3DKeyframe@@KMABVC3DMesh2@@@Z (both
//   name sets; `ret 10h` = keyframe, bone, time, mesh).
// Shape: the Relative form is CalcLocalBoneTransformation(local, bone, time) (Ego_r.pdb / FableWin.pdb
//   ?CalcLocalBoneTransformation@C3DAnimation2@@QBEXAAVC3DKeyframe@@KM@Z, out of line at 0x00A4D330 in the same TU, `ret 0Ch`, frame
//   0x38) expanded into a local C3DKeyframe (frame 0x38 + 0x30 = this row's 0x68; 0x00A4D920..0x00A4DE1F is 0x00A4D330's body
//   with the keyframe in the local), then the mesh's rest keyframe copied out and FableWin.pdb
//   ?PostApplyRelativeAnimation@C3DKeyframe@@QAEXABV1@@Z (rotation *= local rotation through the out-of-line CQuaternion::operator*=
//   0x00A88C10, position += local position) on it.
// Inline: CalcLocalBoneTransformation is FABLE_FORCEINLINE in this TU. ASM_RULES rule 3.3: VC7.1 never auto-inlines a function that
//   contains inline asm, and its body reaches the GFFloatToLongNear asm helper (through InlineGetKeyframeInterpolate -> SetSlerp ->
//   GFFastArcCos / GFFastSin). Plain and `inline` definitions stay called at /d2inlT100..800 (DIFFER 118v1341); the forced expansion
//   is retail's (RELOCATION_MATCH). The same reason keeps ConvertTimeToFrames (GFFloatToLongFloor asm) called at 0x00A4D93D.
//   VC7.1 still emits the referenced inline as a COMDAT, which is the out-of-line 0x00A4D330 other TUs call
//   (C3DAnimationManager::CombineBaseAnimationsBoneList 0x00AA0C50).
// Callees: C3DAnimation2::ConvertTimeToFrames 0x00A52650 (called), CQuaternion::operator*= 0x00A88C10 (called); globals
//   g_ArcCosTable 0x013CB530 / g_CosTable 0x013CD550 (bound spellings; no PDB data symbols on this machine).
// Retail-asm: 0x00A4DA78 / 0x00A4DAE9 `fistp dword ptr [esp+1Ch]`, 0x00A4DB59 `fistp dword ptr [esp+18h]` -- engine/GFFloatToLongNear.h inside
//   GFFastArcCos / GFFastSin (bare `fistp dword`: `(long)x` gives __ftol2, /QIfist gives `fistp qword`; the landed
//   C3DKeyframe::BlendWith 0x00AA3C50 and CQuaternion::CalculateSlerpWeights 0x00AA3390 reach the same helper). GFFloatToLongFloor
//   (engine/GFFloatToLongFloor.h) is reached only through the out-of-line ConvertTimeToFrames. This row carries no asm of its own.
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCALBONE
#define FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_CQUATERNION_SLERP
#define FABLE_RETAIL_CQUATERNION_MULASSIGN
#define FABLE_RETAIL_C4DVECTOR_ASSIGN3D
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimation2.h"
#include "engine/C3DMesh2.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFloatToLongFloor.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
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
inline float CQuaternion::Dot(const CQuaternion& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }
FABLE_FORCEINLINE void CQuaternion::SetSlerp(const CQuaternion& from, const CQuaternion& to, float t)
{
    float cosom = to.Dot(from);
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

FABLE_FORCEINLINE void C3DAnimationSequenceData::InlineGetKeyframeInterpolate(C3DKeyframe& keyframe, unsigned long frame, float fraction) const
{
    unsigned long nextFrame = (frame + 1 < FrameCount) ? frame + 1 : 0;
    float oneMinusFraction = 1.0f - fraction;
    if (RotationTrackMode & 2) {
        unsigned long index0 = frame;
        unsigned long index1 = nextFrame;
        if (RotationTrackMode == 3) {
            index0 = PalettedRotationTrack[frame];
            index1 = PalettedRotationTrack[nextFrame];
            if (index0 == index1) {
                keyframe.Rotation = RotationTrack[index0];
                goto rotation_done;
            }
        }
        keyframe.Rotation.SetSlerp(RotationTrack[index0], RotationTrack[index1], fraction);
    } else if (RotationTrackMode == 1) {
        keyframe.Rotation = RotationTrack[0];
    } else {
        keyframe.Rotation.X = 0.0f;
        keyframe.Rotation.Y = 0.0f;
        keyframe.Rotation.Z = 0.0f;
        keyframe.Rotation.W = 1.0f;
    }
rotation_done:
    if (PositionTrackMode & 2) {
        unsigned long index0 = frame;
        unsigned long index1 = nextFrame;
        if (PositionTrackMode == 3) {
            index0 = PalettedPositionTrack[frame];
            index1 = PalettedPositionTrack[nextFrame];
        }
        const C3DVectorWord& position0 = PositionTrack[index0];
        const C3DVectorWord& position1 = PositionTrack[index1];
        keyframe.Position = position0 * (oneMinusFraction * PositionFactor) + position1 * (fraction * PositionFactor);
    } else if (PositionTrackMode == 1) {
        keyframe.Position = PositionTrack[0] * PositionFactor;
    } else {
        keyframe.Position = C3DVector(0.0f, 0.0f, 0.0f);
    }
}

inline void C3DAnimation2::ConvertTimeToFrames(float time, long& frame, float& fraction) const
{
    float frames = time * SamplesPerSecond;
    long whole = GFFloatToLongFloor(frames);
    frame = whole;
    fraction = frames - whole;
    frame = (unsigned long)frame % FrameCount;
    frame = frame < 0 ? frame + FrameCount : frame;
}

FABLE_FORCEINLINE void C3DAnimation2::CalcLocalBoneTransformation(C3DKeyframe& keyframe, unsigned long bone, float time) const
{
    long frame;
    float fraction;
    ConvertTimeToFrames(time, frame, fraction);
    const C3DAnimationSequence& sequence = Sequences[bone];
    sequence.InlineGetKeyframeInterpolate(keyframe, frame, fraction);
    if (RigidBoneMask && RigidBoneMask[bone])
        keyframe.Position = RigidBonePositions[bone];
}

void C3DAnimation2::CalcLocalBoneTransformationRelative(C3DKeyframe& keyframe, unsigned long bone, float time, const C3DMesh2& mesh) const
{
    C3DKeyframe local;
    CalcLocalBoneTransformation(local, bone, time);
    keyframe = mesh.BoneKeyframes[bone];
    keyframe.PostApplyRelativeAnimation(local);
}