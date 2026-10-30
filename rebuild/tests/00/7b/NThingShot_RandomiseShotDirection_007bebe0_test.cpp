// Behaviour test for 0x007BEBE0 NThingShot::RandomiseShotDirection (see src/007bebe0.cpp): the LCG roll (seed*0x24A1+0x24DF,
// ROR13, % 100) at or under the accuracy returns the direction unchanged; above it the axis direction x (Y,-X,0) =
// (XZ, YZ, -(X^2+Y^2)) is spun about the direction by GFFloatRandom(1), normalised, and the direction is rotated about the
// normalised axis by GFFloatRandom(1)/48 + 1/144.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} void Rotate(const V& axis, float angle); void Normalise(); };
static int g_rot = 0, g_norm = 0; static V g_axis0, g_about0, g_axis1; static float g_ang0, g_ang1;
void V::Rotate(const V& axis, float angle)
{
    if (g_rot == 0) { g_axis0 = *this; g_about0 = axis; g_ang0 = angle; }
    else { g_axis1 = axis; g_ang1 = angle; X += 100.0f; }
    ++g_rot;
}
void V::Normalise() { ++g_norm; float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
static inline V Cross(const V& a, const V& b) { return V(b.Z * a.Y - a.Z * b.Y, b.X * a.Z - a.X * b.Z, b.Y * a.X - a.Y * b.X); }
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static float g_float = 0.5f;
static float FloatRandom(float range, unsigned long*) { return g_float * range; }
static unsigned long Random(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df; *seed = Ror13(*seed); return range ? *seed % range : 0;
}
static V Randomise(const V& direction, long accuracy, unsigned long* seed)
{
    V result(direction);
    if ((long)Random(100, seed) > accuracy) {
        V axis;
        axis = Cross(result, V(result.Y, -result.X, 0.0f));
        float spin = FloatRandom(1.0f, seed);
        axis.Rotate(result, spin);
        axis.Normalise();
        float tilt = FloatRandom(1.0f, seed) * (1.0f / 48.0f) + 1.0f / 144.0f;
        result.Rotate(axis, tilt);
    }
    return result;
}
int main()
{
    V d(1.0f, 2.0f, 3.0f);
    unsigned long seed = 7, s2 = 7;
    unsigned long roll = Random(100, &s2);
    V same = Randomise(d, (long)roll, &seed);                      // roll <= accuracy: unchanged
    bool ok = g_rot == 0 && same.X == 1.0f && same.Y == 2.0f && same.Z == 3.0f && seed == s2;
    seed = 7;
    V moved = Randomise(d, (long)roll - 1, &seed);                 // roll > accuracy: two rotations
    ok = ok && g_rot == 2 && g_norm == 1 && g_axis0.X == 3.0f && g_axis0.Y == 6.0f && g_axis0.Z == -5.0f;
    ok = ok && g_about0.X == 1.0f && g_ang0 == 0.5f && std::fabs(g_ang1 - (0.5f / 48.0f + 1.0f / 144.0f)) < 1e-6f;
    ok = ok && std::fabs(g_axis1.X * g_axis1.X + g_axis1.Y * g_axis1.Y + g_axis1.Z * g_axis1.Z - 1.0f) < 1e-5f && moved.X == 101.0f;
    std::printf(ok ? "007bebe0_TEST PASS\n" : "007bebe0_TEST FAIL\n");
    return ok ? 0 : 1;
}