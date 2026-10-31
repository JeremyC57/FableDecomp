// Behaviour test for CPerlinNoise::Initialise @ 0x00F35ED0: permutation is a shuffle of 0..255, gradients in [-1, 1), deterministic.
#include <cstdio>
#include <math.h>

static long GFROR13(unsigned long v) { return (long)((v >> 13) | (v << 19)); }
static float GFFloatRandom(float range, unsigned long* seed)
{
    double scaled = (double)range;
    *seed = *seed * 0x24A1 + 0x24DF;
    scaled *= 65536.0;
    *seed = (unsigned long)GFROR13(*seed);
    if (scaled > 0.0001) {
        double result = fmod((double)*seed, scaled);
        return (float)(result * (1.0 / 65536.0));
    }
    return 0.0f;
}
class C3DVector { public: float X, Y, Z; };
struct CPerlinNoise {
    unsigned long IndexTable[256];
    C3DVector VectorTable[256];
    void Initialise(unsigned long seed)
    {
        unsigned long i;
        for (i = 0; i < 256; ++i) IndexTable[i] = i;
        for (i = 0; i < 256; ++i) {
            seed = (unsigned long)GFROR13(seed * 0x24A1 + 0x24DF);
            unsigned long swap = IndexTable[i];
            IndexTable[i] = IndexTable[seed & 0xFF];
            IndexTable[seed & 0xFF] = swap;
        }
        for (i = 0; i < 256; ++i) {
            C3DVector& v = VectorTable[i];
            v.X = GFFloatRandom(2.0f, &seed) - 1.0f;
            v.Y = GFFloatRandom(2.0f, &seed) - 1.0f;
            v.Z = GFFloatRandom(2.0f, &seed) - 1.0f;
        }
    }
};

static CPerlinNoise a, b;
int main()
{
    a.Initialise(12345); b.Initialise(12345);
    bool ok = true;
    int seen[256] = { 0 };
    for (int i = 0; i < 256; ++i) {
        if (a.IndexTable[i] > 255) ok = false; else seen[a.IndexTable[i]]++;
        const C3DVector& v = a.VectorTable[i];
        if (v.X < -1.0f || v.X >= 1.0f || v.Y < -1.0f || v.Y >= 1.0f || v.Z < -1.0f || v.Z >= 1.0f) ok = false;
        if (v.X != b.VectorTable[i].X || a.IndexTable[i] != b.IndexTable[i]) ok = false;
    }
    for (int i = 0; i < 256; ++i) if (seen[i] != 1) ok = false;
    // first shuffle step by hand: seed' = ror13(12345*0x24A1 + 0x24DF)
    unsigned long s1 = (unsigned long)GFROR13(12345UL * 0x24A1 + 0x24DF);
    CPerlinNoise c; c.Initialise(12345);
    ok = ok && (s1 & 0xFF) < 256;
    b.Initialise(777);
    int diff = 0; for (int i = 0; i < 256; ++i) diff += a.IndexTable[i] != b.IndexTable[i];
    ok = ok && diff > 0;
    if (!ok) { std::printf("00f35ed0_TEST FAIL\n"); return 1; }
    std::printf("00f35ed0_TEST PASS\n");
    return 0;
}