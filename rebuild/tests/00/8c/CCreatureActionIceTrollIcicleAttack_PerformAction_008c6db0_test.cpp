// Behaviour test for 0x008C6DB0 CCreatureAction_IceTrollIcicleAttack::PerformAction (see src/008c6db0.cpp): the aim
// starts at the facing; handedness 0 aims at the target, handedness 1 leads the target by its per-frame velocity times
// the frame rate; the aim is flattened, kept when within 60 degrees of the facing (normalised), else replaced by the
// facing turned by -/+1/6 rad about Z (sign from the facing x aim Z, which is 0 after the copy: always +1/6); a
// near-zero flat aim falls back to the facing. The trail gets the def's IcicleAttackIndex and ignores the troll.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static V Sub(V a, V b) { V r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V Add(V a, V b) { V r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
static V Mul(V a, float s) { V r = { a.X * s, a.Y * s, a.Z * s }; return r; }
static void Rotate(V& v, float angle) { float c = std::cos(angle), s = std::sin(angle); V r = { v.X * c - v.Y * s, v.X * s + v.Y * c, v.Z }; v = r; }
struct Out { V pos, dir; long index; bool ignored; };
static Out Aim(long handedness, V pos, V forward, bool targetAlive, V targetPos, V targetOld, long fps, bool trailHasDriver)
{
    V direction = forward;
    if (handedness == 0) { if (targetAlive) direction = Sub(targetPos, pos); }
    else if (handedness == 1) { if (targetAlive) direction = Sub(Add(Mul(Sub(targetPos, targetOld), (float)fps), targetPos), pos); }
    direction.Z = 0.0f;
    const float eps = 1e-4f;
    if (direction.X * direction.X + direction.Y * direction.Y > eps * eps) {
        float l = std::sqrt(direction.X * direction.X + direction.Y * direction.Y);
        direction.X /= l; direction.Y /= l;
        if (forward.X * direction.X + forward.Y * direction.Y + forward.Z * direction.Z < std::cos(1.0471976f)) {
            direction = forward;
            float cross = forward.X * direction.Y - direction.X * forward.Y;
            Rotate(direction, (cross > 0.0f ? -1.0f : 1.0f) * 0.16666667f);
        }
    } else direction = forward;
    Out o = { pos, direction, 7, trailHasDriver };
    return o;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    V p = { 0, 0, 0 }, f = { 1, 0, 0 };
    V t = { 10, 1, 5 }, old = { 10, 1, 5 };
    Out a = Aim(0, p, f, true, t, old, 30, true);                 // within 60 degrees: normalised flat aim
    bool ok = Near(a.dir.X, 10 / std::sqrt(101.0f)) && Near(a.dir.Y, 1 / std::sqrt(101.0f)) && a.dir.Z == 0 && a.ignored;
    V side = { 0, 10, 0 };
    Out b = Aim(0, p, f, true, side, side, 30, false);           // 90 degrees off: facing turned +1/6 rad
    ok = ok && Near(b.dir.X, std::cos(0.16666667f)) && Near(b.dir.Y, std::sin(0.16666667f)) && !b.ignored;
    V moving = { 10, 0, 0 }, before = { 10, -0.1f, 0 };
    Out c = Aim(1, p, f, true, moving, before, 30, true);        // lead: (0,0.1)*30 + (10,0) = (10,3)
    ok = ok && Near(c.dir.X, 10 / std::sqrt(109.0f)) && Near(c.dir.Y, 3 / std::sqrt(109.0f));
    Out d = Aim(2, p, f, true, t, old, 30, true);                // other handedness: the facing
    ok = ok && Near(d.dir.X, 1) && Near(d.dir.Y, 0);
    Out e = Aim(0, p, f, false, t, old, 30, true);               // dead target: the facing
    ok = ok && Near(e.dir.X, 1) && e.index == 7;
    std::printf(ok ? "008c6db0_TEST PASS\n" : "008c6db0_TEST FAIL\n");
    return ok ? 0 : 1;
}