// Behaviour test for NParticleEngine::CPSCLight::InterpolateTo (0x00B18C60): a standalone model of the blend -- floats lerp
// a*(1-t)+b*t, long integers lerp and round to nearest, bools and the position parameter take the target's value only when t > 0.5,
// colours blend per channel with the weight t*256 (GFGetInterpolatedColour 0x00A44250), and the user points lerp for as many
// entries as the target holds.
#include <cstdio>
#include <cmath>
#include <vector>
struct V3 { float x, y, z; };
static float Lerp(float a, float b, float t) { return a * (1 - t) + b * t; }
static long LerpL(long a, long b, float t) { return (long)std::floor((1 - t) * a + b * t + 0.5f); }
static bool Sel(bool a, bool b, float t) { return t > 0.5f ? b : a; }
static unsigned long SelU(unsigned long a, unsigned long b, float t) { return t > 0.5f ? b : a; }
static unsigned long LerpC(unsigned long a, unsigned long b, float t) {
    long w = (long)std::floor(t * 256 + 0.5f); unsigned long r = 0;
    for (int s = 0; s < 32; s += 8) { long ca = (a >> s) & 255, cb = (b >> s) & 255; r |= (unsigned long)((ca * (256 - w) + cb * w) >> 8) << s; }
    return r;
}
struct Light {
    bool Enabled, UseLife; unsigned long PositionParam; float LifeSecs, StartRadius; long Atten, FadeIn;
    unsigned long StartColour; std::vector<V3> Points;
    void InterpolateTo(const Light& o, float t) {
        Enabled = Sel(Enabled, o.Enabled, t); PositionParam = SelU(PositionParam, o.PositionParam, t);
        UseLife = Sel(UseLife, o.UseLife, t); LifeSecs = Lerp(LifeSecs, o.LifeSecs, t); StartRadius = Lerp(StartRadius, o.StartRadius, t);
        Atten = LerpL(Atten, o.Atten, t); FadeIn = LerpL(FadeIn, o.FadeIn, t);
        StartColour = LerpC(StartColour, o.StartColour, t);
        unsigned long count = (unsigned long)o.Points.size();
        for (unsigned long i = 0; i < count; ++i) {
            V3& p = Points[i]; const V3& q = o.Points[i];
            p.x = Lerp(p.x, q.x, t); p.y = Lerp(p.y, q.y, t); p.z = Lerp(p.z, q.z, t);
        }
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Light a, b;
    a.Enabled = false; a.UseLife = true; a.PositionParam = 1; a.LifeSecs = 2; a.StartRadius = 10; a.Atten = 0; a.FadeIn = 100;
    a.StartColour = 0x00000000;
    b.Enabled = true; b.UseLife = false; b.PositionParam = 9; b.LifeSecs = 6; b.StartRadius = 20; b.Atten = 3; b.FadeIn = 200;
    b.StartColour = 0xFFFFFFFF;
    V3 p0 = { 0, 0, 0 }, p1 = { 4, 8, -4 };
    for (int i = 0; i < 5; ++i) a.Points.push_back(p0);
    for (int i = 0; i < 3; ++i) b.Points.push_back(p1);
    Light c = a; c.InterpolateTo(b, 0.25f);
    bool ok = !c.Enabled && c.UseLife && c.PositionParam == 1 && near(c.LifeSecs, 3) && near(c.StartRadius, 12.5f)
        && c.Atten == 1 && c.FadeIn == 125 && c.StartColour == 0x3F3F3F3F
        && near(c.Points[0].x, 1) && near(c.Points[2].y, 2) && near(c.Points[1].z, -1)
        && near(c.Points[3].x, 0) && near(c.Points[4].y, 0);            // only the target's three points are blended
    Light d = a; d.InterpolateTo(b, 0.75f);
    ok = ok && d.Enabled && !d.UseLife && d.PositionParam == 9 && near(d.LifeSecs, 5) && d.Atten == 2 && d.FadeIn == 175;
    Light e = a; e.InterpolateTo(b, 0.5f);                                // exactly half keeps the source's flags
    ok = ok && !e.Enabled && e.PositionParam == 1 && near(e.LifeSecs, 4);
    Light f = a; f.InterpolateTo(b, 1.0f);
    ok = ok && f.StartColour == 0xFFFFFFFF && f.Atten == 3;
    printf(ok ? "00b18c60_TEST PASS\n" : "00b18c60_TEST FAIL\n");
    return ok ? 0 : 1;
}