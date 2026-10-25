// Behaviour test for 0x00CBE693 GetRandomPositionInsideRadius: draw a length in [0, radius) and then a bearing in [0, 1)
// turns from the script random seed, and return centre + (sin(2pi b) * len, cos(2pi b) * len, 0) (the
// InitialiseFromAngles(bearing, 0, len) polar helper with a zero pitch).
#include <cstdio>
#include <cmath>

static unsigned long g_seed = 0;
static float g_draws[4];
static unsigned int g_drawIndex = 0;
static float g_ranges[4];

static float FloatRandom(float range, unsigned long* seed)
{
    ++*seed;
    g_ranges[g_drawIndex] = range;
    return g_draws[g_drawIndex++] * range;
}

struct Vec {
    float X, Y, Z;
    Vec() { }
    Vec(float x, float y, float z) : X(x), Y(y), Z(z) { }
    Vec operator+(const Vec& v) const { return Vec(X + v.X, Y + v.Y, Z + v.Z); }
    const Vec& InitialiseFromAngles(float angleXY, float angleYZ, float length)
    {
        const double twoPi = 6.2831854820251465;
        float cosYZ = (float)std::cos(angleYZ * twoPi);
        X = (float)(std::sin(angleXY * twoPi) * (cosYZ * length));
        Y = (float)(std::cos(angleXY * twoPi) * (cosYZ * length));
        Z = (float)(std::sin(angleYZ * twoPi) * length);
        return *this;
    }
};

static Vec GetRandomPositionInsideRadius(const Vec& centre, float radius)
{
    float length = FloatRandom(radius, &g_seed);
    Vec offset;
    offset.InitialiseFromAngles(FloatRandom(1.0f, &g_seed), 0.0f, length);
    return offset + centre;
}

static int near_(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int ok = 1;
    Vec centre(10.0f, 20.0f, 5.0f);
    g_draws[0] = 0.5f; g_draws[1] = 0.25f;                     // length 2 of radius 4, a quarter turn
    Vec p = GetRandomPositionInsideRadius(centre, 4.0f);
    if (g_ranges[0] != 4.0f || g_ranges[1] != 1.0f || g_seed != 2) ok = 0;   // radius first, then the bearing
    if (!near_(p.X, 12.0f) || !near_(p.Y, 20.0f) || !near_(p.Z, 5.0f)) ok = 0;
    g_drawIndex = 0; g_draws[0] = 1.0f; g_draws[1] = 0.5f;     // full radius 3, half a turn
    p = GetRandomPositionInsideRadius(centre, 3.0f);
    if (!near_(p.X, 10.0f) || !near_(p.Y, 17.0f) || !near_(p.Z, 5.0f)) ok = 0;
    g_drawIndex = 0; g_draws[0] = 0.0f; g_draws[1] = 0.125f;   // zero length stays at the centre
    p = GetRandomPositionInsideRadius(centre, 6.0f);
    if (!near_(p.X, 10.0f) || !near_(p.Y, 20.0f) || !near_(p.Z, 5.0f)) ok = 0;
    std::printf(ok ? "00cbe693_TEST PASS\n" : "00cbe693_TEST FAIL\n");
    return ok ? 0 : 1;
}