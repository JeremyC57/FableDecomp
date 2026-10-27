// Behaviour test for 0x00B5EBF0 CEngineScreenEffectGlowRenderer::SetConfiguration(int) -- copy preset 0..7, ignore anything else
#include <cstdio>
struct CGlowConfiguration { float ThresholdR, ThresholdG, ThresholdB, IntensityR, IntensityG, IntensityB, MotionBlur; };
static CGlowConfiguration g_GlowConfigurations[8];
class CEngineScreenEffectGlowRenderer { public:
    float ThresholdR, ThresholdG, ThresholdB, IntensityR, IntensityG, IntensityB, MotionBlur;
    void SetConfiguration(int c)
    {
        if (c >= 0 && c <= 7) {
            ThresholdR = g_GlowConfigurations[c].ThresholdR; ThresholdG = g_GlowConfigurations[c].ThresholdG;
            ThresholdB = g_GlowConfigurations[c].ThresholdB; IntensityR = g_GlowConfigurations[c].IntensityR;
            IntensityG = g_GlowConfigurations[c].IntensityG; IntensityB = g_GlowConfigurations[c].IntensityB;
            MotionBlur = g_GlowConfigurations[c].MotionBlur;
        }
    }
};
int main()
{
    for (int i = 0; i < 8; ++i) {
        CGlowConfiguration& g = g_GlowConfigurations[i];
        g.ThresholdR = i + 0.1f; g.ThresholdG = i + 0.2f; g.ThresholdB = i + 0.3f;
        g.IntensityR = i + 0.4f; g.IntensityG = i + 0.5f; g.IntensityB = i + 0.6f; g.MotionBlur = i + 0.7f;
    }
    CEngineScreenEffectGlowRenderer r;
    r.ThresholdR = r.ThresholdG = r.ThresholdB = r.IntensityR = r.IntensityG = r.IntensityB = r.MotionBlur = -1.0f;
    r.SetConfiguration(8); r.SetConfiguration(-1);
    if (r.ThresholdR != -1.0f || r.MotionBlur != -1.0f) { std::printf("00b5ebf0_TEST FAIL\n"); return 1; }
    r.SetConfiguration(7);
    if (r.ThresholdR != 7.1f || r.ThresholdB != 7.3f || r.IntensityG != 7.5f || r.MotionBlur != 7.7f) { std::printf("00b5ebf0_TEST FAIL\n"); return 1; }
    r.SetConfiguration(0);
    if (r.ThresholdG != 0.2f || r.IntensityR != 0.4f || r.IntensityB != 0.6f) { std::printf("00b5ebf0_TEST FAIL\n"); return 1; }
    std::printf("00b5ebf0_TEST PASS\n");
    return 0;
}