// Behaviour test for 0x00A4D920 C3DAnimation2::CalcLocalBoneTransformationRelative(C3DKeyframe& keyframe, unsigned long bone,
// float time, const C3DMesh2& mesh) const: host model. The time becomes a whole frame (floor, wrapped into the frame count) and a
// fraction (ConvertTimeToFrames); the bone's sequence gives the local keyframe (rotation slerped between this frame and the next one,
// directly or through the palette, or constant / identity; position interpolated from the word track scaled by PositionFactor, W = 0);
// a rigid bone takes its fixed position; the result is the mesh's rest keyframe with the local one post-applied (rotation = rest *
// local through CQuaternion::operator*, position += local position). The table-driven arc cosine / sine are built here from the
// same tables the engine reads (1026-entry arc-cosine, 1025-entry cosine), rounded with the nearest-integer conversion.
#include <cstdio>
#include <cmath>

struct Quat { float X, Y, Z, W; };
struct V4 { float X, Y, Z, W; };
struct V3 { float X, Y, Z; };
struct Key { Quat Rotation; V4 Position; V4 Scaling; };
struct Word { short X, Y, Z; };
struct SeqData {
    unsigned long FrameCount; int RotationTrackMode, PositionTrackMode; float PositionFactor;
    const Quat* RotationTrack; const Word* PositionTrack; const unsigned char* PalettedRotationTrack; const unsigned char* PalettedPositionTrack;
};
struct Anim { float SamplesPerSecond; unsigned long FrameCount; const SeqData* Sequences; const bool* RigidBoneMask; const V3* RigidBonePositions; };

static float g_ArcCosTable[1026];
static float g_CosTable[1025];
static long Near(float x) { return (long)std::floor(x + 0.5f); }
static float FastArcCos(float c)
{
    c = (c + 1.0f) * 512.5f; long i = Near(c - 0.5f); float f = c - (float)i;
    return ((1.0f - f) * g_ArcCosTable[i] + f * g_ArcCosTable[i + 1]) * 0.15915494f;
}
static float FastSin(float a)
{
    a = a * 1024.0f; long i = Near(a - 0.5f); float f = a - (float)i; i = (i - 256) & 0x3ff;
    return (1.0f - f) * g_CosTable[i] + f * g_CosTable[i + 1];
}
static void Slerp(Quat& r, const Quat& a, const Quat& b, float t)
{
    float cosom = b.X * a.X + b.Y * a.Y + b.Z * a.Z + b.W * a.W, sign = 1.0f, s0, s1;
    if (cosom < 0.0f) { sign = -1.0f; cosom = -cosom; }
    if (1.0f - cosom > 0.05f) {
        float omega = FastArcCos(cosom), sinom = (float)std::sqrt(1.0f - cosom * cosom);
        s0 = FastSin((1.0f - t) * omega) * (1.0f / sinom); s1 = FastSin(omega * t) * (1.0f / sinom);
    } else { s0 = 1.0f - t; s1 = t; }
    s1 *= sign;
    r.X = s0 * a.X + s1 * b.X; r.Y = s0 * a.Y + s1 * b.Y; r.Z = s0 * a.Z + s1 * b.Z; r.W = s0 * a.W + s1 * b.W;
}
static Quat Mul(const Quat& p, const Quat& q)                     // CQuaternion::operator* 0x00A88B60 (scalar part W)
{
    Quat r;
    r.W = p.W * q.W - q.X * p.X - p.Y * q.Y - p.Z * q.Z;
    r.X = p.W * q.X + p.X * q.W + p.Y * q.Z - p.Z * q.Y;
    r.Y = q.Y * p.W + p.Y * q.W + p.Z * q.X - q.Z * p.X;
    r.Z = q.Z * p.W + q.Y * p.X + p.Z * q.W - p.Y * q.X;
    return r;
}
static void Interpolate(const SeqData& s, Key& k, unsigned long frame, float fraction)
{
    unsigned long next = (frame + 1 < s.FrameCount) ? frame + 1 : 0;
    float omf = 1.0f - fraction;
    if (s.RotationTrackMode & 2) {
        unsigned long i0 = frame, i1 = next;
        if (s.RotationTrackMode == 3) { i0 = s.PalettedRotationTrack[frame]; i1 = s.PalettedRotationTrack[next]; }
        if (s.RotationTrackMode == 3 && i0 == i1) k.Rotation = s.RotationTrack[i0];
        else Slerp(k.Rotation, s.RotationTrack[i0], s.RotationTrack[i1], fraction);
    } else if (s.RotationTrackMode == 1) k.Rotation = s.RotationTrack[0];
    else { k.Rotation.X = 0; k.Rotation.Y = 0; k.Rotation.Z = 0; k.Rotation.W = 1; }
    if (s.PositionTrackMode & 2) {
        unsigned long i0 = frame, i1 = next;
        if (s.PositionTrackMode == 3) { i0 = s.PalettedPositionTrack[frame]; i1 = s.PalettedPositionTrack[next]; }
        const Word& a = s.PositionTrack[i0]; const Word& b = s.PositionTrack[i1];
        float fa = omf * s.PositionFactor, fb = fraction * s.PositionFactor;
        k.Position.X = a.X * fa + b.X * fb; k.Position.Y = a.Y * fa + b.Y * fb; k.Position.Z = a.Z * fa + b.Z * fb;
    } else if (s.PositionTrackMode == 1) {
        k.Position.X = s.PositionTrack[0].X * s.PositionFactor; k.Position.Y = s.PositionTrack[0].Y * s.PositionFactor;
        k.Position.Z = s.PositionTrack[0].Z * s.PositionFactor;
    } else { k.Position.X = 0; k.Position.Y = 0; k.Position.Z = 0; }
    k.Position.W = 0.0f;
}
static void CalcRelative(const Anim& an, Key& keyframe, unsigned long bone, float time, const Key* restKeyframes)  // 0x00A4D920
{
    float frames = time * an.SamplesPerSecond;
    long frame = (long)std::floor(frames);
    float fraction = frames - frame;
    frame = (long)((unsigned long)frame % an.FrameCount);
    Key local;
    Interpolate(an.Sequences[bone], local, (unsigned long)frame, fraction);
    if (an.RigidBoneMask && an.RigidBoneMask[bone]) {
        local.Position.X = an.RigidBonePositions[bone].X; local.Position.Y = an.RigidBonePositions[bone].Y;
        local.Position.Z = an.RigidBonePositions[bone].Z; local.Position.W = 0.0f;
    }
    keyframe = restKeyframes[bone];
    keyframe.Rotation = Mul(keyframe.Rotation, local.Rotation);
    keyframe.Position.X += local.Position.X; keyframe.Position.Y += local.Position.Y;
    keyframe.Position.Z += local.Position.Z; keyframe.Position.W += local.Position.W;
}
static bool Close(float a, float b) { return std::fabs(a - b) < 2e-3f; }

