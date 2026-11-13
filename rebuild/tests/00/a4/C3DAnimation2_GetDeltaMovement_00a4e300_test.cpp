// Behaviour test for 0x00A4E300 C3DAnimation2::GetDeltaMovement(CMatrix3x4& delta, float start_time, float end_time) const: host
// model. Without a movement track the delta is the identity. Otherwise both times are clamped to [0, non-looping duration]
// ((FrameCount - 1) / SamplesPerSecond), the movement track's keyframes at both times are turned into rotation matrices
// (C3DKeyframe::ToMatrix, no scaling), the delta's rotation is the start rotation transposed times the end rotation
// (SetMultiplyNoTranslation, translation row cleared), and its translation is the position difference plus the linear movement
// vector times the elapsed fraction of the duration, rotated into the start frame (the transposed start rotation).
#include <cstdio>
#include <cmath>

struct M { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct Q { float X, Y, Z, W; };
struct V { float X, Y, Z; };
struct Key { Q R; V P; };
struct Track { Key K0, K1; float T0, T1; };        // a two-key linear track for the host model

static Key Sample(const Track& t, float time)
{
    float f = (t.T1 > t.T0) ? (time - t.T0) / (t.T1 - t.T0) : 0.0f;
    if (f < 0.0f) f = 0.0f; if (f > 1.0f) f = 1.0f;
    Key k;
    k.R.X = t.K0.R.X + (t.K1.R.X - t.K0.R.X) * f; k.R.Y = t.K0.R.Y + (t.K1.R.Y - t.K0.R.Y) * f;
    k.R.Z = t.K0.R.Z + (t.K1.R.Z - t.K0.R.Z) * f; k.R.W = t.K0.R.W + (t.K1.R.W - t.K0.R.W) * f;
    k.P.X = t.K0.P.X + (t.K1.P.X - t.K0.P.X) * f; k.P.Y = t.K0.P.Y + (t.K1.P.Y - t.K0.P.Y) * f; k.P.Z = t.K0.P.Z + (t.K1.P.Z - t.K0.P.Z) * f;
    return k;
}
static void ToMatrix(const Q& q, M& m)
{
    float n = q.W * q.W + q.Z * q.Z + q.Y * q.Y + q.X * q.X;
    float s = (n != 0.0f) ? 2.0f / n : 1.0f;
    float xs = q.X * s, ys = q.Y * s, zs = q.Z * s;
    float wx = q.W * xs, wy = q.W * ys, wz = q.W * zs, xx = q.X * xs, xy = q.X * ys, xz = q.X * zs, yy = q.Y * ys, yz = q.Y * zs, zz = q.Z * zs;
    m.E11 = 1.0f - (yy + zz); m.E12 = xy - wz; m.E13 = xz + wy;
    m.E21 = xy + wz; m.E22 = 1.0f - (xx + zz); m.E23 = yz - wx;
    m.E31 = xz - wy; m.E32 = yz + wx; m.E33 = 1.0f - (xx + yy);
}
static void GetDeltaMovement(const Track* track, unsigned long frames, float rate, V mv, M& d, float t0, float t1)
{
    if (!track) {
        d.E12 = d.E13 = d.E21 = d.E23 = d.E31 = d.E32 = d.E41 = d.E42 = d.E43 = 0.0f; d.E11 = d.E22 = d.E33 = 1.0f; return;
    }
    float duration = (frames - 1.0f) / rate;
    if (t0 < 0.0f) t0 = 0.0f; else if (t0 > duration) t0 = duration;
    if (t1 < 0.0f) t1 = 0.0f; else if (t1 > duration) t1 = duration;
    Key a = Sample(*track, t0), b = Sample(*track, t1);
    M s, e; ToMatrix(a.R, s); ToMatrix(b.R, e);
    d.E11 = s.E11 * e.E11 + s.E21 * e.E21 + s.E31 * e.E31; d.E12 = s.E11 * e.E12 + s.E21 * e.E22 + s.E31 * e.E32;
    d.E13 = s.E11 * e.E13 + s.E21 * e.E23 + s.E31 * e.E33; d.E21 = s.E12 * e.E11 + s.E22 * e.E21 + s.E32 * e.E31;
    d.E22 = s.E12 * e.E12 + s.E22 * e.E22 + s.E32 * e.E32; d.E23 = s.E12 * e.E13 + s.E22 * e.E23 + s.E32 * e.E33;
    d.E31 = s.E13 * e.E11 + s.E23 * e.E21 + s.E33 * e.E31; d.E32 = s.E13 * e.E12 + s.E23 * e.E22 + s.E33 * e.E32;
    d.E33 = s.E13 * e.E13 + s.E23 * e.E23 + s.E33 * e.E33;
    V m = { b.P.X - a.P.X, b.P.Y - a.P.Y, b.P.Z - a.P.Z };
    if (duration > 0.0f) { float f = (t1 - t0) / duration; m.X += f * mv.X; m.Y += f * mv.Y; m.Z += f * mv.Z; }
    d.E41 = m.X * s.E11 + m.Y * s.E12 + m.Z * s.E13;
    d.E42 = m.X * s.E21 + m.Y * s.E22 + m.Z * s.E23;
    d.E43 = m.X * s.E31 + m.Y * s.E32 + m.Z * s.E33;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    bool pass = true;
    M d; V mv = { 0.0f, 0.0f, 3.0f };
    GetDeltaMovement(0, 31, 30.0f, mv, d, 0.2f, 0.8f);
    pass = pass && d.E11 == 1.0f && d.E22 == 1.0f && d.E33 == 1.0f && d.E12 == 0.0f && d.E41 == 0.0f;
    // start: no rotation at the origin; end: 90 degrees about Z, moved by (1, 2, 0); 31 frames at 30/s -> 1 second
    Track t; float h = std::sqrt(0.5f);
    t.K0.R.X = 0; t.K0.R.Y = 0; t.K0.R.Z = 0; t.K0.R.W = 1; t.K0.P.X = 0; t.K0.P.Y = 0; t.K0.P.Z = 0;
    t.K1.R.X = 0; t.K1.R.Y = 0; t.K1.R.Z = h; t.K1.R.W = h; t.K1.P.X = 1; t.K1.P.Y = 2; t.K1.P.Z = 0;
    t.T0 = 0.0f; t.T1 = 1.0f;
    GetDeltaMovement(&t, 31, 30.0f, mv, d, -5.0f, 9.0f);                 // both times clamped to [0, 1]
    pass = pass && Near(d.E11, 0.0f) && Near(d.E12, -1.0f) && Near(d.E21, 1.0f) && Near(d.E22, 0.0f) && Near(d.E33, 1.0f);
    pass = pass && Near(d.E41, 1.0f) && Near(d.E42, 2.0f) && Near(d.E43, 3.0f);   // position delta + whole movement vector
    // a start already rotated: the translation is expressed in the start frame
    t.K0.R.Z = h; t.K0.R.W = h; t.K0.P.X = 0;
    GetDeltaMovement(&t, 31, 30.0f, mv, d, 0.0f, 0.5f);
    pass = pass && Near(d.E11, 1.0f) && Near(d.E22, 1.0f) && Near(d.E43, 1.5f);
    pass = pass && Near(d.E41, -1.0f) && Near(d.E42, 0.5f);         // (0.5, 1, 1.5) through the transposed start rotation
    if (!pass) { std::printf("00a4e300_TEST FAIL\n"); return 1; }
    std::printf("00a4e300_TEST PASS\n");
    return 0;
}