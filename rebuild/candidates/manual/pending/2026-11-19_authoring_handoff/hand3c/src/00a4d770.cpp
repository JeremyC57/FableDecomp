// C3DAnimation2::CalcLocalTransformationsRelative(C3DKeyframe* dest, unsigned long dest_size, float t, const C3DMesh2& mesh) const -- for every sequence, interpolate the animation keyframe at `t` into a local (GetKeyframeInterpolateFixPos) and copy the mesh's rest keyframe for that bone into dest
// Leaf: ?CalcLocalTransformationsRelative@C3DAnimation2@@QBEXPAVC3DKeyframe@@KMABVC3DMesh2@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4D770, 420 bytes, thiscall, `ret 10h`.
// Identity: Ego_r.pdb / FableWin.pdb ?CalcLocalTransformationsRelative@C3DAnimation2@@QBEXPAVC3DKeyframe@@KMABVC3DMesh2@@@Z (Ego_r 0x00BA2EB0
//   has the same shape; caller C3DAnimationManager::CombineBaseAnimations 0x00A9F1D0). Locals dest, dest_size, t, mesh, frame, fraction,
//   c_seq, kf_rel: Ego_d.pdb.
// Shape: FableWin 0x03227CA0: ConvertTimeToFrames(t, frame, fraction); for c_seq < SequenceCount: C3DKeyframe kf_rel;
//   GetKeyframeInterpolateFixPos(c_seq, kf_rel, frame, fraction); dest[c_seq] = *mesh.GetBoneKeyframe(c_seq) (`rep movsd` 0x30 bytes).
//   kf_rel is never read, so every store into it is dead; what survives is the slerp weight work up to the GFFloatToLongNear
//   `fistp`s (asm, kept) and the paletted-index compare that guards it. GetKeyframeInterpolateFixPos (FableWin 0x03227270, private;
//   Ego_d.pdb params bone_index, dest, frame, fraction) is Sequences[bone_index].InlineGetKeyframeInterpolate(dest, frame, fraction)
//   then the rigid-bone position override (FableWin +0x74 / +0x78 = RigidBoneMask / RigidBonePositions in the retail layout).
// Inline: GetKeyframeInterpolateFixPos is FABLE_FORCEINLINE (ASM_RULES rule 3.3: it reaches the GFFloatToLongNear asm helper, so
//   VC7.1 never auto-inlines it; retail has no out-of-line copy). The helper texts above it are the landed 0x00A4D920 row's, verbatim
//   (one text per function). ConvertTimeToFrames stays called (0x00A52650; GFFloatToLongFloor asm).
// Callees: C3DAnimation2::ConvertTimeToFrames 0x00A52650; globals g_ArcCosTable 0x013CB530 / g_CosTable 0x013CD550.
// Retail-asm: 0x00A4D859 `fistp dword ptr [esp+14h]`, 0x00A4D8AF `fistp dword ptr [esp+24h]`, 0x00A4D8CF `fistp dword ptr [esp+28h]` --
//   engine/GFFloatToLongNear.h inside GFFastArcCos / GFFastSin (as in 0x00A4D920). This row carries no asm of its own.
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCALRELATIVE
#define FABLE_RETAIL_C3DMESH2_GETBONEKEYFRAME
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#define FABLE_RETAIL_CQUATERNION_SLERP
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

FABLE_FORCEINLINE void C3DAnimation2::GetKeyframeInterpolateFixPos(unsigned long bone_index, C3DKeyframe& dest, unsigned long frame, float fraction) const
{
    Sequences[bone_index].InlineGetKeyframeInterpolate(dest, frame, fraction);
    if (RigidBoneMask && RigidBoneMask[bone_index])
        dest.Position = RigidBonePositions[bone_index];
}

void C3DAnimation2::CalcLocalTransformationsRelative(C3DKeyframe* dest, unsigned long dest_size, float t, const C3DMesh2& mesh) const
{
    long frame;
    float fraction;
    ConvertTimeToFrames(t, frame, fraction);
    for (unsigned long c_seq = 0; c_seq < SequenceCount; c_seq++) {
        C3DKeyframe kf_rel;
        GetKeyframeInterpolateFixPos(c_seq, kf_rel, frame, fraction);
        dest[c_seq] = *mesh.GetBoneKeyframe(c_seq);
    }
}
