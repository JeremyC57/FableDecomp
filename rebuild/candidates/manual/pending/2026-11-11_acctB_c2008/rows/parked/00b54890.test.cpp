// Behaviour test for CEngineWeatherRenderer::RenderRain (0x00B54890): a host model of the constant uploads and of the box walk (81 box
// columns x nine levels, the per-box visibility slot [i * level], camera clip unless locked, landscape clip, mask-sphere split into the
// plain and the clipping list, plain boxes rendered first).
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
static float constants[32][4]; static long nconst;
static void SetConst(unsigned long i, float x, float y, float z, float w) { constants[i][0] = x; constants[i][1] = y; constants[i][2] = z; constants[i][3] = w; ++nconst; }
static long rendered[1000]; static long nrendered; static long clipped_from;
static int Visible[729];
static Vec BoxPos[81];
static bool locked;
static int ClipCamera(const Vec& p, float radius) { return p.X > radius ? 1 : 0; }
static int ClipLandscape(const Vec& p) { return p.Z < -30.0f ? 1 : 0; }
static int ClipMask(const Vec& p) { return p.Y > 50.0f ? 1 : 0; }

static void RenderRain(bool have_texture, float span, float fade_start, float fade_end)
{
    if (!have_texture) return;
    SetConst(7, 2.0f, (3.0f - 1.0f) * 0.5f + 1.0f, fade_end / (fade_start - fade_end) + 1.0f, 1.0f / (fade_end - fade_start));
    long constant = 9 + 16;                                   // SetCommonParticleShaderConstants: 1 + 3 x 5 constants
    for (long set = 0; set < 5; ++set) SetConst(constant++, (float)set, 10.0f + set, 0.5f, 0.0f);
    float max_z = span * 5.0f;
    Vec boxes[1000], clipped[1000]; long nb = 0, nc = 0;
    long level = 1;
    for (float z = span * -4.0f; z < max_z; z += span, ++level)
        for (long i = 0; i < 81; ++i) {
            Vec pos = { BoxPos[i].X, BoxPos[i].Y, BoxPos[i].Z + z };
            float radius = fade_end;
            int& visible = Visible[i * level];
            if (!locked) visible = ClipCamera(pos, radius);
            if (visible != 0) continue;
            if (ClipLandscape(pos) != 0) continue;
            if (ClipMask(pos) == 1) clipped[nc++] = pos; else boxes[nb++] = pos;
        }
    for (long b = 0; b < nb; ++b) rendered[nrendered++] = (long)boxes[b].X;
    clipped_from = nrendered;
    for (long c = 0; c < nc; ++c) rendered[nrendered++] = (long)clipped[c].X;
}

int main()
{
    for (long i = 0; i < 81; ++i) { BoxPos[i].X = (float)i; BoxPos[i].Y = (float)(i % 9) * 10.0f; BoxPos[i].Z = 0.0f; }
    RenderRain(false, 10.0f, 20.0f, 60.0f);
    if (nconst != 0) { std::printf("00b54890_TEST FAIL\n"); return 1; }
    RenderRain(true, 10.0f, 20.0f, 60.0f);
    // levels -4..4 (9); z < -30 clipped by the landscape (levels -4); x > 60 clipped by the camera; y > 50 -> clipping list
    long expect_plain = 0, expect_clip = 0;
    for (long l = -3; l <= 4; ++l) for (long i = 0; i <= 60; ++i) { if ((i % 9) * 10 > 50) ++expect_clip; else ++expect_plain; }
    bool ok = nconst == 6 && std::fabs(constants[7][3] - 1.0f / 40.0f) < 1e-6f && std::fabs(constants[7][2] - (60.0f / -40.0f + 1.0f)) < 1e-6f
        && constants[7][1] == 2.0f && constants[29][1] == 14.0f && clipped_from == expect_plain && nrendered == expect_plain + expect_clip
        && Visible[61 * 9] == 1 && Visible[3] == 0;
    locked = true; long before = nrendered; nrendered = 0; nconst = 0;
    for (long v = 0; v < 729; ++v) Visible[v] = 1;
    RenderRain(true, 10.0f, 20.0f, 60.0f);                         // locked boxes keep their saved (clipped) visibility
    ok = ok && before == 488 && nrendered == 0 && nconst == 6;
    if (!ok) { std::printf("00b54890_TEST FAIL\n"); return 1; }
    std::printf("00b54890_TEST PASS\n");
    return 0;
}
