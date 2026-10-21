// behaviour test for 00b5f8c0 CEngineScreenEffectGlowRenderer::ComputeGlowColor: byte-quantised threshold/intensity
// colours, the positive and negative blend formulas, the intensity products and the motion-blur complement
#include <cstdio>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b) { R = r; G = g; B = b; }
};
struct SFloatColour { float R, G, B; };
struct SGlow { SFloatColour Threshold, Intensity; float Blend, Scale, Motion; };
struct SOut { float TR, TG, TB, IR, IG, IB, MotionBlur; };
static void ComputeGlowColor(const SGlow* settings, SOut& o)
{
    CRGBColour threshold((unsigned char)(settings->Threshold.R * 255.0f), (unsigned char)(settings->Threshold.G * 255.0f),
                         (unsigned char)(settings->Threshold.B * 255.0f));
    CRGBColour intensity((unsigned char)(settings->Intensity.R * 255.0f), (unsigned char)(settings->Intensity.G * 255.0f),
                         (unsigned char)(settings->Intensity.B * 255.0f));
    float blend = settings->Blend;
    float scale = settings->Scale;
    o.MotionBlur = 1.0f - settings->Motion;
    if (blend >= 0.0f) {
        o.TR = (float)threshold.R * blend * (1.0f / 255.0f) + 1.0f;
        o.TG = (float)threshold.G * blend * (1.0f / 255.0f) + 1.0f;
        o.TB = (float)threshold.B * blend * (1.0f / 255.0f) + 1.0f;
    } else {
        blend = -blend;
        o.TR = 1.0f - (float)threshold.R * blend * (1.0f / 255.0f);
        o.TG = 1.0f - (float)threshold.G * blend * (1.0f / 255.0f);
        o.TB = 1.0f - (float)threshold.B * blend * (1.0f / 255.0f);
    }
    o.IR = (float)intensity.R * scale * (1.0f / 255.0f);
    o.IG = (float)intensity.G * scale * (1.0f / 255.0f);
    o.IB = (float)intensity.B * scale * (1.0f / 255.0f);
}
static int near_(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    SGlow g = { { 1.0f, 0.0f, 0.5f }, { 1.0f, 0.2f, 0.0f }, 0.5f, 2.0f, 0.25f };
    SOut o;
    ComputeGlowColor(&g, o);
    if (!near_(o.TR, 1.5f) || !near_(o.TG, 1.0f) || !near_(o.TB, 1.0f + 127.0f * 0.5f / 255.0f)) { std::printf("00b5f8c0_TEST FAIL\n"); return 1; }
    if (!near_(o.IR, 2.0f) || !near_(o.IG, 51.0f * 2.0f / 255.0f) || !near_(o.IB, 0.0f) || !near_(o.MotionBlur, 0.75f)) { std::printf("00b5f8c0_TEST FAIL\n"); return 1; }
    g.Blend = -0.5f;
    ComputeGlowColor(&g, o);
    if (!near_(o.TR, 0.5f) || !near_(o.TG, 1.0f) || !near_(o.TB, 1.0f - 127.0f * 0.5f / 255.0f)) { std::printf("00b5f8c0_TEST FAIL\n"); return 1; }
    std::printf("00b5f8c0_TEST PASS\n");
    return 0;
}
