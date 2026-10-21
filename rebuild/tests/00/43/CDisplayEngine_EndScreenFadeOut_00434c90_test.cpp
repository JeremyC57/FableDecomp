#include <cstdio>
#include "engine/CDisplayEngine.h"
#include "engine/CFadeInFadeOutBase.h"
extern bool g_InitialFadeActiveMirror;                   // 0x01375CDC
struct CDisplayEngine_Methods : CDisplayEngine {
    CFadeInFadeOutBase& GetScreenFadeOutInfo() { return *reinterpret_cast<CFadeInFadeOutBase*>(ScreenFadeOutInfo); }
    void EndScreenFadeOut();
};
void CDisplayEngine_Methods::EndScreenFadeOut()
{
    InitialFadeActive = false;
    g_InitialFadeActiveMirror = false;
    if (GetScreenFadeOutInfo().Active && !ScreenFadeOutLocked)
    {
        ScreenFadeDuration = -1.0f;
        TimeSinceFadeStarted = 0.0f;
        GetScreenFadeOutInfo().Opening = false;
        GetScreenFadeOutInfo().Closing = true;
    }
}

#include <cstring>
bool g_InitialFadeActiveMirror = true;
static bool Case(bool active, bool locked)
{
    static CDisplayEngine_Methods e; std::memset(&e, 0, sizeof e);
    e.InitialFadeActive = true; g_InitialFadeActiveMirror = true; e.ScreenFadeDuration = 5.0f; e.TimeSinceFadeStarted = 2.0f;
    e.GetScreenFadeOutInfo().Active = active; e.ScreenFadeOutLocked = locked; e.GetScreenFadeOutInfo().Opening = true;
    e.EndScreenFadeOut();
    if (e.InitialFadeActive || g_InitialFadeActiveMirror) return false;
    bool fired = active && !locked;
    if (fired) return e.ScreenFadeDuration == -1.0f && e.TimeSinceFadeStarted == 0.0f && !e.GetScreenFadeOutInfo().Opening && e.GetScreenFadeOutInfo().Closing;
    return e.ScreenFadeDuration == 5.0f && e.TimeSinceFadeStarted == 2.0f && e.GetScreenFadeOutInfo().Opening && !e.GetScreenFadeOutInfo().Closing;
}
int main()
{
    if (Case(true, false) && Case(true, true) && Case(false, false)) std::printf("00434c90_TEST PASS\n");
    return 0;
}