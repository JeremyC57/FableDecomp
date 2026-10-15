// Behaviour test for 0x006b84e0 CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo
#include <cstdio>
struct C3DVector { float x, y, z; C3DVector() : x(0.0f), y(0.0f), z(0.0f) {} };
struct CRadialBlurParams { float Length; };
struct CRadialBlurPositionParams {
    C3DVector AreaCenter, BlurCenter; long AreaTrackPos, BlurTrackPos;
    CRadialBlurPositionParams() : AreaTrackPos(1), BlurTrackPos(1) {}
};
static bool g_ok = false; static int g_calls = 0;
struct CScreenEffectPriorityStack {
    void InsertRadialBlurEntry(float duration, const CRadialBlurParams* params, const CRadialBlurPositionParams* position, long priority)
    {
        ++g_calls;
        g_ok = duration == 2.5f && params->Length == 4.0f && priority == 3 && position->AreaTrackPos == 1 && position->BlurTrackPos == 1
            && position->AreaCenter.x == 0.0f && position->BlurCenter.z == 0.0f;
    }
};
struct CEnvironmentSpellEffectInterpolator {
    CScreenEffectPriorityStack ScreenEffectPriorityStack;
    void RadialBlurFadeTo(float duration, long priority, const CRadialBlurParams& params)
    {
        CRadialBlurPositionParams position;
        ScreenEffectPriorityStack.InsertRadialBlurEntry(duration, &params, &position, priority);
    }
};
int main()
{
    CEnvironmentSpellEffectInterpolator i; CRadialBlurParams p; p.Length = 4.0f;
    i.RadialBlurFadeTo(2.5f, 3, p);
    if (!(g_ok && g_calls == 1)) { std::printf("006b84e0_TEST FAIL\n"); return 1; }
    std::printf("006b84e0_TEST PASS\n");
    return 0;
}
