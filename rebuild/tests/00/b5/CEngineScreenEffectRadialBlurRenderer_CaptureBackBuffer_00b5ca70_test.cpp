// Behaviour test for 0x00B5CA70 CEngineScreenEffectRadialBlurRenderer::CaptureBackBuffer(): a host model of the row with recording
// stand-ins for the render target switch and the shader-constant setters. Checks: the four render passes go into BlurTexture256_1,
// _2, _1, _2 (copy, then three ping-pong blur passes); the copy pass sets PS constants 5..7 = (threshold RGB, 0.3) / 1s / 0s and VS
// constants 0..1 = (AreaCenter.X * 2 - 1, 1 - AreaCenter.Y * 2, 1 / Range, 1 / Range) / (0.5, 0, 0, 0); each blur pass with step
// 1 / 4 / 16 sets VS 0 = (0.5, 0.5, 0.5, 0.5) and VS 1..4 = GetRotationVector(Twist * k) * length_k plus the centre offsets
// ((BlurCenter - 0.5) * Length * k / 256, zero for tap 0, length_0 = 1), k = tap * step, and PS 5..8 = pow(IntensityR, k).
#include <cstdio>
#include <cmath>
struct Rec { int kind; int index; float v[4]; };
struct Log { Rec r[64]; int n; int target[8]; int nt; };
static void VS(Log& g, int i, float a, float b, float c, float d) { Rec& r = g.r[g.n++]; r.kind = 0; r.index = i; r.v[0] = a; r.v[1] = b; r.v[2] = c; r.v[3] = d; }
static void PS(Log& g, int i, float a, float b, float c, float d) { Rec& r = g.r[g.n++]; r.kind = 1; r.index = i; r.v[0] = a; r.v[1] = b; r.v[2] = c; r.v[3] = d; }
static void Target(Log& g, int t) { g.target[g.nt++] = t; }
struct P { float ThrR, ThrG, ThrB, Range, Twist, Length, IntensityR, AreaX, AreaY, CentreX, CentreY; };
static void GetRotationVector(float angle, float* pX, float* pY) { *pX = cos(angle); *pY = sin(angle); }
static void BlurPass(Log& g, const P& p, int dst, float step)
{
    Target(g, dst);
    VS(g, 0, 0.5f, 0.5f, 0.0f, 0.0f);
    float length0 = 1.0f;
    float length[4] = { length0, 1.0f - p.Length * (1 * step / 256.0f), 1.0f - p.Length * (2 * step / 256.0f), 1.0f - p.Length * (3 * step / 256.0f) };
    VS(g, 0, 0.5f, 0.5f, 0.5f, 0.5f);
    for (int i = 0; i < 4; ++i) {
        float x, y;
        GetRotationVector(p.Twist * (i * step), &x, &y);
        float ox = i ? (float)((p.CentreX - 0.5f) * p.Length * (i * step / 256.0)) : 0.0f;
        float oy = i ? (float)((p.CentreY - 0.5f) * p.Length * (i * step / 256.0)) : 0.0f;
        VS(g, 1 + i, x * length[i], y * length[i], ox, oy);
    }
    for (int i = 0; i < 4; ++i) { float k = (float)pow(p.IntensityR, i * step); PS(g, 5 + i, k, k, k, k); }
}
static void Row(Log& g, const P& p)
{
    Target(g, 1);
    PS(g, 5, p.ThrR, p.ThrG, p.ThrB, 0.3f); PS(g, 6, 1, 1, 1, 1); PS(g, 7, 0, 0, 0, 0);
    float invRange = 1.0f / p.Range;
    VS(g, 0, p.AreaX * 2.0f - 1.0f, 1.0f - p.AreaY * 2.0f, invRange, invRange); VS(g, 1, 0.5f, 0, 0, 0);
    BlurPass(g, p, 2, 1.0f); BlurPass(g, p, 1, 4.0f); BlurPass(g, p, 2, 16.0f);
}
static int bad = 0;
static void Near(float a, float b, const char* what) { if (fabs(a - b) > 1e-4f * (1.0f + fabs(b))) { printf("mismatch %s: %f v %f\n", what, a, b); bad = 1; } }
int main()
{
    P p = { 0.25f, 0.5f, 0.75f, 4.0f, 0.1f, 32.0f, 0.9f, 0.3f, 0.6f, 0.7f, 0.2f };
    Log g; g.n = 0; g.nt = 0; Row(g, p);
    if (g.nt != 4 || g.target[0] != 1 || g.target[1] != 2 || g.target[2] != 1 || g.target[3] != 2) bad = 1;
    if (g.n != 5 + 3 * 10) bad = 1;
    Near(g.r[0].v[3], 0.3f, "thr w"); Near(g.r[3].v[0], -0.4f, "area x"); Near(g.r[3].v[1], -0.2f, "area y"); Near(g.r[3].v[2], 0.25f, "1/range");
    const float steps[3] = { 1, 4, 16 };
    for (int pass = 0; pass < 3; ++pass) {
        const Rec* r = &g.r[5 + pass * 10];
        if (r[0].index != 0 || r[1].index != 0 || r[1].v[3] != 0.5f) bad = 1;
        for (int i = 0; i < 4; ++i) {
            float k = i * steps[pass], len = i ? 1.0f - 32.0f * k / 256.0f : 1.0f;
            if (r[2 + i].kind != 0 || r[2 + i].index != 1 + i) bad = 1;
            Near(r[2 + i].v[0], (float)cos(0.1f * k) * len, "tap x"); Near(r[2 + i].v[1], (float)sin(0.1f * k) * len, "tap y");
            Near(r[2 + i].v[2], i ? 0.2f * 32.0f * k / 256.0f : 0.0f, "off x"); Near(r[2 + i].v[3], i ? -0.3f * 32.0f * k / 256.0f : 0.0f, "off y");
            if (r[6 + i].kind != 1 || r[6 + i].index != 5 + i) bad = 1;
            Near(r[6 + i].v[2], (float)pow(0.9, (double)k), "intensity");
        }
    }
    printf(bad ? "00b5ca70_TEST FAIL\n" : "00b5ca70_TEST PASS\n");
    return bad;
}