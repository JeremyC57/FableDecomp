// Behaviour test for NThingShot::CTCShotDefinitionProjectile::InitMovement (0x007BBB3B), g++ model.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static V sub(V a, V b) { V r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V norm(V v) { float l = std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); V r = { v.X / l, v.Y / l, v.Z / l }; return r; }
static V cross(V a, V b) { V r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
struct Physics { V Position; V Up, Forward; V Accel; bool Gravity; };
static int g_seedCalls, g_lastAccuracy;
static V Randomise(V d, long accuracy, unsigned long*) { ++g_seedCalls; g_lastAccuracy = (int)accuracy; return d; }
static void InitMovement(Physics& p, V target, float accuracyPct, float velocity)
{
    V direction = sub(target, p.Position);
    long accuracy = (long)accuracyPct;
    unsigned long seed = 0;
    direction = norm(Randomise(direction, accuracy, &seed));
    V perp = { direction.Y, -direction.X, 0.0f };
    V up = norm(cross(direction, perp));
    p.Up = up; p.Forward = direction;
    V a = { direction.X * velocity, direction.Y * velocity, direction.Z * velocity };
    p.Accel = a; p.Gravity = false;
}
int main()
{
    Physics p = { { 1, 2, 3 }, {}, {}, {}, true };
    V target = { 4, 6, 3 };
    InitMovement(p, target, 75.9f, 10.0f);
    int ok = 1;
    ok &= std::fabs(p.Forward.X - 0.6f) < 1e-5f && std::fabs(p.Forward.Y - 0.8f) < 1e-5f && std::fabs(p.Forward.Z) < 1e-6f;
    ok &= std::fabs(p.Accel.X - 6.0f) < 1e-4f && std::fabs(p.Accel.Y - 8.0f) < 1e-4f;
    ok &= g_lastAccuracy == 75 && g_seedCalls == 1 && !p.Gravity;
    float dot = p.Up.X * p.Forward.X + p.Up.Y * p.Forward.Y + p.Up.Z * p.Forward.Z;
    ok &= std::fabs(dot) < 1e-5f && std::fabs(std::fabs(p.Up.Z) - 1.0f) < 1e-5f;
    std::printf(ok ? "007bbb3b_TEST PASS\n" : "007bbb3b_TEST FAIL\n");
    return ok ? 0 : 1;
}
