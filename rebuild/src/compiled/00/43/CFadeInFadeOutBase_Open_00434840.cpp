// CFadeInFadeOutBase::Open(float, float, CRGBColour) -- start fading in towards a colour, with the given fade-in and fade-out times
// Leaf: ?Open@CFadeInFadeOutBase@@QAEXMMVCRGBColour@@@Z
// Retail: Fable.exe @ 0x00434840, 44 bytes, `ret 0Ch` (two floats and the colour by value); Ego_r.pdb / FableWin.pdb
// ?Open@CFadeInFadeOutBase@@QAEXMMVCRGBColour@@@Z. Catalog label CEngine::StartNewScene is wrong: the stores are the
// CFadeInFadeOutBase fields (Active +0 = true, FadeInTime +4, FadeOutTime +8, Closing +0xc = false, Opening +0xd = true,
// OpenTimer +0x10 = 0, CloseTimer +0x14 = the fade-out time, ToColour +0x18), and the 15 callers are the same GUI classes
// that construct it with 0x00436560 (CSubtitleRenderer, NPlayerGui::CDraw*, NHeroInformationScreens::CManager, CTCTrapBase).
// Layout: as 0x00436560 -- engine/CFadeInFadeOutBase.h models ToColour as opaque bytes, retail passes a CRGBColour by value.
// Shape: /O2; any statement order that sets the fields (CloseTimer from the argument or from FadeOutTime) schedules the same.
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CFadeInFadeOutBase {
    bool       Active;                                   // +0x00
    float      FadeInTime;                               // +0x04
    float      FadeOutTime;                              // +0x08
    bool       Closing;                                  // +0x0c
    bool       Opening;                                  // +0x0d
    float      OpenTimer;                                // +0x10
    float      CloseTimer;                               // +0x14
    CRGBColour ToColour;                                 // +0x18
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour);
};
void CFadeInFadeOutBase::Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)
{
    Active = true;
    FadeInTime = fade_in_time;
    FadeOutTime = fade_out_time;
    Opening = true;
    Closing = false;
    OpenTimer = 0.0f;
    CloseTimer = fade_out_time;
    ToColour = to_colour;
}