// C3DAnimation2::CalcLocalBoneTransformationRelative(C3DKeyframe& keyframe, unsigned long bone, float time, const C3DMesh2& mesh) const -- the bone's local keyframe at `time` (slerped rotation, interpolated position, rigid-bone override) applied on top of the mesh's rest keyframe
// Grade: DIFFER(1341v1341) -- one x87 operand pair: 0x00A4DAC6 retail `fld [esp+18h]; fmul [esp+14h]` (omega * (1-t)), ours loads the (1-t) CSE temp first; everything else byte-matches
// Leaf: ?CalcLocalBoneTransformationRelative@C3DAnimation2@@QBEXAAVC3DKeyframe@@KMABVC3DMesh2@@@Z
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
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

void C3DAnimation2::CalcLocalBoneTransformationRelative(C3DKeyframe& keyframe, unsigned long bone, float time, const C3DMesh2& mesh) const
{
    long frame;
    float fraction;
    ConvertTimeToFrames(time, frame, fraction);
    C3DKeyframe local;
    const C3DAnimationSequence& sequence = Sequences[bone];
    sequence.InlineGetKeyframeInterpolate(local, frame, fraction);
    if (RigidBoneMask && RigidBoneMask[bone])
        local.Position = RigidBonePositions[bone];
    keyframe = mesh.BoneKeyframes[bone];
    keyframe.Rotation *= local.Rotation;
    keyframe.Position.X += local.Position.X;
    keyframe.Position.Y += local.Position.Y;
    keyframe.Position.Z += local.Position.Z;
    keyframe.Position.W += local.Position.W;
}
