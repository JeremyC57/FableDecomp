// C3DAnimation2::CalcLocalTransformationsRelative(C3DKeyframe* dest, unsigned long dest_size, float t, const C3DMesh2& mesh) const -- for every sequence, interpolate the animation keyframe at `t` into a local, then copy the mesh's rest keyframe of that bone to dest
// Leaf: ?CalcLocalTransformationsRelative@C3DAnimation2@@QBEXPAVC3DKeyframe@@KMABVC3DMesh2@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4D770, 420 bytes, thiscall, `ret 10h`.
// Retail-asm: 0x00A4D859 `fistp dword ptr [esp+14h]`, 0x00A4D8AF `fistp dword ptr [esp+24h]`, 0x00A4D8CF `fistp dword ptr [esp+28h]` --
//   the shared engine/GFFloatToLongNear.h inside GFFastArcCos / GFFastSin (as in the landed 0x00A4D920 / 0x00A52760). This row
//   carries no asm of its own.
// Identity: Ego_r.pdb / Ego_d.pdb / FableWin.pdb ?CalcLocalTransformationsRelative@C3DAnimation2@@QBEXPAVC3DKeyframe@@KMABVC3DMesh2@@@Z
//   (caller C3DAnimationManager::CombineBaseAnimations 0x00A9F1D0). Parameters and locals exactly as Ego_d.pdb (debug length 461):
//   dest, dest_size, t, mesh; frame, fraction; { c_seq { kf_rel } }.
// Shape: Ego_d.pdb's C13 line table (ref/00a4d770/egod_lines.tsv, source lines 612..625; VC10 /Od: +3 per float argument):
//   615 ConvertTimeToFrames (23 = our 20 + 3); 617 a debug-only assert (279 bytes; dest_size is read nowhere else); 618 the loop
//   (35 v our 29); 620 kf_rel (8 = ours, the implicit constructor call); 621 GetKeyframeInterpolateFixPos (27 = our 24 + 3);
//   622 `dest[c_seq] = *mesh.GetBoneKeyframe(c_seq);` (30 = ours); 624 the loop's back jump (2 = ours).
//   kf_rel is never read, so its stores are dead: what survives of the interpolation is the slerp weight work up to the three
//   `fistp`s (asm, kept) and the paletted-index compare that guards it.
// Helper texts (one text per function, shared with the landed 0x00A52760 / 0x00A53860 / 0x00A4D920 family): GFFastArcCos, GFFastSin,
//   CQuaternion::Dot / SetIdentity / SetSlerp and C3DAnimationSequenceData::InlineGetKeyframeInterpolate are the landed 0x00A53860
//   texts (Ego_d line tables and locals, see that row) with the two changes below; ConvertTimeToFrames is the shared header's
//   body (engine/C3DAnimation2.h, FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY: Ego_d has it in a header, file id 1224; called, not
//   expanded -- it holds the GFFloatToLongFloor asm -- and the COMDAT this object emits equals retail 0x00A52650, 117 / 117 B); GetKeyframeInterpolateFixPos is Ego_d's (lines 145..152:
//   the forward 32 bytes, the rigid-bone test 26, `dest.SetPosition(RigidBonePositions[bone_index]);` 21 = a call with dest as
//   `this`; private, FableWin 0x03227270, no retail copy: FABLE_FORCEINLINE because its body reaches the asm helper).
// What decided the last instruction (0x00A4D8F1 `lea edi,[edx+ebx]`, dest + IV; the October text gave `[ebx+edx]` for 42 grades):
//   c2's tree optimiser sorts the two addends by a 16-bit key, descending (canonicaliser 0x1075CC68; measured with the hand3c3 IL
//   kit, tools/il): dest is frame symbol #5 = key 00A0; the strength-reduced index is a temporary, key = its symbol number << 6
//   mod 2^16. Retail's order needs the temporary's key below 00A0, i.e. its number = 1024, 1025 or 1026 (mod 1024). The number is
//   the count of symbols c2 has handed out when it makes the index, so it depends on every helper expanded before the loop tail:
//   the October helper texts gave #1021, the landed 0x00A53860 texts #1022, and each of the two changes below adds one -> #1024.
// Inferred: SetSlerp's weight argument is `w2`, not `fraction` (InlineGetKeyframeInterpolate). Ego_d has the local (line 41 `w2 =
//   fraction`, 6 bytes) and line 59 is 49 bytes with either operand; SetSlerp's parameter is named weight2. Measured on this row:
//   `fraction` = temporary #1023 with the cast below (420v420, the lea), `w2` = #1024 (match).
// Inferred: `float pos = (float)((value + 1.0f) * 512.5f);` in GFFastArcCos. Ego_d line 235 is 24 bytes; the uncast statement is
//   18 at /Od (GFFastSin's `pos = angle * 1024.0f` is 12 in both builds), and VC10 spends 6 bytes on an explicit float cast
//   (store + reload). Measured on this row with `w2`: no cast #1023 (the lea); cast of the whole product #1024 (match); cast of
//   the sum only, `(float)(value + 1.0f) * 512.5f`, #1023; a two-step scale (`* 0.5f * 1025.0f`, also 24 bytes at /Od) #1023;
//   outer brackets instead of the cast #1024 (same bytes as the cast; the cast is the form the debug size supports).
// Inferred: `goto rotation_done;` in InlineGetKeyframeInterpolate (the landed 0x00A53860 / 0x00A52760 / 0x00A4C1F0 text: Ego_d's line 56
//   is a 2-byte jump out of the slerp; a `break` out of a one-pass loop would be the other 2-byte shape, not graded).
// Family check (tools/il/fam.py, famrun.sh): with both changes the landed 0x00A4D920 (1341 bytes), 0x00A52760 (1004) and
//   0x00A53860 (1005) still grade RELOCATION_MATCH; the October one-expression helper texts with a `w2` copy matched this row
//   (#1024) but broke 0x00A4D920 (a Dot term order), so they are not the text.
// Callees: C3DAnimation2::ConvertTimeToFrames 0x00A52650; globals GFastArcCosTable 0x013CB530 / GFastCosTable 0x013CD550.
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
#define FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCALRELATIVE
#define FABLE_RETAIL_C3DMESH2_GETBONEKEYFRAME
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_CQUATERNION_SLERP
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

FABLE_FORCEINLINE void C3DAnimation2::GetKeyframeInterpolateFixPos(unsigned long bone_index, C3DKeyframe& dest, unsigned long frame, float fraction) const
{
    Sequences[bone_index].InlineGetKeyframeInterpolate(dest, frame, fraction);
    if (RigidBoneMask && RigidBoneMask[bone_index])
    {
        dest.SetPosition(RigidBonePositions[bone_index]);
    }
}

void C3DAnimation2::CalcLocalTransformationsRelative(C3DKeyframe* dest, unsigned long dest_size, float t, const C3DMesh2& mesh) const
{
    long frame;
    float fraction;
    ConvertTimeToFrames(t, frame, fraction);
    for (unsigned long c_seq = 0; c_seq < SequenceCount; c_seq++)
    {
        C3DKeyframe kf_rel;
        GetKeyframeInterpolateFixPos(c_seq, kf_rel, frame, fraction);
        dest[c_seq] = *mesh.GetBoneKeyframe(c_seq);
    }
}