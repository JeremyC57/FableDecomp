// Behaviour test for 0x006AB820 CThingPlayerCreature::ResolveFacingDirection (host model of the row's logic; plain C++98).
// Scenarios: action preventing movement, PC third-person aiming camera, first-person targeting, z-targeting with and without a
// locked target, free movement (turn limited by the max turn speed, lean smoothing and limit), movement-band stop rules.
#include <cstdio>
#include <math.h>

struct Vec { float X, Y, Z; Vec() : X(0), Y(0), Z(0) {} Vec(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static Vec Sub(const Vec& a, const Vec& b) { return Vec(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static float Dot(const Vec& a, const Vec& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static Vec Cross(const Vec& a, const Vec& b) { return Vec(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
static float SqMag(const Vec& v) { return Dot(v, v); }
static void Normalise(Vec& v) { float m = (float)sqrt(SqMag(v)); v.X /= m; v.Y /= m; v.Z /= m; }
static const float EPS = 1e-4f;
static const double TWO_PI_D = 6.2831854820251465;
static float Wrap01(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
static float AngleXY(const Vec& v) { return Wrap01((float)atan2(v.X, v.Y) * 0.15915494f); }
static float AngleXYTo(const Vec& from, const Vec& to) { return Wrap01((float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f); }
static float RoundedSigned(float a)
{
    if (a < -0.5f) return (float)fmod((double)(a - 0.5f), 1.0) + 0.5f;
    if (a >= 0.5f) return (float)fmod((double)(a + 0.5f), 1.0) - 0.5f;
    return a;
}
// Rotation of v about a unit axis by `turns` (the engine's C3DVector::Rotate, angle in turns; a positive turn about +Z increases
// the XY heading atan2(X, Y), i.e. turns clockwise seen from above).
static Vec Rotate(const Vec& v, const Vec& axis, float turns)
{
    double a = -turns * TWO_PI_D, c = cos(a), s = sin(a);
    Vec k = axis; Vec kxv = Cross(k, v); double kd = Dot(k, v);
    return Vec((float)(v.X * c + kxv.X * s + k.X * kd * (1 - c)), (float)(v.Y * c + kxv.Y * s + k.Y * kd * (1 - c)),
               (float)(v.Z * c + kxv.Z * s + k.Z * kd * (1 - c)));
}
static void SmoothTowards(float target, float max_step, float& current)
{
    float step = target - current;
    if (step < -max_step) step = -max_step; else if (step > max_step) step = max_step;
    current += step;
    if (step > 0) { if (current > target) current = target; } else { if (current < target) current = target; }
}

struct Physics {
    Vec Up, Forward, Position, OldPosition; int SetCount;
    Physics() : Up(0, 0, 1), Forward(0, 1, 0), SetCount(0) {}
    Vec GetFacingVector() const { return Forward; }
    float GetFacingAngleXY() const { return AngleXY(Forward); }
    void SetRHSet(const Vec& up, const Vec& forward) { Up = up; Forward = forward; ++SetCount; }
};
struct Target { bool HasGraphic; Vec Focal, Pos; };
struct World {
    bool ActionPreventing, Sneaking, PCAiming, FirstPerson, ZTargeting, HasCamera;
    Vec CameraView, TargetedPos, Pos; Target* Locked;
    float MaxTurnDegrees, LeanTurnFactor, LeanLimit, LeanStep;
    World() : ActionPreventing(false), Sneaking(false), PCAiming(false), FirstPerson(false), ZTargeting(false), HasCamera(true),
              Locked(0), MaxTurnDegrees(36.0f), LeanTurnFactor(90.0f), LeanLimit(18.0f), LeanStep(3.6f) {}
};
struct Player {
    Physics Phys; World W; Vec MovementAcceleration; float LeanAngle; int MovementBandType;
    Player() : LeanAngle(0.5f), MovementBandType(0) {}
    void ResolveFacingDirection();
};

void Player::ResolveFacingDirection()
{
    const Vec up(0, 0, 1);
    if (W.ActionPreventing) { LeanAngle = 0.0f; Phys.SetRHSet(up, Phys.GetFacingVector()); return; }
    bool sneaking = W.Sneaking;
    if (W.PCAiming) {
        if (W.HasCamera) {
            Vec d = W.CameraView; d.Z = 0.0f;
            if (d.X * d.X + d.Y * d.Y > EPS * EPS) { Normalise(d); Phys.SetRHSet(up, d); }
        }
        LeanAngle = 0.0f; return;
    } else if (W.FirstPerson) {
        Vec d = Sub(W.TargetedPos, W.Pos); d.Z = 0.0f;
        if (d.X * d.X + d.Y * d.Y > EPS * EPS) { Normalise(d); Phys.SetRHSet(up, d); }
        LeanAngle = 0.0f; return;
    } else if (W.ZTargeting) {
        if (W.Locked) {
            Vec tp = W.Locked->HasGraphic ? W.Locked->Focal : W.Locked->Pos;
            float a = AngleXYTo(W.Pos, tp);
            // InitialiseFromAngles(a, 0, 1): cosYZ = GFCos(0); X = GFSin(a) * (cosYZ * 1); Y = GFCos(a) * (cosYZ * 1); Z = GFSin(0) * 1
            float cos_yz = (float)cos(0.0f * TWO_PI_D);
            Vec d((float)(sin(a * TWO_PI_D) * (cos_yz * 1.0f)), (float)(cos(a * TWO_PI_D) * (cos_yz * 1.0f)), (float)(sin(0.0f * TWO_PI_D) * 1.0f));
            Phys.SetRHSet(up, d);
        } else
            Phys.SetRHSet(up, Phys.GetFacingVector());
        LeanAngle = 0.0f; return;
    }
    if (SqMag(MovementAcceleration) > EPS * EPS) {
        Vec facing = Phys.GetFacingVector();
        Vec d(MovementAcceleration.X, MovementAcceleration.Y, 0.0f);
        if (SqMag(d) > EPS * EPS) Normalise(d);
        float c = Dot(facing, d);
        if (c < -1.0f) c = -1.0f; else if (c > 1.0f) c = 1.0f;
        float angle = (float)acos(c) * 0.15915494f;
        float turn = ((Cross(facing, d).Z > 0.0f) ? -1.0f : 1.0f) * angle;
        float desired = -turn;
        float limit = W.MaxTurnDegrees / 360.0f;
        if (turn < -limit) turn = -limit; else if (turn > limit) turn = limit;
        float actual = -turn;
        facing = Rotate(facing, up, turn);
        Phys.SetRHSet(up, facing);
        float lean_target = 0.0f;
        if (!sneaking && (MovementBandType == 4 || MovementBandType == 3) && desired == actual && fabs(actual) > 4.0f / 360.0f)
            lean_target = (float)sqrt(SqMag(MovementAcceleration)) * actual / (W.LeanTurnFactor / 360.0f) * 0.45f;
        float step = W.LeanStep / 360.0f;
        if (SqMag(Sub(Phys.Position, Phys.OldPosition)) < 0.05f) { LeanAngle = 0.0f; Phys.SetRHSet(up, Phys.GetFacingVector()); }
        else SmoothTowards(lean_target, step, LeanAngle);
        float lean_limit = W.LeanLimit / 360.0f;
        if (LeanAngle < -lean_limit) LeanAngle = -lean_limit; else if (LeanAngle > lean_limit) LeanAngle = lean_limit;
        if (fabs(LeanAngle) > EPS) { Vec f = Phys.GetFacingVector(); Phys.SetRHSet(Rotate(up, f, LeanAngle), f); }
    } else {
        LeanAngle = 0.0f; Phys.SetRHSet(up, Phys.GetFacingVector());
    }
    if (MovementBandType == 1)
        MovementAcceleration = Vec();
    else if (sneaking || MovementBandType == 2) {
        float facing_angle = Phys.GetFacingAngleXY();
        if (fabs(RoundedSigned(facing_angle - AngleXY(MovementAcceleration))) > 1.0f / 360.0f)
            MovementAcceleration = Vec();
    }
}

static bool Near(float a, float b) { return fabs(a - b) < 1e-3f; }
static int fail(int n) { std::printf("006ab820_TEST FAIL (%d)\n", n); return 1; }

int main()
{
    { Player p; p.W.ActionPreventing = true; p.ResolveFacingDirection();
      if (p.LeanAngle != 0.0f || p.Phys.SetCount != 1 || !Near(p.Phys.Forward.Y, 1.0f)) return fail(1); }
    { Player p; p.W.PCAiming = true; p.W.CameraView = Vec(3, 0, 5); p.ResolveFacingDirection();
      if (!Near(p.Phys.Forward.X, 1.0f) || !Near(p.Phys.Forward.Z, 0.0f) || p.LeanAngle != 0.0f) return fail(2); }
    { Player p; p.W.PCAiming = true; p.W.HasCamera = false; p.ResolveFacingDirection();
      if (p.Phys.SetCount != 0 || p.LeanAngle != 0.0f) return fail(3); }
    { Player p; p.W.FirstPerson = true; p.W.Pos = Vec(1, 1, 0); p.W.TargetedPos = Vec(1, -4, 9); p.ResolveFacingDirection();
      if (!Near(p.Phys.Forward.Y, -1.0f) || p.Phys.SetCount != 1) return fail(4); }
    { Player p; Target t; t.HasGraphic = true; t.Focal = Vec(10, 0, 3); t.Pos = Vec(0, -10, 0);
      p.W.ZTargeting = true; p.W.Locked = &t; p.ResolveFacingDirection();
      if (!Near(p.Phys.Forward.X, 1.0f) || !Near(p.Phys.Forward.Y, 0.0f) || p.LeanAngle != 0.0f) return fail(5);
      t.HasGraphic = false; p.ResolveFacingDirection();
      if (!Near(p.Phys.Forward.Y, -1.0f)) return fail(6); }
    { Player p; p.W.ZTargeting = true; p.ResolveFacingDirection();
      if (p.Phys.SetCount != 1 || !Near(p.Phys.Forward.Y, 1.0f) || p.LeanAngle != 0.0f) return fail(7); }
    { Player p; p.ResolveFacingDirection();                                   // no acceleration: upright, lean cleared
      if (p.LeanAngle != 0.0f || p.Phys.SetCount != 1) return fail(8); }
    { Player p; p.MovementAcceleration = Vec(5, 0, 0); p.Phys.Position = Vec(1, 0, 0);   // 90 degree turn wanted, 36 allowed
      p.MovementBandType = 4; p.LeanAngle = 0.0f; p.ResolveFacingDirection();
      float a = AngleXY(p.Phys.Forward);
      if (!Near(a, 36.0f / 360.0f)) return fail(9);
      if (p.LeanAngle != 0.0f) return fail(10);                              // clamped turn: no lean target, stays upright
      if (p.MovementAcceleration.X != 5.0f) return fail(11);
      p.LeanAngle = 0.5f; p.ResolveFacingDirection();                        // lean eases back by one step per frame
      if (!Near(p.LeanAngle, 18.0f / 360.0f)) return fail(16); }
    { Player p; p.MovementAcceleration = Vec(5, 0, 0); p.MovementBandType = 1; p.ResolveFacingDirection();
      if (p.MovementAcceleration.X != 0.0f || p.LeanAngle != 0.0f) return fail(12); }
    { Player p; p.MovementAcceleration = Vec(5, 0, 0); p.MovementBandType = 2; p.ResolveFacingDirection();
      if (p.MovementAcceleration.X != 0.0f) return fail(13); }               // still 54 degrees off: stop and turn first
    { Player p; p.MovementAcceleration = Vec(0, 5, 0); p.MovementBandType = 2; p.ResolveFacingDirection();
      if (p.MovementAcceleration.Y != 5.0f) return fail(14); }               // already facing: keep moving
    { Player p; p.MovementAcceleration = Vec(1, 2, 0); p.Phys.Position = Vec(1, 0, 0); p.MovementBandType = 3; p.LeanAngle = 0.0f;
      p.W.MaxTurnDegrees = 90.0f; p.W.LeanStep = 360.0f; p.W.LeanLimit = 360.0f; p.ResolveFacingDirection();
      float turn = (float)acos(2.0 / sqrt(5.0)) * 0.15915494f;               // unclamped turn: lean towards it
      float expect = (float)sqrt(5.0) * turn / (90.0f / 360.0f) * 0.45f;
      if (!Near(fabs(p.LeanAngle), expect)) return fail(15); }
    std::printf("006ab820_TEST PASS\n");
    return 0;
}