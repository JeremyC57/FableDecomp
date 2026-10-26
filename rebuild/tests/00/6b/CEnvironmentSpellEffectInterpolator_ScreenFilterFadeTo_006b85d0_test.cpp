// Behaviour test for 0x006b85d0 CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo
#include <cstdio>
class CRGBFloatColour { public: float R, G, B, A; };
struct CScreenFilterParams { float Alpha, Saturation, Contrast, Brightness; CRGBFloatColour Tint; };
struct CThingGraphicArgList { int n; };
static bool g_ok = false;
struct CScreenEffectPriorityStack {
    void InsertScreenFilterEntry(float duration, const CScreenFilterParams* p, const CThingGraphicArgList* things, long priority)
    {
        g_ok = duration == 1.5f && priority == 2 && things && things->n == 7 && p->Alpha == 0.5f && p->Saturation == 0.25f
            && p->Contrast == 2.0f && p->Brightness == 3.0f && p->Tint.R == 0.125f && p->Tint.A == 1.0f;
    }
};
class CEnvironmentSpellEffectInterpolator { public:
    CScreenEffectPriorityStack ScreenEffectPriorityStack;
    void ScreenFilterFadeTo(float duration, long priority, float alpha, float saturation, float contrast, float brightness,
                            const CRGBFloatColour& tint, const CThingGraphicArgList* things)
    {
        CScreenFilterParams params;
        params.Alpha = alpha; params.Saturation = saturation; params.Contrast = contrast; params.Brightness = brightness; params.Tint = tint;
        ScreenEffectPriorityStack.InsertScreenFilterEntry(duration, &params, things, priority);
    }
};
int main()
{
    CEnvironmentSpellEffectInterpolator i; CRGBFloatColour t = { 0.125f, 0.25f, 0.5f, 1.0f }; CThingGraphicArgList l; l.n = 7;
    i.ScreenFilterFadeTo(1.5f, 2, 0.5f, 0.25f, 2.0f, 3.0f, t, &l);
    if (!g_ok) { std::printf("006b85d0_TEST FAIL\n"); return 1; }
    std::printf("006b85d0_TEST PASS\n");
    return 0;
}