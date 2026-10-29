// Behaviour test for CDisplayEngine::SetAboutToRun @ 0x00434CD0: unless the fade-out is locked (or the initial-fade flag is
// clear), the screen fade opens to black over 3 s, steps 0.1 s, switches to closing and the initial-fade state is armed;
// profile section 1 is always closed.
#include <stdio.h>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
static float g_updated = 0.0f;
static int g_updates = 0;
class CFadeInFadeOutBase { public:
    float FadeInTime, FadeOutTime, OpenTimer, CloseTimer;
    bool Active, Opening, Closing;
    unsigned char ToColour[4];
    void Open(float fadeInTime, float fadeOutTime, CRGBColour toColour)
    {
        FadeInTime = fadeInTime;
        FadeOutTime = fadeOutTime;
        Active = true;
        Closing = false;
        Opening = true;
        OpenTimer = 0.0f;
        CloseTimer = fadeOutTime;
        *reinterpret_cast<CRGBColour*>(ToColour) = toColour;
    }
    void Update(float timeElapsed) { g_updated = timeElapsed; g_updates++; }
    void Close() { Opening = false; Closing = true; }
};
static int g_profileEnded = -1;
namespace NProfileTimer { void EndProfile(int id) { g_profileEnded = id; } }
bool g_InitialFadeActiveMirror = true;
class CDisplayEngine { public:
    CFadeInFadeOutBase ScreenFadeOutInfo;
    bool  ScreenFadeOutLocked;
    bool  InitialFadeActive;
    float ScreenFadeDuration;
    float TimeSinceFadeStarted;
    void SetAboutToRun()
    {
        if (!ScreenFadeOutLocked && g_InitialFadeActiveMirror) {
            ScreenFadeOutInfo.Open(0.0f, 3.0f, CRGBColour(0, 0, 0, 255));
            ScreenFadeOutInfo.Update(0.1f);
            ScreenFadeOutInfo.Close();
            ScreenFadeDuration = -1.0f;
            TimeSinceFadeStarted = 0.0f;
            InitialFadeActive = true;
        }
        NProfileTimer::EndProfile(1);
    }
};
static int fail(int line) { printf("00434cd0_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CDisplayEngine e;
    e.ScreenFadeOutInfo.Active = false; e.ScreenFadeOutInfo.Opening = false; e.ScreenFadeOutInfo.Closing = false;
    e.ScreenFadeOutLocked = true; e.InitialFadeActive = false; e.ScreenFadeDuration = 5.0f; e.TimeSinceFadeStarted = 2.0f;
    e.SetAboutToRun();
    if (g_updates != 0 || e.InitialFadeActive || e.ScreenFadeDuration != 5.0f || g_profileEnded != 1) return fail(__LINE__);
    g_profileEnded = -1; e.ScreenFadeOutLocked = false; g_InitialFadeActiveMirror = false;
    e.SetAboutToRun();
    if (g_updates != 0 || e.InitialFadeActive || g_profileEnded != 1) return fail(__LINE__);
    g_InitialFadeActiveMirror = true;
    e.SetAboutToRun();
    CFadeInFadeOutBase& f = e.ScreenFadeOutInfo;
    if (g_updates != 1 || g_updated != 0.1f || !f.Active || f.Opening || !f.Closing) return fail(__LINE__);
    if (f.FadeInTime != 0.0f || f.FadeOutTime != 3.0f || f.CloseTimer != 3.0f || f.OpenTimer != 0.0f) return fail(__LINE__);
    CRGBColour* c = reinterpret_cast<CRGBColour*>(f.ToColour);
    if (c->R != 0 || c->G != 0 || c->B != 0 || c->A != 255) return fail(__LINE__);
    if (e.ScreenFadeDuration != -1.0f || e.TimeSinceFadeStarted != 0.0f || !e.InitialFadeActive) return fail(__LINE__);
    printf("00434cd0_TEST PASS\n");
    return 0;
}