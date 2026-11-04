// Behaviour test for NParticleEngine::CPSCRenderSprite::InterpolateTo (0x00AF9840): a standalone model of the blend -- bank indices,
// Flags and the four blend fields take the target's value only past t = 0.5; every packed setting (an unsigned bit-field) is lerped as
// (1 - t) * a + b * t and rounded to nearest (GFFloatToLongNear), then truncated to its field width; the three colours blend per channel
// with the weight round(t * 256) (GFGetInterpolatedColour).
#include <cstdio>
#include <cmath>
// round half to even, the FPU default the retail fistp uses (portable: no C99 nearbyint in VC7.1)
static double RoundEven(double v)
{
    double f = std::floor(v), d = v - f;
    if (d > 0.5 || (d == 0.5 && std::fmod(f, 2.0) != 0.0)) f += 1.0;
    return f;
}
static unsigned long LerpU(unsigned long a, unsigned long b, float t, int bits)
{
    long v = (long)RoundEven((1.0f - t) * a + b * t);
    return (unsigned long)v & ((1ul << bits) - 1);
}
static unsigned long LerpC(unsigned long a, unsigned long b, float t)
{
    long w = (long)RoundEven(t * 256.0f); unsigned long r = 0;
    for (int s = 0; s < 32; s += 8) { long ca = (a >> s) & 255, cb = (b >> s) & 255; r |= (unsigned long)(((ca * (256 - w) + cb * w) >> 8) & 255) << s; }
    return r;
}
struct Sprite {
    long SpriteBankIndex, TrailBankIndex; unsigned long StartColour, Flags, BlendMode, StartRenderSize, TrailWidth, FlickerBias;
    unsigned long AlphaFadeEnable;
    void InterpolateTo(const Sprite& o, float t)
    {
        SpriteBankIndex = t > 0.5f ? o.SpriteBankIndex : SpriteBankIndex;
        TrailBankIndex = t > 0.5f ? o.TrailBankIndex : TrailBankIndex;
        Flags = t > 0.5f ? o.Flags : Flags;
        BlendMode = t > 0.5f ? o.BlendMode : BlendMode;
        StartRenderSize = LerpU(StartRenderSize, o.StartRenderSize, t, 11);
        FlickerBias = LerpU(FlickerBias, o.FlickerBias, t, 8);
        AlphaFadeEnable = LerpU(AlphaFadeEnable, o.AlphaFadeEnable, t, 1);
        StartColour = LerpC(StartColour, o.StartColour, t);
        TrailWidth = LerpU(TrailWidth, o.TrailWidth, t, 10);
    }
};
int main()
{
    Sprite a = { 3, 0, 0x00000000, 0x041, 2, 100, 50, 0, 0 };
    Sprite b = { 9, 4, 0xFF00FF00, 0x080, 5, 300, 150, 255, 1 };
    Sprite c = a; c.InterpolateTo(b, 0.25f);
    bool ok = c.SpriteBankIndex == 3 && c.TrailBankIndex == 0 && c.Flags == 0x041 && c.BlendMode == 2 && c.StartRenderSize == 150
        && c.TrailWidth == 75 && c.FlickerBias == 64 && c.AlphaFadeEnable == 0 && c.StartColour == 0x3F003F00;
    Sprite d = a; d.InterpolateTo(b, 0.75f);
    ok = ok && d.SpriteBankIndex == 9 && d.TrailBankIndex == 4 && d.Flags == 0x080 && d.BlendMode == 5 && d.StartRenderSize == 250
        && d.TrailWidth == 125 && d.FlickerBias == 191 && d.AlphaFadeEnable == 1 && d.StartColour == 0xBF00BF00;
    Sprite e = a; e.InterpolateTo(b, 0.5f);
    ok = ok && e.SpriteBankIndex == 3 && e.BlendMode == 2 && e.StartRenderSize == 200;
    printf(ok ? "00af9840_TEST PASS\n" : "00af9840_TEST FAIL\n");
    return ok ? 0 : 1;
}