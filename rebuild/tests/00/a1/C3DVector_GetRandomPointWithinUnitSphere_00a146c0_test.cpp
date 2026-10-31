// Behaviour test for C3DVector::GetRandomPointWithinUnitSphere (0x00A146C0): a copy of the model with a C++ GFROR13
// (rotate right by 13) stand-in; checks the point lies inside the unit sphere, components in [-1, 1), and that the seed
// advances exactly as the reference LCG + rotate sequence does.
#include <cstdio>
#include <cmath>

static unsigned long ror13(unsigned long v) { v &= 0xFFFFFFFFUL; return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }
static long GFROR13(unsigned long value) { return (long)ror13(value); }

static float GFFloatRandom(float range, unsigned long* seed)
{
    double scaled = (double)range;
    *seed = (*seed * 0x24A1 + 0x24DF) & 0xFFFFFFFFUL;
    scaled *= 65536.0;
    *seed = (unsigned long)GFROR13(*seed) & 0xFFFFFFFFUL;
    if (scaled > 0.0001) {
        double result = std::fmod((double)*seed, scaled);
        return (float)(result * (1.0 / 65536.0));
    }
    return 0.0f;
}

class C3DVector { public:
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    static C3DVector GetRandomPointWithinUnitSphere(unsigned long* seed)
    {
        C3DVector point;
        do {
            point.X = GFFloatRandom(2.0f, seed) - 1.0f;
            point.Y = GFFloatRandom(2.0f, seed) - 1.0f;
            point.Z = GFFloatRandom(2.0f, seed) - 1.0f;
        } while (point.GetLengthSquared() > 1.0f);
        return point;
    }
};

int main()
{
    unsigned long seed = 12345;
    for (int i = 0; i < 200; ++i) {
        unsigned long before = seed;
        C3DVector p = C3DVector::GetRandomPointWithinUnitSphere(&seed);
        if (p.GetLengthSquared() > 1.0f || p.X < -1.0f || p.X >= 1.0f || p.Y < -1.0f || p.Y >= 1.0f || p.Z < -1.0f || p.Z >= 1.0f) {
            std::printf("00a146c0_TEST FAIL\n");
            return 1;
        }
        // the seed advanced by a multiple of three LCG+rotate steps
        unsigned long s = before;
        int steps = 0;
        while (s != seed && steps < 3000) { s = ror13((s * 0x24A1 + 0x24DF) & 0xFFFFFFFFUL); ++steps; }
        if (s != seed || steps % 3 != 0 || steps == 0) {
            std::printf("00a146c0_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00a146c0_TEST PASS\n");
    return 0;
}