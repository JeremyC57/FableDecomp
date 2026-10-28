// Behaviour test for 0x00A145C0: C3DVector::GetRandomUnitVector -- z uniform in [-1, 1), a random bearing on the circle of
// radius sqrt(1 - z*z), cosine/sine from the 1025-entry table with linear interpolation (round-to-nearest index).
#include <cstdio>
#include <math.h>

static float g_CosTable[1025];
static long FloatToLong(float value) { return (long)floor(value + 0.5f); }   // the FPU's round-to-nearest
static float GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
static float GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
static unsigned int g_next;
static float GFFloatRandom(float range, unsigned long* seed)          // a deterministic stand-in
{
    (void)seed;
    g_next = g_next * 1103515245u + 12345u;
    return range * (float)((g_next >> 8) & 0xffff) / 65536.0f;
}
class C3DVector {
public:
    float X, Y, Z;
    static C3DVector GetRandomUnitVector(unsigned long* seed)
    {
        C3DVector result;
        result.Z = GFFloatRandom(2.0f, seed) - 1.0f;
        float r = (float)sqrt(1.0f - result.Z * result.Z);
        float angle = GFFloatRandom(1.0f, seed);
        result.X = GFFastCos(angle) * r;
        result.Y = GFFastSin(angle) * r;
        return result;
    }
};

int main()
{
    for (int k = 0; k <= 1024; ++k)
        g_CosTable[k] = (float)cos(6.283185307179586 * k / 1024.0);
    unsigned long seed = 1;
    g_next = 7;
    for (int n = 0; n < 1000; ++n) {
        C3DVector v = C3DVector::GetRandomUnitVector(&seed);
        float m = (float)sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
        if (fabs(m - 1.0f) > 0.002f || v.Z < -1.0f || v.Z >= 1.0f) { std::printf("00a145c0_TEST FAIL\n"); return 1; }
    }
    std::printf("00a145c0_TEST PASS\n");
    return 0;
}