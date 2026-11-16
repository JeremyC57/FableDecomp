// Behaviour test for CEngineScreenEffectColourFilter::ApplyColourFilter (0x00B5A3F0): a model of the four colour-filter pixel-shader
// constants (c5 grey weights + saturation, c6 tint * clamp(contrast + 1, 0, 2) * 0.5, c7 tint * (clamp(brightness - (1 - grey level)
// * contrast, -0.5, 0.5) + 0.5), c8 = 1 - c5) and of the quad colour (alpha clamped to [0, 1], rounded to a byte; alpha blending
// enabled only below 255).
#include <cstdio>
#include <cmath>
struct V4 { float x, y, z, w; };
struct Settings { float saturation, contrast, brightness, alpha; V4 tint; };
static float Limit(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static void Constants(const Settings& s, const V4& weights, float greyLevel, V4 c[4], unsigned long& diffuse, bool& blend)
{
    c[0].x = weights.x; c[0].y = weights.y; c[0].z = weights.z; c[0].w = s.saturation;
    float contrast = Limit(s.contrast + 1.0f, 0.0f, 2.0f) * 0.5f;
    c[1].x = s.tint.x * contrast; c[1].y = s.tint.y * contrast; c[1].z = s.tint.z * contrast; c[1].w = s.tint.w * contrast;
    float brightness = Limit(s.brightness - (1.0f - greyLevel) * s.contrast, -0.5f, 0.5f) + 0.5f;
    c[2].x = s.tint.x * brightness; c[2].y = s.tint.y * brightness; c[2].z = s.tint.z * brightness; c[2].w = s.tint.w * brightness;
    c[3].x = 1.0f - c[0].x; c[3].y = 1.0f - c[0].y; c[3].z = 1.0f - c[0].z; c[3].w = 1.0f - c[0].w;
    long a = (long)std::floor(Limit(s.alpha, 0.0f, 1.0f) * 255.0f + 0.5f);
    diffuse = 0x00FFFFFFul | ((unsigned long)a << 24);
    blend = a < 255;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    V4 w = { 0.3f, 0.59f, 0.11f, 0.0f };
    Settings s = { 0.25f, 0.5f, 0.1f, 0.5f, { 1.0f, 0.5f, 0.25f, 1.0f } };
    V4 c[4]; unsigned long d; bool blend;
    Constants(s, w, 0.5f, c, d, blend);
    bool ok = Near(c[0].w, 0.25f) && Near(c[1].x, 0.75f) && Near(c[1].z, 0.1875f)      // contrast 1.5 * 0.5
        && Near(c[2].x, 0.35f) && Near(c[2].y, 0.175f)                                     // brightness 0.1 - 0.25 + 0.5
        && Near(c[3].x, 0.7f) && Near(c[3].w, 0.75f) && d == 0x80FFFFFFul && blend;
    Settings h = { 0.0f, 3.0f, 2.0f, 2.0f, { 1.0f, 1.0f, 1.0f, 1.0f } };                  // every clamp at its top
    Constants(h, w, 0.5f, c, d, blend);
    ok = ok && Near(c[1].x, 1.0f) && Near(c[2].x, 1.0f) && d == 0xFFFFFFFFul && !blend;
    Settings l = { 1.0f, -3.0f, -2.0f, -1.0f, { 1.0f, 1.0f, 1.0f, 1.0f } };               // contrast floor; brightness -2 + 1.5 clamped to -0.5
    Constants(l, w, 0.5f, c, d, blend);
    ok = ok && Near(c[1].x, 0.0f) && Near(c[2].x, 0.0f) && Near(c[3].w, 0.0f) && d == 0x00FFFFFFul && blend;
    printf(ok ? "00b5a3f0_TEST PASS\n" : "00b5a3f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
