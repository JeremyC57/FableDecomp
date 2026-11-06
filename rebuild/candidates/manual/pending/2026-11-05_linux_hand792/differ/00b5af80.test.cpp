// Behaviour test for 00b5af80 (CEngineScreenEffectColourFilter colour-filter constants): a standalone copy of the reconstructed
// pixel-shader constant math (grey weights + saturation, tint * clamped contrast * 0.5, tint * (clamped brightness + 0.5),
// the inverse, and the clamped alpha byte).
#include <cstdio>
#include <cmath>
static void Limit(float& v, float lo, float hi) { if (v < lo) v = lo; else if (v > hi) v = hi; }
struct V4 { float x, y, z, w; };
static void Constants(float sat, float con, V4 tint, float bri, float alpha, float greyLevel, V4 grey, V4* out, int* a)
{
    V4 g = { grey.x, grey.y, grey.z, sat };
    float c = con + 1.0f; Limit(c, 0.0f, 2.0f);
    V4 scale = { tint.x * c * 0.5f, tint.y * c * 0.5f, tint.z * c * 0.5f, tint.w * c * 0.5f };
    float b = bri - (1.0f - greyLevel) * con; Limit(b, -0.5f, 0.5f); b += 0.5f;
    V4 off = { tint.x * b, tint.y * b, tint.z * b, tint.w * b };
    V4 inv = { 1.0f - g.x, 1.0f - g.y, 1.0f - g.z, 1.0f - g.w };
    out[0] = g; out[1] = scale; out[2] = off; out[3] = inv;
    Limit(alpha, 0.0f, 1.0f); *a = (int)floor(alpha * 255.0f + 0.5f);
}
int main()
{
    V4 out[4]; int a = 0; V4 tint = { 1, 1, 1, 1 }; V4 grey = { 0.3f, 0.59f, 0.11f, 0 };
    int fails = 0;
    Constants(0.5f, 0.0f, tint, 0.0f, 1.0f, 0.5f, grey, out, &a);
    if (out[0].w != 0.5f || out[1].x != 0.5f || out[2].x != 0.5f || out[3].y != 1.0f - 0.59f || a != 255) ++fails;
    Constants(1.0f, 5.0f, tint, 9.0f, -1.0f, 0.5f, grey, out, &a);      // contrast clamps to 2, brightness to +0.5, alpha to 0
    if (out[1].x != 1.0f || out[2].x != 1.0f || a != 0) ++fails;
    if (fails) { std::printf("00b5af80_TEST FAIL %d\n", fails); return 1; }
    std::printf("00b5af80_TEST PASS\n");
    return 0;
}
