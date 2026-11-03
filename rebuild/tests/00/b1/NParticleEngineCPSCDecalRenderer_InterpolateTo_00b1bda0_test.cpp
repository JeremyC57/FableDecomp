// Behaviour test for 0x00B1BDA0 NParticleEngine::CPSCDecalRenderer::InterpolateTo: floats lerp a*(1-t)+b*t,
// integer fields round (1-t)*a+b*t to nearest, enum-like fields switch to the source when t > 0.5,
// colours lerp per channel with a 0..256 weight.
#include <cstdio>
#include <cmath>
static long Near(float f) { return (long)std::floor(f + 0.5f); }
static float LerpF(float a, float b, float t) { return a * (1.0f - t) + b * t; }
static unsigned long LerpU(unsigned long a, unsigned long b, float t) { return (unsigned long)Near((1.0f - t) * a + b * t); }
static unsigned char LerpC(unsigned char a, unsigned char b, unsigned long w) { return (unsigned char)((a * (256 - w) + b * w) >> 8); }
struct Decal { float Life; long Bank; unsigned long Start; unsigned char R; };
static void InterpolateTo(Decal& d, const Decal& s, float t)
{
    d.Life = LerpF(d.Life, s.Life, t);
    d.Bank = t > 0.5f ? s.Bank : d.Bank;
    d.Start = LerpU(d.Start, s.Start, t);
    d.R = LerpC(d.R, s.R, (unsigned long)Near(t * 256.0f));
}
int main()
{
    Decal a = { 2.0f, 1, 10, 0 }, b = { 4.0f, 7, 20, 200 };
    Decal x = a; InterpolateTo(x, b, 0.25f);
    Decal y = a; InterpolateTo(y, b, 0.75f);
    bool ok = std::fabs(x.Life - 2.5f) < 1e-5f && x.Bank == 1 && x.Start == 13 && x.R == 50
           && std::fabs(y.Life - 3.5f) < 1e-5f && y.Bank == 7 && y.Start == 18 && y.R == 150;
    if (!ok) { std::printf("00b1bda0_TEST FAIL\n"); return 1; }
    std::printf("00b1bda0_TEST PASS\n");
    return 0;
}