int main()
{
    const double pi = 3.14159265358979;
    for (int i = 0; i < 1026; ++i) { double c = i / 512.5 - 1.0; if (c > 1.0) c = 1.0; g_ArcCosTable[i] = (float)std::acos(c); }
    for (int i = 0; i < 1025; ++i) g_CosTable[i] = (float)std::cos(2.0 * pi * i / 1024.0);
    const float h = 0.70710678f;
    Quat rot[3] = { { 0, 0, 0, 1 }, { 0, 0, h, h }, { h, 0, 0, h } };
    Word pos[3] = { { 0, 0, 0 }, { 100, 200, -40 }, { 7, 7, 7 } };
    unsigned char pal[4] = { 0, 1, 1, 2 };
    SeqData seqs[3] = {
        { 4, 2, 2, 0.1f, rot, pos, 0, 0 },              // direct tracks
        { 4, 3, 3, 0.1f, rot, pos, pal, pal },          // paletted tracks
        { 4, 1, 0, 1.0f, rot + 2, pos, 0, 0 },          // constant rotation, no position track
    };
    bool rigid[3] = { false, false, true };
    V3 rigidPos[3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 1, 2, 3 } };
    Anim an = { 10.0f, 4, seqs, rigid, rigidPos };
    Key rest[3];
    for (int i = 0; i < 3; ++i) {
        rest[i].Rotation = rot[0];
        rest[i].Position.X = 10.0f * i; rest[i].Position.Y = 1; rest[i].Position.Z = 2; rest[i].Position.W = 1;
        rest[i].Scaling.X = rest[i].Scaling.Y = rest[i].Scaling.Z = rest[i].Scaling.W = 1;
    }
    int fails = 0;
    Key k;
    // bone 0 at t = 0.05 s: frame 0, fraction 0.5: 45 degrees about Z, position half way to (10, 20, -4).
    CalcRelative(an, k, 0, 0.05f, rest);
    if (!Close(k.Rotation.Z, 0.38268343f) || !Close(k.Rotation.W, 0.92387953f)) ++fails;
    if (!Close(k.Position.X, 5.0f) || !Close(k.Position.Y, 11.0f) || !Close(k.Position.Z, 0.0f) || !Close(k.Position.W, 1.0f)) ++fails;
    // bone 1 at t = 0.45 s: frame 4 wraps to 0, fraction 0.5; the palette maps frames 0 / 1 to entries 0 / 1 (same as bone 0).
    CalcRelative(an, k, 1, 0.45f, rest);
    if (!Close(k.Rotation.Z, 0.38268343f) || !Close(k.Position.X, 15.0f)) ++fails;
    // bone 1 at frame 1 -> 2: both palette entries are 1, the rotation is copied, not slerped.
    CalcRelative(an, k, 1, 0.13f, rest);
    if (!Close(k.Rotation.Z, h) || !Close(k.Rotation.W, h)) ++fails;
    // bone 2: constant rotation about X, rigid position (1, 2, 3) on top of the rest position (20, 1, 2).
    CalcRelative(an, k, 2, 0.2f, rest);
    if (!Close(k.Rotation.X, h) || !Close(k.Rotation.W, h)) ++fails;
    if (!Close(k.Position.X, 21.0f) || !Close(k.Position.Y, 3.0f) || !Close(k.Position.Z, 5.0f) || !Close(k.Position.W, 1.0f)) ++fails;
    if (!Close(k.Scaling.X, 1.0f)) ++fails;
    // a non-identity rest rotation is pre-multiplied: rest (90 about Z) * local (90 about X).
    rest[2].Rotation = rot[1];
    CalcRelative(an, k, 2, 0.2f, rest);
    if (!Close(k.Rotation.W, 0.5f) || !Close(k.Rotation.X, 0.5f) || !Close(k.Rotation.Y, 0.5f) || !Close(k.Rotation.Z, 0.5f)) ++fails;
    if (fails) { std::printf("00a4d920_TEST FAIL\n"); return 1; }
    std::printf("00a4d920_TEST PASS\n");
    return 0;
}