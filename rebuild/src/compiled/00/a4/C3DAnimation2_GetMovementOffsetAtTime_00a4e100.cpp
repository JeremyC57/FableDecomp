// Leaf: ?GetMovementOffsetAtTime@C3DAnimation2@@UBE?AVC3DVector@@M@Z
// C3DAnimation2::GetMovementOffsetAtTime(float) -- the movement track's interpolated keyframe position at the given time as a C3DVector (hidden result pointer), or zero when there is no movement track
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4E100, length 77 bytes.
// Callees: C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe*, float) @ 0x00A4DEA0.
// Retail C3DAnimation2 holds the movement track CCountedPointer<C3DAnimationSequence> at +0x40
// (PDB +0x44; the Ego_r layout is 4 bytes longer before it); C3DAnimationSequence starts with
// its C3DAnimationSequenceData pointer (PDB). C3DKeyframe is {CQuaternion Rotation, C4DVector
// Position +0x10, C4DVector Scaling} = 0x30 bytes, the stack frame. ret 8 = hidden pointer + time.
class C3DKeyframe { public:
    float Rotation[4];                     // +0 CQuaternion
    float Position[4];                     // +0x10 C4DVector X, Y, Z, W
    float Scaling[4];                      // +0x20 C4DVector
};

class C3DAnimationSequenceData { public:
    void GetKeyframeInterpolate(C3DKeyframe& keyframe, float time) const;   // 0x00A4DEA0  NF37: Ego_r + FableWin ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@M@Z
};

class C3DAnimationSequence { public:
    C3DAnimationSequenceData* Data;        // +0
};

#include "engine/C3DVector.h"

class C3DAnimation2 { public:
    unsigned char _pad[0x3c];
    C3DAnimationSequence* MovementTrack;   // +0x40 CCountedPointer<C3DAnimationSequence>::m_pObject
    virtual C3DVector GetMovementOffsetAtTime(float time) const;
};

C3DVector C3DAnimation2::GetMovementOffsetAtTime(float time) const
{
    if (MovementTrack) {                   // the track path is the if-body: the zero path sits at the end
        C3DKeyframe keyframe;
        MovementTrack->Data->GetKeyframeInterpolate(keyframe, time);
        return C3DVector(keyframe.Position[0], keyframe.Position[1], keyframe.Position[2]);
    }
    return C3DVector(0.0f, 0.0f, 0.0f);
}