#include <cstdio>
#include <cmath>
// Behaviour test for 0121ac30: the dynamic initializer leaves g_InventoryOrientationRHSet1 = Up (sqrt(0.5), -sqrt(0.5), 0),
// Forward (-sqrt(0.5), -sqrt(0.5), 0): a unit Up, a unit Forward, perpendicular (a valid right-handed set basis pair).
struct Vec { float X, Y, Z; Vec(float x, float y, float z) : X(x), Y(y), Z(z) { } Vec(const Vec& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
struct RHSet { Vec Up; Vec Forward; RHSet(const Vec& u, const Vec& f) : Up(u), Forward(f) { } };
static RHSet g_set(Vec(std::sqrt(0.5f), -std::sqrt(0.5f), 0.0f), Vec(-std::sqrt(0.5f), -std::sqrt(0.5f), 0.0f));
static int near(float a, float b) { return std::fabs(a - b) < 1e-6f; }
int main()
{
    float s = (float)std::sqrt(0.5);
    int ok = near(g_set.Up.X, s) && near(g_set.Up.Y, -s) && near(g_set.Up.Z, 0.0f);
    ok = ok && near(g_set.Forward.X, -s) && near(g_set.Forward.Y, -s) && near(g_set.Forward.Z, 0.0f);
    ok = ok && near(g_set.Up.X * g_set.Up.X + g_set.Up.Y * g_set.Up.Y, 1.0f);
    ok = ok && near(g_set.Forward.X * g_set.Forward.X + g_set.Forward.Y * g_set.Forward.Y, 1.0f);
    ok = ok && near(g_set.Up.X * g_set.Forward.X + g_set.Up.Y * g_set.Forward.Y + g_set.Up.Z * g_set.Forward.Z, 0.0f);
    if (!ok) { std::printf("0121ac30_TEST FAIL\n"); return 1; }
    std::printf("0121ac30_TEST PASS\n");
    return 0;
}