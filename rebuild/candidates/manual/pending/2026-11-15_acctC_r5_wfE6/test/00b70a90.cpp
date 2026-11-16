// Behaviour test for 0x00B70A90 CEngineWaterRenderer::RenderEnvironmentMap(): a host model of the row with recording stand-ins for the
// texture-stage attach, the shader-constant setters, the UV-mode / blend setters and the draws. Checks: no sky texture or a failed
// GetFrame draws nothing; stage 0 = sky, stage 1 = sky-blend texture when it loads, else the sky again; PS 5 = water colour
// premultiplied by W = Scalers[2] * ColourToReflectionFactor, (R*W, G*W, B*W, 1 - W); PS 6 = clamp01((1 - Blends[0]) * 255 / 255) x4;
// VS 0/1 = colours 12/13; VS 2 = (angle - HFOV/2/2pi, HFOV/2pi, tan(pi/2 - acos(look.Z) + VFOV/2) * 13/14 + 1/14, 0) with
// angle = acos(normalised look.Y)/2pi, mirrored (1 - angle) when look.X < 0; sky pass UV wrap/clamp, draw (0, 14, 18, 2); back-buffer
// pass: blend SRC_ALPHA / INV_SRC_ALPHA on, stage 0 = back buffer, VS 0 = 0s, VS 1 = (w, h, 0, 0), UV clamp, draw (0, 14, 6, 4).
#include <cstdio>
#include <cmath>
struct Rec { int kind; int a; float v[4]; };
struct Log { Rec r[64]; int n; };
static void Put(Log& g, int kind, int a, float x = 0, float y = 0, float z = 0, float w = 0)
{ Rec& r = g.r[g.n++]; r.kind = kind; r.a = a; r.v[0] = x; r.v[1] = y; r.v[2] = z; r.v[3] = w; }
enum { ATTACH, PS, VS, UVU, UVV, BLEND, DRAW };
struct World {
    int sky, skyBlend; bool skyLoads, blendLoads;
    float top[3], bottom[3], blend0, scaler2, colour[3], factor;
    float look[3], hfov, vfov; unsigned long w, h;
};
static void Row(Log& g, const World& p)
{
    if (!p.sky) return;
    if (!p.skyLoads) return;
    int texture = p.sky;
    Put(g, ATTACH, 0, (float)texture);
    if (p.skyBlend && p.blendLoads) texture = p.skyBlend;
    Put(g, ATTACH, 1, (float)texture);
    float alpha = (1.0f - p.blend0) * 255.0f;
    float blend = alpha / 255.0f;
    if (blend < 0.0f) blend = 0.0f; else if (blend > 1.0f) blend = 1.0f;
    float W = p.scaler2 * p.factor;
    Put(g, PS, 5, p.colour[0] * W, p.colour[1] * W, p.colour[2] * W, 1.0f - W);
    Put(g, PS, 6, blend, blend, blend, blend);
    float m = sqrtf(p.look[0] * p.look[0] + p.look[1] * p.look[1]);
    float dx = p.look[0] / m, dy = p.look[1] / m;
    float fovFraction = p.hfov / 6.2831855f;
    float angle = acosf(dy) / 6.2831855f;
    if (dx < 0.0f) angle = 1.0f - angle;
    float start = angle - p.hfov * 0.5f / 6.2831855f;
    Put(g, VS, 0, p.top[0], p.top[1], p.top[2]);
    Put(g, VS, 1, p.bottom[0], p.bottom[1], p.bottom[2]);
    Put(g, VS, 2, start, fovFraction, tanf(1.5707964f - acosf(p.look[2]) + p.vfov * 0.5f) * 0.9285714f + 0.071428575f, 0.0f);
    Put(g, UVU, 0, 0); Put(g, UVV, 0, 1); Put(g, UVU, 1, 0); Put(g, UVV, 1, 1);
    Put(g, DRAW, 0, 14, 18, 2);
    Put(g, BLEND, 0, 1, 1, 1);
    Put(g, ATTACH, 0, -1.0f);
    Put(g, VS, 0, 0, 0, 0, 0);
    Put(g, VS, 1, (float)p.w, (float)p.h, 0, 0);
    Put(g, UVU, 0, 1); Put(g, UVV, 0, 1);
    Put(g, DRAW, 0, 14, 6, 4);
}
static int bad = 0;
static void Near(float a, float b, const char* what) { if (fabs(a - b) > 1e-4f * (1.0f + fabs(b))) { printf("mismatch %s: %f v %f\n", what, a, b); bad = 1; } }
int main()
{
    World p = { 7, 9, true, true, { 0.1f, 0.2f, 0.3f }, { 0.4f, 0.5f, 0.6f }, 0.25f, 0.5f, { 0.8f, 0.6f, 0.4f }, 1.5f,
                { -0.6f, 0.8f, 0.2f }, 1.2f, 0.9f, 640, 480 };
    Log g; g.n = 0;
    World q = p; q.sky = 0; Row(g, q); if (g.n) bad = 1;
    q = p; q.skyLoads = false; Row(g, q); if (g.n) bad = 1;
    q = p; q.blendLoads = false; Row(g, q); if (g.n < 2 || g.r[1].v[0] != 7.0f) bad = 1;
    g.n = 0; Row(g, p);
    if (g.n != 19) bad = 1;
    if (g.r[0].kind != ATTACH || g.r[0].v[0] != 7.0f || g.r[1].a != 1 || g.r[1].v[0] != 9.0f) bad = 1;
    Near(g.r[2].v[0], 0.8f * 0.75f, "ps5 r"); Near(g.r[2].v[3], 0.25f, "ps5 w");
    Near(g.r[3].v[0], 0.75f, "blend");
    Near(g.r[4].v[1], 0.2f, "top g"); Near(g.r[5].v[2], 0.6f, "bottom b");
    float angle = 1.0f - acosf(0.8f) / 6.2831855f;  // look.X < 0 mirrors the heading
    Near(g.r[6].v[0], angle - 0.6f / 6.2831855f, "start"); Near(g.r[6].v[1], 1.2f / 6.2831855f, "fov fraction");
    Near(g.r[6].v[2], tanf(1.5707964f - acosf(0.2f) + 0.45f) * 0.9285714f + 0.071428575f, "sky v");
    if (g.r[11].kind != DRAW || g.r[11].v[1] != 18.0f || g.r[18].v[1] != 6.0f || g.r[18].v[2] != 4.0f) bad = 1;
    Near(g.r[15].v[0], 640.0f, "width"); Near(g.r[15].v[1], 480.0f, "height");
    q = p; q.blend0 = -1.0f; g.n = 0; Row(g, q); Near(g.r[3].v[0], 1.0f, "clamp high");
    q = p; q.blend0 = 2.0f; g.n = 0; Row(g, q); Near(g.r[3].v[0], 0.0f, "clamp low");
    printf(bad ? "00b70a90_TEST FAIL\n" : "00b70a90_TEST PASS\n");
    return bad;
}
