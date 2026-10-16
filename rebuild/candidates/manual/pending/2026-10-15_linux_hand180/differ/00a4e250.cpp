// Grade: DIFFER(171v171) -- register rotation only: retail keeps the result quaternion's X,Y,Z,W in edi,edx,ecx,esi, ours in ecx,edx,esi,edi (clamp, call and both copy arms otherwise identical); tried rotation local declared before/after the duration local
// C3DAnimation2::GetRotationAtTime(float) -- the movement track's interpolated rotation at the time clamped to [0, Duration], or the identity quaternion when there is no track
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4E250, length 176 bytes (`ret 8`: hidden result + time).
// Callees: C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe*, float) @ 0x00A4DEA0 (0x30-byte keyframe).
// Shape: the time parameter is clamped in place (`if (t < 0) t = 0; else if (t > Duration) t = Duration;`, the
// duration loaded first); both returns copy a CQuaternion (the keyframe's Rotation, or a named identity local:
// no NRVO) through edi/edx/ecx/esi into the result in one shared tail.
// Layout: as the landed GetMovementOffsetAtTime 0x00A4E100: MovementTrack (CCountedPointer<C3DAnimationSequence>
// object) at +0x40, C3DAnimationSequence::Data at +0; the duration float at +0x0c.
// Leaf: GetRotationAtTime@C3DAnimation2
struct CQuaternion {
    float X, Y, Z, W;
    CQuaternion() {}
    CQuaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct C3DKeyframe {
    CQuaternion Rotation;                  // +0
    float Position[4];                     // +0x10 C4DVector
    float Scaling[4];                      // +0x20 C4DVector
};
struct C3DAnimationSequenceData {
    void GetKeyframeInterpolate(C3DKeyframe* keyframe, float time);   // 0x00A4DEA0
};
struct C3DAnimationSequence {
    C3DAnimationSequenceData* Data;        // +0
};
struct C3DAnimation2 {
    unsigned char _pad00[0x0c];
    float Duration;                        // +0x0c
    unsigned char _pad10[0x30];
    C3DAnimationSequence* MovementTrack;   // +0x40 CCountedPointer<C3DAnimationSequence>::m_pObject
    CQuaternion GetRotationAtTime(float time);
};

CQuaternion C3DAnimation2::GetRotationAtTime(float time)
{
    float duration = Duration;
    if (time < 0.0f)
        time = 0.0f;
    else if (time > duration)
        time = duration;
    CQuaternion rotation;
    if (MovementTrack) {
        C3DKeyframe keyframe;
        MovementTrack->Data->GetKeyframeInterpolate(&keyframe, time);
        rotation = keyframe.Rotation;
    }
    else
        rotation = CQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
    return rotation;
}
