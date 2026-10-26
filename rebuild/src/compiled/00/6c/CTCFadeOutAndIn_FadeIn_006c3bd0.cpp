// CTCFadeOutAndIn::FadeIn(float seconds, bool immediately) -- start a fadein over seconds (frames-per-second * seconds rounded to a frame count), the alpha set at once when immediately, else taken from the thing's graphic
// Leaf: ?FadeIn@CTCFadeOutAndIn@@QAEXM_N@Z
// Flags: /O2 /Oy /W3
// Retail-asm: 0x006C3BE8 `fld dword ptr [esp+8]` + 0x006C3BEC `fistp dword ptr [esp]` -- the engine's inline-assembly ROUND helper: the frame count product is stored into the dead `seconds` argument slot and reloaded for a bare dword fistp into the `push ecx` frame word; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload, so the dword fistp is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C3BD0, length 83 bytes. Twin of 0x006C3B70 / 0x006C3BD0 (FadeOut / FadeIn).
// Layout: PDB CTCFadeOutAndIn (40 bytes) over CTCBase (engine/CTCBase.h): FadingOut +0xc, FadingIn +0xd, Alpha +0x10,
// FramesLeftForFade +0x14. The thing's graphic record hangs off +0x64 with its alpha byte at +0x5a.
// Shape: the frame count is `fild g_framesPerSecond (0x01375550) ; fmul seconds`; the graphic alpha is `fild` of the
// zero-extended byte through the dead `immediately` slot times 1/255 (pool constant @ 0x01231724).
#include "engine/CTCBase.h"
extern int g_framesPerSecond;                      // 0x01375550
struct CThingGraphic {
    unsigned char _pad[0x5a];
    unsigned char m_alpha;                         // +0x5a
};
class CThing { public:
    unsigned char  _pad[0x64];
    CThingGraphic* m_pGraphic;                     // +0x64
};
class CTCFadeOutAndIn : public CTCBase { public:
    bool  FadingOut;                               // +0x0c
    bool  FadingIn;                                // +0x0d
    float Alpha;                                   // +0x10
    long  FramesLeftForFade;                       // +0x14
    void FadeIn(float seconds, bool immediately);  // 0x006C3BD0
    void FadeOut(float seconds, bool immediately); // 0x006C3B70
};
// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains __asm in
// place; its by-value parameter is homed in the dead `seconds` argument slot (`fstp [esp+8] ; fld [esp+8]`)
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
void CTCFadeOutAndIn::FadeIn(float seconds, bool immediately)
{
    FadingIn = true;
    FadingOut = false;
    FramesLeftForFade = ROUND(g_framesPerSecond * seconds);
    if (immediately)
        Alpha = 0.0f;
    else if (ParentThing->m_pGraphic)
        Alpha = ParentThing->m_pGraphic->m_alpha * (1.0f / 255.0f);
}