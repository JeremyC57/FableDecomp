#include <cstdio>
#include <math.h>
int g_framesPerSecond = 30;
struct CThingGraphic { unsigned char _pad[0x5a]; unsigned char m_alpha; };
class CThing { public: unsigned char _pad[0x64]; CThingGraphic* m_pGraphic; };
class CTCBase { public: void* vt; CThing* ParentThing; unsigned char _pad8[4]; };
class CTCFadeOutAndIn : public CTCBase { public: bool FadingOut; bool FadingIn; float Alpha; long FramesLeftForFade; void FadeIn(float seconds, bool immediately); };
__forceinline long ROUND(float value) { long result;
    __asm fld value
    __asm fistp result
    return result;
}
void CTCFadeOutAndIn::FadeIn(float seconds, bool immediately)
{
    FadingIn = true; FadingOut = false;
    FramesLeftForFade = ROUND(g_framesPerSecond * seconds);
    if (immediately) Alpha = 0.0f;
    else if (ParentThing->m_pGraphic) Alpha = ParentThing->m_pGraphic->m_alpha * (1.0f / 255.0f);
}
int main()
{
    CThingGraphic g; g.m_alpha = 51; CThing th; th.m_pGraphic = &g; CTCFadeOutAndIn f; f.ParentThing = &th; f.Alpha = -1.0f;
    f.FadeIn(2.05f, false);
    bool ok = f.FramesLeftForFade == 62 && fabs(f.Alpha - 0.2f) < 0.001f && f.FadingIn && !f.FadingOut;
    f.FadeIn(0.5f, true); ok = ok && f.FramesLeftForFade == 15 && f.Alpha == 0.0f;
    th.m_pGraphic = 0; f.Alpha = -1.0f; f.FadeIn(1.0f, false); ok = ok && f.FramesLeftForFade == 30 && f.Alpha == -1.0f;
    std::printf(ok ? "006c3bd0_TEST PASS\n" : "006c3bd0_TEST FAIL\n");
    return ok ? 0 : 1;
}