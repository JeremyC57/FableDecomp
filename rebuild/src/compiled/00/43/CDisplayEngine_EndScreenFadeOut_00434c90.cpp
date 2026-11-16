// CDisplayEngine::EndScreenFadeOut() -- cancel the initial fade and, unless the fade-out is locked, start fading the screen back in
// Leaf: ?EndScreenFadeOut@CDisplayEngine@@QAEXXZ
// Retail: Fable.exe @ 0x00434C90, 59 bytes, `ret`; Ego_r.pdb ?EndScreenFadeOut@CDisplayEngine@@QAEXXZ.
// Body: InitialFadeActive (+0xec) and the file-scope flag at 0x01375CDC (written only here and at 0x00434C08, always
// together with InitialFadeActive) are cleared; if ScreenFadeOutInfo.Active (+0xbc) && !ScreenFadeOutLocked (+0xd8):
// ScreenFadeDuration = -1.0f, TimeSinceFadeStarted = 0, ScreenFadeOutInfo.Opening = false (+0xc9), .Closing = true (+0xc8).
// Layout: engine/CDisplayEngine.h (ScreenFadeOutInfo is CFadeInFadeOutBase, engine/CFadeInFadeOutBase.h).
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_ENDSCREENFADEOUT
#include "engine/CDisplayEngine.h"
extern bool g_InitialFadeActiveMirror;                   // 0x01375CDC
void CDisplayEngine::EndScreenFadeOut()
{
    InitialFadeActive = false;
    g_InitialFadeActiveMirror = false;
    if (ScreenFadeOutInfo.Active && !ScreenFadeOutLocked)
    {
        ScreenFadeDuration = -1.0f;
        TimeSinceFadeStarted = 0.0f;
        ScreenFadeOutInfo.Opening = false;
        ScreenFadeOutInfo.Closing = true;
    }
}