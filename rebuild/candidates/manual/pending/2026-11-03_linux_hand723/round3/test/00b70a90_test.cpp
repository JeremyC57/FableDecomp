// Behaviour test for CEngineWaterRenderer::RenderEnvironmentMap (0x00B70A90): the sky-strip constants (heading fraction, field of
// view fraction, the vertical texture factor), the blend clamp and the premultiplied water colour, through a copy of the row's math.
#include <cstdio>
#include <cmath>

struct Vec4 { float X, Y, Z, W; };
static float GFClamp01(float a) { if (a < 0.0f) a = 0.0f; else if (a > 1.0f) a = 1.0f; return a; }

static void SkyConstants(float lx, float ly, float lz, float hfov, float vfov, float out[3])
{
    float len = std::sqrt(lx * lx + ly * ly);
    float dx = lx / len, dy = ly / len;                          // C2DVector::AccurateNormalise
    float fovFraction = hfov / 6.2831855f;
    float angle = std::acos(dy) / 6.2831855f;
    if (dx < 0.0f)
        angle = 1.0f - angle;
    float halfFOV = hfov * 0.5f;
    out[0] = angle - halfFOV / 6.2831855f;
    out[1] = fovFraction;
    out[2] = std::tan(1.5707964f - std::acos(lz) + vfov * 0.5f) * 0.9285714f + 0.071428575f;
}

static Vec4 Premultiply(float r, float g, float b, float scaler, float factor)
{
    Vec4 c = { r, g, b, scaler * factor };
    Vec4 p = c;
    p.X *= p.W; p.Y *= p.W; p.Z *= p.W; p.W = 1.0f - p.W;
    return p;
}

int main()
{
    bool ok = true;
    float k[3];
    SkyConstants(0.0f, 1.0f, 0.0f, 1.5707964f, 1.0f, k);         // looking north, level, 90-degree HFOV
    ok = ok && std::fabs(k[0] - (-0.125f)) < 1e-5f && std::fabs(k[1] - 0.25f) < 1e-5f;
    SkyConstants(-1.0f, 0.0f, 0.0f, 1.5707964f, 1.0f, k);        // looking west: mirrored heading 1 - 0.25
    ok = ok && std::fabs(k[0] - (0.75f - 0.125f)) < 1e-5f;
    SkyConstants(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, k);               // level look, zero VFOV: tan(0) term
    ok = ok && std::fabs(k[2] - 0.071428575f) < 1e-5f;
    float alpha = (1.0f - 0.25f) * 255.0f;
    ok = ok && std::fabs(GFClamp01(alpha / 255.0f) - 0.75f) < 1e-6f;
    ok = ok && GFClamp01(-0.5f) == 0.0f && GFClamp01(2.0f) == 1.0f;
    Vec4 p = Premultiply(0.5f, 1.0f, 0.25f, 0.5f, 1.0f);
    ok = ok && p.X == 0.25f && p.Y == 0.5f && p.Z == 0.125f && p.W == 0.5f;
    if (!ok) { std::printf("00b70a90_TEST FAIL\n"); return 1; }
    std::printf("00b70a90_TEST PASS\n");
    return 0;
}
