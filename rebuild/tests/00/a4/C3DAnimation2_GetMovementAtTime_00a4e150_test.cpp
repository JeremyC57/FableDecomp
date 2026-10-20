// Behaviour test for 00a4e150: body copied from src/00a4e150.cpp (C3DAnimation2::GetMovementAtTime), GetKeyframeInterpolate stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool feq(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void operator+=(const C3DVector& o) { float x = o.X; float y = o.Y; float z = o.Z; X += x; Y += y; Z += z; }
};
struct C3DKeyframe {
    float Rotation[4];
    C3DVector Position;
    float PositionW;
    float Scaling[4];
};
static int g_interp = 0; static float g_interpTime = -99.0f; static C3DVector g_keyPos(0, 0, 0);
struct C3DAnimationSequenceData {
    void GetKeyframeInterpolate(C3DKeyframe* keyframe, float time) { ++g_interp; g_interpTime = time; keyframe->Position = g_keyPos; }
};
struct C3DAnimationSequence { C3DAnimationSequenceData* Data; };
#pragma pack(push, 2)
struct C3DAnimationStats {
    virtual ~C3DAnimationStats() {}
    long _04;
    long AnimType;
    float Duration;
    bool LoopFlag;
    bool MovementFlag;
    C3DVector MovementVector;
    unsigned char _1e[0x0e];
};
#pragma pack(pop)
struct C3DAnimation2 : C3DAnimationStats {
    unsigned char _2c[0x14];
    C3DAnimationSequence* MovementTrack;
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
int main()
{
    C3DAnimation2 anim; memset(anim._2c, 0, sizeof(anim._2c));
    anim.Duration = 2.0f; anim.MovementVector = C3DVector(4.0f, 8.0f, -2.0f); anim.MovementTrack = 0;
    C3DVector r = anim.GetMovementAtTime(0.5f);
    CHECK(feq(r.X, 1.0f) && feq(r.Y, 2.0f) && feq(r.Z, -0.5f)); CHECK(g_interp == 0);
    r = anim.GetMovementAtTime(-3.0f);               // clamped to 0
    CHECK(feq(r.X, 0.0f) && feq(r.Y, 0.0f) && feq(r.Z, 0.0f));
    r = anim.GetMovementAtTime(10.0f);               // clamped to duration
    CHECK(feq(r.X, 4.0f) && feq(r.Y, 8.0f) && feq(r.Z, -2.0f));
    C3DAnimationSequenceData data; C3DAnimationSequence seq; seq.Data = &data; anim.MovementTrack = &seq;
    g_keyPos = C3DVector(0.25f, 0.5f, 1.0f);
    r = anim.GetMovementAtTime(1.0f);
    CHECK(g_interp == 1); CHECK(feq(g_interpTime, 1.0f));
    CHECK(feq(r.X, 2.25f) && feq(r.Y, 4.5f) && feq(r.Z, 0.0f));
    anim.Duration = 0.0f;                            // no linear part, track only (time clamped to 0)
    r = anim.GetMovementAtTime(5.0f);
    CHECK(g_interp == 2); CHECK(feq(g_interpTime, 0.0f));
    CHECK(feq(r.X, 0.25f) && feq(r.Y, 0.5f) && feq(r.Z, 1.0f));
    if (fails == 0) printf("00a4e150_TEST PASS\n"); else printf("00a4e150_TEST FAIL\n");
    return fails;
}