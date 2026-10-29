// CDisplayEngine::SetAboutToRun() -- unless the screen fade-out is locked, start the initial fade (open the fade to black over 3 s, step it by 0.1 s, then switch it to closing) and arm the initial-fade state; then close profile section 1
// Leaf: ?SetAboutToRun@CDisplayEngine@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00434CD0, 141 bytes; Ego_r.pdb `?SetAboutToRun@CDisplayEngine@@QAEXXZ`.
// Callees: CFadeInFadeOutBase::Update(float) 0x00434870 (bound `?Update@CFadeInFadeOutBase@@QAEXM@Z`),
//   NProfileTimer::EndProfile(long) 0x009D8250 (__fastcall, empty in retail; tail jump with ecx = 1). Ego_r.pdb names exactly one
//   __fastcall void(long) / void(const CCharString&, long) pair, NProfileTimer::EndProfile / StartProfile, and retail calls
//   0x009D8240 with (string, id) and 0x009D8250 with the same id (CWorld::ActivateWorld 0x0049F180); the landed binding
//   `?LoadingStageDone@@YIXJ@Z` is invented (NAME_FIXES.tsv).
// Globals: 0x01375CDC, the file-scope bool cleared together with InitialFadeActive by StartScreenFadeOut 0x00434C00 and
//   EndScreenFadeOut 0x00434C90; kept under its bound descriptive name g_InitialFadeActiveMirror (no PDB name found).
// Inlined: CFadeInFadeOutBase::Open(float, float, CRGBColour) -- its out-of-line copy is 0x00434840 in this TU, and the store
//   sequence here is that body with FadeInTime 0, FadeOutTime 3.0f; CRGBColour(0, 0, 0, 255) (R, G, B, A stored in that order);
//   CFadeInFadeOutBase::Close() (Opening = false, Closing = true; Ego_r.pdb `?Close@CFadeInFadeOutBase@@QAEXXZ`).
// Shape: the inlined Open's store order is free in its own out-of-line body (every order compiles to retail 0x00434840) but
//   not here: FadeInTime, FadeOutTime, Active, Closing, Opening, OpenTimer, CloseTimer, ToColour gives retail's schedule
//   (FadeOutTime/CloseTimer hoisted through eax, then FadeInTime, Active, Closing, Opening, OpenTimer, ToColour).
// Layout: engine/CDisplayEngine.h (ScreenFadeOutInfo +0xBC, ScreenFadeOutLocked +0xD8, InitialFadeActive +0xEC,
//   ScreenFadeDuration +0xF0, TimeSinceFadeStarted +0xF4) and engine/CFadeInFadeOutBase.h -- retail matches both headers.
#define CDisplayEngine CDisplayEngineLayout
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
#define CFadeInFadeOutBase CFadeInFadeOutBaseLayout
#include "engine/CFadeInFadeOutBase.h"
#undef CFadeInFadeOutBase

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};

class CFadeInFadeOutBase : public CFadeInFadeOutBaseLayout { public:
    void Open(float fadeInTime, float fadeOutTime, CRGBColour toColour)   // out of line at 0x00434840
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
    void Update(float timeElapsed);   // 0x00434870
    void Close()
    {
        Opening = false;
        Closing = true;
    }
};

namespace NProfileTimer {
void __fastcall EndProfile(long id);   // 0x009D8250
}

extern bool g_InitialFadeActiveMirror;   // 0x01375CDC

class CDisplayEngine : public CDisplayEngineLayout { public:
    void SetAboutToRun();
};

void CDisplayEngine::SetAboutToRun()
{
    if (!ScreenFadeOutLocked && g_InitialFadeActiveMirror) {
        reinterpret_cast<CFadeInFadeOutBase&>(ScreenFadeOutInfo).Open(0.0f, 3.0f, CRGBColour(0, 0, 0, 255));
        reinterpret_cast<CFadeInFadeOutBase&>(ScreenFadeOutInfo).Update(0.1f);
        reinterpret_cast<CFadeInFadeOutBase&>(ScreenFadeOutInfo).Close();
        ScreenFadeDuration = -1.0f;
        TimeSinceFadeStarted = 0.0f;
        InitialFadeActive = true;
    }
    NProfileTimer::EndProfile(1);
}