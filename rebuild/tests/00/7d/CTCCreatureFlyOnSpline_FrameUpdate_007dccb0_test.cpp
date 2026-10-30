// Behaviour test for 0x007DCCB0 CTCCreatureFlyOnSpline::FrameUpdate (see src/007dccb0.cpp): no spline does nothing; otherwise
// progress += delta (looping keeps the fractional part and restarts the previous sample there), clamped to [0, 1]; the creature
// moves to the spline position at the new progress and, when the travel from the previous sample is longer than epsilon, is
// oriented along the normalised direction with up = (dY, -dX, 0) x d.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static inline V Cross(const V& a, const V& b) { return V(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
static V SplineAt(float t) { return V(10.0f * t, 0.0f, 0.0f); }
struct Fly {
    bool HasSpline, Looping; float Progress, Delta; V Pos, Up, Fwd; int RHSets;
    void FrameUpdate()
    {
        if (!HasSpline) return;
        float last = Progress;
        Progress += Delta;
        if (Looping) { Progress -= (long)Progress; last = Progress; }
        if (Progress < 0.0f) Progress = 0.0f; else if (Progress > 1.0f) Progress = 1.0f;
        V lp = SplineAt(last), p = SplineAt(Progress);
        Pos = p;
        V d(p.X - lp.X, p.Y - lp.Y, p.Z - lp.Z);
        if (d.X * d.X + d.Y * d.Y + d.Z * d.Z > 1e-6f) {
            float l = std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z); d.X /= l; d.Y /= l; d.Z /= l;
            Up = Cross(V(d.Y, -d.X, 0.0f), d); Fwd = d; ++RHSets;
        }
    }
};
int main()
{
    Fly f = { true, false, 0.2f, 0.5f, V(0, 0, 0), V(0, 0, 0), V(0, 0, 0), 0 };
    f.FrameUpdate();
    bool ok = std::fabs(f.Progress - 0.7f) < 1e-6f && std::fabs(f.Pos.X - 7.0f) < 1e-5f && f.RHSets == 1 && f.Fwd.X == 1.0f && f.Up.Z == 1.0f;
    f.FrameUpdate();
    ok = ok && f.Progress == 1.0f && f.RHSets == 2;
    f.Looping = true; f.Progress = 0.9f; f.FrameUpdate();
    ok = ok && std::fabs(f.Progress - 0.4f) < 1e-5f && f.RHSets == 2;
    f.HasSpline = false; f.FrameUpdate();
    ok = ok && f.RHSets == 2;
    std::printf(ok ? "007dccb0_TEST PASS\n" : "007dccb0_TEST FAIL\n");
    return ok ? 0 : 1;
}