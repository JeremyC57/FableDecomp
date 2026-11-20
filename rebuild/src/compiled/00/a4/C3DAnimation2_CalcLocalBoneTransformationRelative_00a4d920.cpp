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
//   GFastArcCosTable 0x013CB530 / GFastCosTable 0x013CD550 (Ego_d.pdb publics ?GFastArcCosTable@@3PAMA / ?GFastCosTable@@3PAMA).
// Retail-asm: 0x00A4DA78 / 0x00A4DAE9 `fistp dword ptr [esp+1Ch]`, 0x00A4DB59 `fistp dword ptr [esp+18h]` -- engine/GFFloatToLongNear.h inside
//   GFFastArcCos / GFFastSin (bare `fistp dword`: `(long)x` gives __ftol2, /QIfist gives `fistp qword`; the landed
//   C3DKeyframe::BlendWith 0x00AA3C50 and CQuaternion::CalculateSlerpWeights 0x00AA3390 reach the same helper). GFFloatToLongFloor
//   (engine/GFFloatToLongFloor.h) is reached only through the out-of-line ConvertTimeToFrames. This row carries no asm of its own.
// Helper texts (re-port, audit 2026-11-20, with row 0x00A4D770): GFFastArcCos, GFFastSin, CQuaternion::Dot / SetIdentity / SetSlerp and
//   C3DAnimationSequenceData::InlineGetKeyframeInterpolate are now the Ego_d-shaped block of the landed 0x00A53860 / 0x00A52760 /
//   0x00A4C1F0 (line tables and locals in 0x00A53860's header), in place of the October one-expression texts on the made-up table
//   names g_ArcCosTable / g_CosTable. One text per helper across the five rows.
// Inferred: `goto rotation_done;` for the equal-palette-entry copy (Ego_d's line 56 is a 2-byte jump out of the slerp; a `break` out
//   of a one-pass loop would be the other 2-byte shape, not graded).
// Inferred: two helper lines fitted by row 0x00A4D770 C3DAnimation2::CalcLocalTransformationsRelative (measurements in that row's header)
//   and carried here so that the family keeps one text per helper: SetSlerp's weight argument is `w2` (Ego_d local, line 41), and
//   GFFastArcCos has `float pos = (float)((value + 1.0f) * 512.5f);` (Ego_d line 235 is 24 bytes, the uncast statement 18 at /Od).
//   Both are byte-neutral in this row: RELOCATION_MATCH with and without them (re-port, audit 2026-11-20).
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
#define FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCALBONE
#define FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_CQUATERNION_SLERP
#define FABLE_RETAIL_CQUATERNION_MULASSIGN
#define FABLE_RETAIL_C4DVECTOR_ASSIGN3D
#define FABLE_RETAIL_C3DKEYFRAME_SETPOSITION
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimation2.h"
#include "engine/C3DMesh2.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFloatToLongFloor.h"
extern float GFastArcCosTable[1026];    // 0x013CB530 ?GFastArcCosTable@@3PAMA (acos(k * 2 / 1025 - 1), k = 0..1025)
extern float GFastCosTable[1025];       // 0x013CD550 ?GFastCosTable@@3PAMA (cos(2 * pi * k / 1024), k = 0..1024)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
FABLE_FORCEINLINE float __fastcall GFFastArcCos(float value)
{
    float pos = (float)((value + 1.0f) * 512.5f);
    long index = GFFloatToLongNear(pos - 0.5f);
    float interp = pos - index;
    float result = (1.0f - interp) * GFastArcCosTable[index] + interp * GFastArcCosTable[index + 1];
    result *= 0.15915494f;
    return result;
}
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    float pos = angle * 1024.0f;
    long index = GFFloatToLongNear(pos - 0.5f);
    float interp = pos - index;
    index -= 256;
    index &= 1023;
    float result = (1.0f - interp) * GFastCosTable[index] + interp * GFastCosTable[index + 1];
    return result;
}
inline float CQuaternion::Dot(const CQuaternion& q2) const
{
    return X * q2.X + Y * q2.Y + Z * q2.Z + W * q2.W;
}
inline void CQuaternion::SetIdentity()
{
    X = 0.0f; Y = 0.0f; Z = 0.0f; W = 1.0f;
}
FABLE_FORCEINLINE void CQuaternion::SetSlerp(const CQuaternion& q1, const CQuaternion& q2, float weight2)
{
    float scale1;
    float scale2;
    float cosa = q1.Dot(q2);
    float sign = 1.0f;
    if (cosa < 0.0f)
    {
        cosa = -cosa;
        sign = -1.0f;
    }
    if (1.0f - cosa > 0.05f)
    {
        float angle = GFFastArcCos(cosa);
        float sina = (float)sqrt(1.0f - cosa * cosa);
        scale1 = GFFastSin((1.0f - weight2) * angle) / sina;
        scale2 = GFFastSin(weight2 * angle) / sina;
    }
    else
    {
        scale1 = 1.0f - weight2;
        scale2 = weight2;
    }
    scale2 *= sign;
    X = scale1 * q1.X + scale2 * q2.X;
    Y = scale1 * q1.Y + scale2 * q2.Y;
    Z = scale1 * q1.Z + scale2 * q2.Z;
    W = scale1 * q1.W + scale2 * q2.W;
}

FABLE_FORCEINLINE void C3DAnimationSequenceData::InlineGetKeyframeInterpolate(C3DKeyframe& dest, unsigned long frame, float fraction) const
{
    unsigned long next_frame = (frame + 1 < FrameCount) ? frame + 1 : 0;
    float w1 = 1.0f - fraction;
    float w2 = fraction;
    if (RotationTrackMode & 2)
    {
        unsigned long index = frame;
        unsigned long next_index = next_frame;
        if (RotationTrackMode == 3)
        {
            index = PalettedRotationTrack[frame];
            next_index = PalettedRotationTrack[next_frame];
            if (index == next_index)
            {
                dest.Rotation = RotationTrack[index];
                goto rotation_done;
            }
        }
        dest.Rotation.SetSlerp(RotationTrack[index], RotationTrack[next_index], w2);
    }
    else if (RotationTrackMode == 1)
        dest.Rotation = RotationTrack[0];
    else
        dest.Rotation.SetIdentity();
rotation_done:
    if (PositionTrackMode & 2)
    {
        unsigned long index = frame;
        unsigned long next_index = next_frame;
        if (PositionTrackMode == 3)
        {
            index = PalettedPositionTrack[frame];
            next_index = PalettedPositionTrack[next_frame];
        }
        const C3DVectorWord& position = PositionTrack[index];
        const C3DVectorWord& next_position = PositionTrack[next_index];
        dest.SetPosition(position * (w1 * PositionFactor) + next_position * (w2 * PositionFactor));
    }
    else if (PositionTrackMode == 1)
        dest.SetPosition(PositionTrack[0] * PositionFactor);
    else
        dest.SetPosition(C3DVector(0.0f, 0.0f, 0.0f));
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