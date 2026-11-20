// C3DAnimationSequence::InlineGetKeyframeInterpolate(C3DKeyframe& dest, unsigned long frame, float fraction) const -- retail's out-of-line copy of the header forwarder: the keyframe of the sequence's Data between `frame` and the next frame, with the data's InlineGetKeyframeInterpolate expanded in it
// Leaf: ?InlineGetKeyframeInterpolate@C3DAnimationSequence@@QBEXAAVC3DKeyframe@@KM@Z
// Flags: none
// Retail-asm: 0x00A53963 `fistp dword ptr [esp+18h]`, 0x00A539CB `fistp dword ptr [esp+18h]`, 0x00A53A35 `fistp dword ptr [esp+14h]` -- engine/GFFloatToLongNear.h inside GFFastArcCos / GFFastSin. This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A53860, 1005 bytes, thiscall, `ret 0Ch`; no caller in Fable.exe.
// Identity: an uncalled COMDAT in the animation-file TU; 0x00A53863 `mov ecx,[ecx]` loads the C3DAnimationSequence's Data (+0) and the
//   rest is the sequence data's inline body, (C3DKeyframe&, unsigned long, float): FableWin.pdb / Ego_d.pdb
//   ?InlineGetKeyframeInterpolate@C3DAnimationSequence@@QBEXAAVC3DKeyframe@@KM@Z (FableWin 0x032274B0, 38 bytes, one line 307).
//   C3DAnimationSequence::GetKeyframeInterpolate(KM) forwards to the data's out-of-line 0x00A4C1F0 instead (a call, not this body).
//   The catalog name C3DAnimation2::CalcLocalBoneTransformation is wrong (that function is 0x00A4D330, `this` a C3DAnimation2).
// Shape: the Ego_d.pdb / FableWin.pdb debug bodies, statement for statement from their per-line tables (egod_lines.tsv), with their
//   locals: C3DAnimationSequenceData::InlineGetKeyframeInterpolate (FableWin 0x032274E0, 1404 bytes; next_frame, w1, w2; index /
//   next_index; position / next_position; lines 39..92: SetSlerp 49 bytes, SetIdentity 8, SetPosition 106 / 43 / 38), CQuaternion::
//   SetSlerp (0x031E2170, 323 bytes; q1, q2, weight2; cosa, sign, scale1, scale2, angle, sina; `cosa = -cosa; sign = -1.0f;`,
//   both scales divided by sina, `scale2 *= sign;`), CQuaternion::Dot (0x031E2310; q2), GFFastArcCos (0x02F30710; value; pos,
//   index, interp, result; `result *= 1 / 2pi` its own 12-byte line) and GFFastSin (0x02DCFC30; angle; pos, index, interp, result;
//   `index -= 256; index &= 1023;` two lines). The debug ASSERTs (lines 37 / 38 and GFFastArcCos line 238) compile to nothing.
//   The equal-palette-entry copy leaves the slerp with a 2-byte jump (line 56): spelled `goto`, as the landed 0x00A4D920.
//   Retail divides once: 0x00A539E5 `fld 1.0 ; fdiv sina` and two multiplies (VC7.1 shares the reciprocal of the two `/ sina`).
// Inline: the helpers are FABLE_FORCEINLINE (ASM_RULES rule 3.3: their bodies reach the GFFloatToLongNear asm helper, which VC7.1
//   never auto-inlines; retail has them expanded here). These helper texts also give RELOCATION_MATCH on the landed 0x00A4D920 (regraded, see the batch README).
// Inferred: `goto rotation_done;` for the equal-palette-entry copy. Ego_d's line 56 is a 2-byte jump out of the slerp; `goto` is the landed
//   0x00A4D920 spelling, and a `break` out of a one-pass loop would be the other 2-byte shape (not graded). [label added by audit h3c2b:
//   the README declared this choice, the source did not]
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_C3DANIMATIONSEQUENCE_INLINEGETKEYFRAMEINTERPOLATE_DECL
#define FABLE_RETAIL_CQUATERNION_SLERP
#define FABLE_RETAIL_C3DKEYFRAME_SETPOSITION
#define FABLE_RETAIL_C4DVECTOR_ASSIGN3D
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimationSequence.h"
#include "engine/GFFloatToLongNear.h"
extern float GFastArcCosTable[1026];    // 0x013CB530 ?GFastArcCosTable@@3PAMA (acos(k * 2 / 1025 - 1), k = 0..1025)
extern float GFastCosTable[1025];       // 0x013CD550 ?GFastCosTable@@3PAMA (cos(2 * pi * k / 1024), k = 0..1024)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
FABLE_FORCEINLINE float __fastcall GFFastArcCos(float value)
{
    float pos = (value + 1.0f) * 512.5f;
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
        dest.Rotation.SetSlerp(RotationTrack[index], RotationTrack[next_index], fraction);
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

void C3DAnimationSequence::InlineGetKeyframeInterpolate(C3DKeyframe& dest, unsigned long frame, float fraction) const
{
    Data->InlineGetKeyframeInterpolate(dest, frame, fraction);
}