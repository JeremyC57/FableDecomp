#include <cstdio>
// Behaviour test for 00f35e00 CNoise::InitPerlinNoise: 512 gradient draws (2 per entry, range 2 minus 1) each normalised, a
// shuffle that leaves P a permutation of 0..255 driven by 255 LCG/ROR13 steps of the seed, and both upper halves duplicated.
static int g_normalised = 0;
static int g_floatCalls = 0;
float GFFloatRandom(float range, unsigned long* seed) { ++g_floatCalls; *seed += 1; return range * 0.75f; }
long GFROR13(unsigned long value) { value &= 0xffffffffUL; return (long)(((value >> 13) | (value << 19)) & 0xffffffffUL); }
class C2DVector { public:
    float X, Y;
    C2DVector& Normalise() { ++g_normalised; X = 0.6f; Y = 0.8f; return *this; }
};
class CNoise {
public:
    void InitPerlinNoise();
    long          P[512];                          // +0x0000
    C2DVector     G2[512];                         // +0x0800
    unsigned long Seed;                            // +0x1800
};

void CNoise::InitPerlinNoise()
{
    unsigned long i;
    for (i = 0; i < 256; ++i) {
        P[i] = i;
        G2[i].X = GFFloatRandom(2.0f, &Seed) - 1.0f;
        G2[i].Y = GFFloatRandom(2.0f, &Seed) - 1.0f;
        G2[i].Normalise();
    }
    while (--i) {
        long k = P[i];
        Seed = Seed * 0x24A1 + 0x24DF;
        Seed = GFROR13(Seed);
        unsigned long j = Seed & 0xff;
        P[i] = P[j];
        P[j] = k;
    }
    for (i = 0; i < 256; ++i) {
        P[256 + i] = P[i];
        G2[256 + i] = G2[i];
    }
}

int main()
{
    static CNoise n;
    n.Seed = 12345;
    n.InitPerlinNoise();
    bool ok = g_floatCalls == 512 && g_normalised == 256;
    int seen[256] = { 0 };
    for (int i = 0; i < 256; ++i) { if (n.P[i] < 0 || n.P[i] > 255) ok = false; else ++seen[n.P[i]]; }
    for (int i = 0; i < 256; ++i) ok = ok && seen[i] == 1;
    for (int i = 0; i < 256; ++i) ok = ok && n.P[256 + i] == n.P[i] && n.G2[256 + i].X == n.G2[i].X && n.G2[256 + i].Y == n.G2[i].Y;
    ok = ok && n.G2[7].X == 0.6f && n.G2[7].Y == 0.8f;
    // the seed: +512 from the float draws, then 255 generator steps
    unsigned long seed = (12345UL + 512UL) & 0xffffffffUL;
    for (int k = 0; k < 255; ++k) { seed = (seed * 0x24A1 + 0x24DF) & 0xffffffffUL; seed = (unsigned long)GFROR13(seed) & 0xffffffffUL; }
    ok = ok && (n.Seed & 0xffffffffUL) == seed;
    bool moved = false;
    for (int i = 0; i < 256; ++i) if (n.P[i] != i) moved = true;
    ok = ok && moved;
    if (!ok) { std::printf("00f35e00_TEST FAIL\n"); return 1; }
    std::printf("00f35e00_TEST PASS\n");
    return 0;
}