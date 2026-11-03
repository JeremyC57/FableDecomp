// Behaviour test for NParticleEngine::CPSCSingleSprite::InterpolateTo (0x00B062A0): a standalone model of the blend -- the
// CPSCSplineBase part (0x00B22310) is interpolated first, then the sprite settings: bank / blend / alignment indices and flags take
// the target's value only when t > 0.5, floats lerp, long integers lerp and round to nearest, colours blend per channel with the
// weight t*256; StayWithEmitterIntFactor is left alone.
#include <cstdio>
#include <cmath>
static float Lerp(float a, float b, float t) { return a * (1 - t) + b * t; }
static long LerpL(long a, long b, float t) { return (long)std::floor((1 - t) * a + b * t + 0.5f); }
static unsigned long LerpC(unsigned long a, unsigned long b, float t) {
    long w = (long)std::floor(t * 256 + 0.5f); unsigned long r = 0;
    for (int s = 0; s < 32; s += 8) { long ca = (a >> s) & 255, cb = (b >> s) & 255; r |= (unsigned long)((ca * (256 - w) + cb * w) >> 8) << s; }
    return r;
}
struct Sprite {
    int order; float splineValue;                                 // the spline base's part, and when it was blended
    long SpriteBankIndex, BlendMode, NoCrossedSprites, MaxTrailLength, StayWithEmitterIntFactor;
    float StartRenderSize, TrailWidth; bool FaceMe2D, UseSplinePoints; unsigned long StartColour;
    int counter;
    void SplineInterpolateTo(const Sprite& o, float t) { order = counter++; splineValue = Lerp(splineValue, o.splineValue, t); }
    void InterpolateTo(const Sprite& o, float t) {
        SplineInterpolateTo(o, t);
        int sprite = counter++;
        SpriteBankIndex = t > 0.5f ? o.SpriteBankIndex : SpriteBankIndex;
        BlendMode = t > 0.5f ? o.BlendMode : BlendMode;
        StartRenderSize = Lerp(StartRenderSize, o.StartRenderSize, t);
        FaceMe2D = t > 0.5f ? o.FaceMe2D : FaceMe2D;
        NoCrossedSprites = LerpL(NoCrossedSprites, o.NoCrossedSprites, t);
        StartColour = LerpC(StartColour, o.StartColour, t);
        TrailWidth = Lerp(TrailWidth, o.TrailWidth, t);
        MaxTrailLength = LerpL(MaxTrailLength, o.MaxTrailLength, t);
        UseSplinePoints = t > 0.5f ? o.UseSplinePoints : UseSplinePoints;
        (void)sprite;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Sprite a = { 0, 0.0f, 2, 1, 1, 10, 7, 1.0f, 0.5f, false, true, 0x00000000, 0 };
    Sprite b = { 0, 8.0f, 5, 3, 4, 20, 99, 3.0f, 1.5f, true, false, 0xFF00FF00, 0 };
    Sprite c = a; c.InterpolateTo(b, 0.25f);
    bool ok = c.order == 0 && near(c.splineValue, 2) && c.SpriteBankIndex == 2 && c.BlendMode == 1 && near(c.StartRenderSize, 1.5f)
        && !c.FaceMe2D && c.NoCrossedSprites == 2 && c.StartColour == 0x3F003F00 && near(c.TrailWidth, 0.75f)
        && c.MaxTrailLength == 13 && c.UseSplinePoints && c.StayWithEmitterIntFactor == 7;
    Sprite d = a; d.InterpolateTo(b, 0.75f);
    ok = ok && d.SpriteBankIndex == 5 && d.BlendMode == 3 && d.FaceMe2D && !d.UseSplinePoints && d.NoCrossedSprites == 3
        && d.MaxTrailLength == 18 && d.StayWithEmitterIntFactor == 7;
    printf(ok ? "00b062a0_TEST PASS\n" : "00b062a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
