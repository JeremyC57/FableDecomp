// CDisplayEngine::StartScreenFadeOut(float, float, CRGBColour, float) -- cancel the initial fade and, unless the screen fade is locked, start the screen fade-out towards a colour (fade-in/out times and total duration)
// Grade: DIFFER(142v144) -- scheduling/registers only: retail copies the colour's low half (G, B) through ebx (push ebx) AFTER the FadeInTime/FadeOutTime stores, ours copies both halves through edx before them; tried member-order and body-order copy constructors (RGBA/BGRA) and two Start statement orders
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434C00, length 144 bytes. No callees.
// Globals: a fade flag byte @ 0x01375CDC (cleared together with InitialFadeActive).
// Layout: engine/CDisplayEngine.h (PDB, fits the bytes): ScreenFadeOutInfo +0xbc (CFadeInFadeOutBase: Active +0, FadeInTime +4,
// FadeOutTime +8, Closing +0xc, Opening +0xd, OpenTimer +0x10, CloseTimer +0x14, ToColour +0x18), ScreenFadeOutLocked +0xd8,
// InitialFadeReadynessCount +0xe8, InitialFadeActive +0xec, ScreenFadeDuration +0xf0, TimeSinceFadeStarted +0xf4.
// Shape: one zero register (eax) for the byte clears, the float zeros and the counter; the locked path only clears the readiness
// count. The fade start is an inline CFadeInFadeOutBase method taking the colour BY VALUE: CRGBColour (D3DCOLOR byte order) has a
// user copy constructor, so the argument copy is built byte by byte (two 16-bit halves of the source dword, R/A first) in the dead
// `duration` slot after the duration has been read, then stored as one dword; with the two times stored first CloseTimer re-reads
// its argument like retail.
#include "engine/CDisplayEngine.h"

struct CRGBColour {                               // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& other)
    {
        R = other.R;
        A = other.A;
        G = other.G;
        B = other.B;
    }
};

struct CFadeInFadeOutBase {                        // PDB 0x1c bytes
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0c
    bool       Opening;                            // +0x0d
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
    void Start(float fadeInTime, float fadeOutTime, CRGBColour colour)
    {
        FadeInTime = fadeInTime;
        FadeOutTime = fadeOutTime;
        Active = true;
        Closing = false;
        Opening = true;
        OpenTimer = 0.0f;
        CloseTimer = fadeOutTime;
        ToColour = colour;
    }
};

extern bool g_screenFadeFlag;                      // 0x01375CDC

struct CDisplayEngine_Methods : CDisplayEngine {
    void StartScreenFadeOut(float fadeInTime, float fadeOutTime, CRGBColour colour, float duration);
};

void CDisplayEngine_Methods::StartScreenFadeOut(float fadeInTime, float fadeOutTime, CRGBColour colour, float duration)
{
    InitialFadeActive = false;
    g_screenFadeFlag = false;
    if (ScreenFadeOutLocked) {
        InitialFadeReadynessCount = 0;
        return;
    }
    ScreenFadeDuration = duration;
    TimeSinceFadeStarted = 0.0f;
    reinterpret_cast<CFadeInFadeOutBase*>(ScreenFadeOutInfo)->Start(fadeInTime, fadeOutTime, colour);
}
