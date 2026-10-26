#include <cstdio>
class C3DKeyframe { public: float Rotation[4]; float Position[4]; float Scaling[4]; };
class C3DAnimationSequenceData { public: void GetKeyframeInterpolate(C3DKeyframe* k, float t) { k->Position[0] = t; k->Position[1] = t * 2; k->Position[2] = t * 3; k->Position[3] = 1; } };
class C3DAnimationSequence { public: C3DAnimationSequenceData* Data; };
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
class C3DAnimation2 { public: unsigned char _pad[0x40]; C3DAnimationSequence* MovementTrack;
    C3DVector GetMovementOffsetAtTime(float t) { if (MovementTrack) { C3DKeyframe k; MovementTrack->Data->GetKeyframeInterpolate(&k, t); return C3DVector(k.Position[0], k.Position[1], k.Position[2]); } return C3DVector(0.0f, 0.0f, 0.0f); } };
int main()
{
    C3DAnimationSequenceData d; C3DAnimationSequence s; s.Data = &d; C3DAnimation2 a; a.MovementTrack = &s;
    C3DVector v = a.GetMovementOffsetAtTime(1.5f);
    if (sizeof(C3DKeyframe) != 0x30 || v.X != 1.5f || v.Y != 3.0f || v.Z != 4.5f) { std::printf("00a4e100_TEST FAIL\n"); return 1; }
    a.MovementTrack = 0; C3DVector z = a.GetMovementOffsetAtTime(9.0f);
    if (z.X != 0 || z.Y != 0 || z.Z != 0) { std::printf("00a4e100_TEST FAIL\n"); return 1; }
    std::printf("00a4e100_TEST PASS\n"); return 0;
}