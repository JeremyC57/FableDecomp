// Behaviour test for NParticleEngine::CPSCRenderSprite::PreparePrimitives (0x00AFCC30): a standalone model of the per-particle maths --
// the colour ramp (colours[3] indexed by round(lifeFraction * 511) >> 8, blended by the low byte), the cosine-table smooth fade-in /
// fade-out (GFSmoothFadeValue: 0.5 - 0.5 * cos(pi * x)), the alpha / size fade ranges, the flicker clamp, the animation frame
// min(floor(fraction * frames), frames - 1) and the crossed-sprite angle (i + 0.5) * scale / count + angle.
#include <cstdio>
#include <cmath>
// round half to even, the FPU default the retail fistp uses (portable: no C99 nearbyint in VC7.1)
static double RoundEven(double v)
{
    double f = std::floor(v), d = v - f;
    if (d > 0.5 || (d == 0.5 && std::fmod(f, 2.0) != 0.0)) f += 1.0;
    return f;
}
static float Smooth(float x) { return 0.5f - 0.5f * (float)std::cos(3.14159265358979 * x); }
static unsigned Blend(unsigned a, unsigned b, unsigned w) { unsigned r = 0; for (int s = 0; s < 32; s += 8) r |= ((((a >> s) & 255) * (256 - w) + ((b >> s) & 255) * w) >> 8 & 255) << s; return r; }
static unsigned Ramp(const unsigned c[3], float f) { unsigned long i = (unsigned long)RoundEven(f * 511.0f); return Blend(c[i >> 8], c[(i >> 8) + 1], i & 255); }
static float Fade(float f, float fadeInEnd, float fadeOutBegin, float global)
{
    float fade = global;
    if (f < fadeInEnd) fade *= Smooth(f / fadeInEnd);
    if (f > fadeOutBegin) fade *= Smooth((1 - f) / (1 - fadeOutBegin));
    return fade;
}
static long Frame(float fraction, long frames) { long f = (long)std::floor(fraction * frames); return f < frames - 1 ? f : frames - 1; }
static float Clamp01(float v) { return v < 0 ? 0 : v > 1 ? 1 : v; }
int main()
{
    unsigned c[3] = { 0xFF000000, 0x80FF0000, 0x0000FF00 };
    bool ok = Ramp(c, 0.0f) == 0xFF000000 && Ramp(c, 1.0f) == Blend(c[1], c[2], 255) && Ramp(c, 0.25f) == Blend(c[0], c[1], 128) && Ramp(c, 0.5f) == c[1];
    ok = ok && std::fabs(Fade(0.05f, 0.1f, 0.9f, 1.0f) - 0.5f) < 1e-5f && std::fabs(Fade(0.5f, 0.1f, 0.9f, 0.8f) - 0.8f) < 1e-6f
        && std::fabs(Fade(0.95f, 0.1f, 0.9f, 1.0f) - 0.5f) < 1e-5f;
    float alphaMin = 0.25f, alphaScale = Fade(0.05f, 0.1f, 0.9f, 1.0f) * (1 - alphaMin) + alphaMin;
    ok = ok && std::fabs(alphaScale - 0.625f) < 1e-5f && Clamp01(1.7f) == 1.0f && Clamp01(-0.2f) == 0.0f;
    ok = ok && Frame(0.0f, 8) == 0 && Frame(0.5f, 8) == 4 && Frame(0.999f, 8) == 7 && Frame(1.0f, 8) == 7;
    float step = 1.0f / 3, angle = (1 + 0.5f) * 0.5f * step + 0.25f;
    ok = ok && std::fabs(angle - 0.5f) < 1e-6f;
    printf(ok ? "00afcc30_TEST PASS\n" : "00afcc30_TEST FAIL\n");
    return ok ? 0 : 1;
}
