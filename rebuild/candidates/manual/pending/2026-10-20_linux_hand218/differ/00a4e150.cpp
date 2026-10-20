// C3DAnimation2::GetMovementAtTime(float time) const -- the animation's root movement at a time: the linear movement vector scaled by the clamped time fraction, plus the movement track's keyframe position
// Grade: DIFFER(ours v retail) -- only the X term of the track add loads movement.X first (fld [esp+4] ; fadd [esp+38h] vs retail fld [esp+38h] ; fadd [esp+4]); Y and Z match
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4E150, 256 bytes (`ret 8`: hidden C3DVector result + time).
// Callees: C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe*, float) 0x00A4DEA0; 0.0f pool 0x0122DEDC.
// Shape: Duration is loaded first and kept on the x87 stack; time is clamped in its own argument slot
// (`< 0` = `test ah,5 ; jp`, `> duration` = `test ah,41h ; jne`, the `fst` stores the duration);
// the result starts at zero, a positive duration adds MovementVector * (time / duration) (the fraction a
// named float spilled to [esp]); with a movement track the interpolated keyframe position is added
// (`fld key ; fadd sum` from a plain `X += o.X` body).
// Layout: C3DAnimationStats base (Ego_r.pdb): Duration +0x0C, LoopFlag +0x10, MovementFlag +0x11,
// MovementVector +0x12 (2-byte packed); retail C3DAnimation2 MovementTrack +0x40 (PDB +0x44, landed 0x00A4E100).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
};

struct C3DKeyframe {
    float Rotation[4];                     // +0 CQuaternion
    C3DVector Position;                    // +0x10 C4DVector X, Y, Z
    float PositionW;                       // +0x1c
    float Scaling[4];                      // +0x20 C4DVector
};

struct C3DAnimationSequenceData {
    void GetKeyframeInterpolate(C3DKeyframe* keyframe, float time);   // 0x00A4DEA0
};

struct C3DAnimationSequence {
    C3DAnimationSequenceData* Data;        // +0
};

#pragma pack(push, 2)
struct C3DAnimationStats {
    virtual ~C3DAnimationStats();
    long _04;
    long AnimType;                         // +0x08 EAnimationType
    float Duration;                        // +0x0c
    bool LoopFlag;                         // +0x10
    bool MovementFlag;                     // +0x11
    C3DVector MovementVector;              // +0x12
    unsigned char _1e[0x0e];               // TimingEvents (retail 12-byte multimap) to 0x2c
};
#pragma pack(pop)

struct C3DAnimation2 : C3DAnimationStats {
    unsigned char _2c[0x14];               // CMovableResource vptr, Allocator, MemoryPool, MemoryHandle
    C3DAnimationSequence* MovementTrack;   // +0x40 CCountedPointer<C3DAnimationSequence>::Data
    C3DVector GetMovementAtTime(float time) const;
};

C3DVector C3DAnimation2::GetMovementAtTime(float time) const
{
    float duration = Duration;
    if (time < 0.0f)
        time = 0.0f;
    else if (time > duration)
        time = duration;
    C3DVector movement(0.0f, 0.0f, 0.0f);
    if (duration > 0.0f) {
        C3DVector vector = MovementVector;
        float fraction = time / duration;
        movement = vector * fraction;
    }
    if (MovementTrack) {
        C3DKeyframe keyframe;
        MovementTrack->Data->GetKeyframeInterpolate(&keyframe, time);
        movement += keyframe.Position;
    }
    return movement;
}
