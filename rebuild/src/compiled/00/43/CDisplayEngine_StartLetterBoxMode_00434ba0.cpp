// CDisplayEngine::StartLetterBoxMode(float fade_in_time, float fade_out_time, float ratio) -- open the letter-box fade towards opaque black and set the bar ratio
// Leaf: ?StartLetterBoxMode@CDisplayEngine_Methods@@QAEXMMM@Z
// Retail: Fable.exe @ 0x00434BA0, 83 bytes, `ret 0Ch`; callers 0x00416349, 0x00434FEC, 0x007797DF, 0x006E7283 (tail jmp);
// Ego_r.pdb ?StartLetterBoxMode@CDisplayEngine@@QAEXMMM@Z.
// Body: LetterBox (+0x70, CLetterBoxModeInfo: a CFadeInFadeOutBase then float Ratio) .Open(fade_in_time, fade_out_time,
// CRGBColour(0, 0, 0, 255)) inlined (the out-of-line copy is 0x00434840) -- Active = true, FadeInTime, FadeOutTime,
// Opening = true, Closing = false, OpenTimer = 0, CloseTimer = fade_out_time, ToColour (built in the `push ecx` slot) --
// then LetterBox.Ratio (+0x8c) = ratio.
// Layout: engine/CDisplayEngine.h (LetterBox opaque at +0x70); CFadeInFadeOutBase as the landed 0x00434840 model (ToColour a
// CRGBColour {B, G, R, A} passed by value); Ratio +0x1c of CLetterBoxModeInfo (Ego_r.pdb).
// Shape: /O2 (16-aligned entry); Open defined in the class so VC7.1 inlines it as retail does. Inside Open the two times
// come first, then Active / Closing / Opening / OpenTimer / CloseTimer / ToColour: other orders keep the fade-out time in a
// register (esi) instead of re-reading the argument for CloseTimer.
#include "engine/CDisplayEngine.h"
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CFadeInFadeOutBase_Open {                         // CFadeInFadeOutBase with ToColour typed
    bool       Active;                                   // +0x00
    float      FadeInTime;                               // +0x04
    float      FadeOutTime;                              // +0x08
    bool       Closing;                                  // +0x0c
    bool       Opening;                                  // +0x0d
    float      OpenTimer;                                // +0x10
    float      CloseTimer;                               // +0x14
    CRGBColour ToColour;                                 // +0x18
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)   // 0x00434840
    {
        FadeInTime = fade_in_time;
        FadeOutTime = fade_out_time;
        Active = true;
        Closing = false;
        Opening = true;
        OpenTimer = 0.0f;
        CloseTimer = fade_out_time;
        ToColour = to_colour;
    }
};
struct CLetterBoxModeInfo : CFadeInFadeOutBase_Open {
    float Ratio;                                         // +0x1c
};
struct CDisplayEngine_Methods : CDisplayEngine {
    CLetterBoxModeInfo& GetLetterBox() { return *reinterpret_cast<CLetterBoxModeInfo*>(LetterBox); }
    void StartLetterBoxMode(float fade_in_time, float fade_out_time, float ratio);
};
void CDisplayEngine_Methods::StartLetterBoxMode(float fade_in_time, float fade_out_time, float ratio)
{
    GetLetterBox().Open(fade_in_time, fade_out_time, CRGBColour(0, 0, 0, 255));
    GetLetterBox().Ratio = ratio;
}