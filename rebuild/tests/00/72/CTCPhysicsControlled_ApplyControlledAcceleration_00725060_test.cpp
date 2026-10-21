// Behaviour test for CTCPhysicsControlled::ApplyControlledAcceleration (0x00725060): below the speed limit apply
// 0.05 * (a - a * dot(a, v)); otherwise rotate the velocity about +Z by AngleDifference clamped to +-0.007 of a turn.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static float Wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
static float AngleDiff(float from, float to) { float d = to - from; if (d > 0.5f) d -= 1.0f; if (d < -0.5f) d += 1.0f; return d; }
struct Phys { V vel; V applied; float turned; };
static void Apply(Phys& p, const V& a, float maxSpeed)
{
    float speed = (float)std::sqrt(p.vel.X * p.vel.X + p.vel.Y * p.vel.Y + p.vel.Z * p.vel.Z);
    if (speed < maxSpeed) {
        float d = a.X * p.vel.X + (a.Y * p.vel.Y + a.Z * p.vel.Z);
        p.applied.X = (a.X - a.X * d) * 0.05f; p.applied.Y = (a.Y - a.Y * d) * 0.05f; p.applied.Z = (a.Z - a.Z * d) * 0.05f;
    } else {
        float aa = Wrap((float)std::atan2(a.X, a.Y) * 0.15915494f);
        float va = Wrap((float)std::atan2(p.vel.X, p.vel.Y) * 0.15915494f);
        float t = AngleDiff(va, aa);
        if (t < -0.007f) t = -0.007f; else if (t > 0.007f) t = 0.007f;
        p.turned = t;
    }
}
int main()
{
    Phys p; p.vel.X = 0.0f; p.vel.Y = 1.0f; p.vel.Z = 0.0f; p.applied.X = p.applied.Y = p.applied.Z = 0.0f; p.turned = 0.0f;
    V a; a.X = 1.0f; a.Y = 0.5f; a.Z = 0.0f;
    Apply(p, a, 2.0f);                                   // d = 0.5: applied = (0.025, 0.0125, 0)
    if (std::fabs(p.applied.X - 0.025f) > 1e-6f || std::fabs(p.applied.Y - 0.0125f) > 1e-6f || p.turned != 0.0f) {
        std::printf("00725060_TEST FAIL\n"); return 1;
    }
    Apply(p, a, 0.5f);                                   // too fast: turn toward a, clamped
    if (std::fabs(p.turned - 0.007f) > 1e-6f) { std::printf("00725060_TEST FAIL\n"); return 1; }
    V b; b.X = -1.0f; b.Y = 0.0f; b.Z = 0.0f; Apply(p, b, 0.5f);
    if (std::fabs(p.turned + 0.007f) > 1e-6f) { std::printf("00725060_TEST FAIL\n"); return 1; }
    std::printf("00725060_TEST PASS\n");
    return 0;
}