// CTCFadeOutAndIn::FadeOut(float seconds, bool immediately) -- start fading the thing out over `seconds` (converted to frames at the game's frame rate); the alpha starts at 1.0 when `immediately`, else at the thing's current graphic alpha
// Grade: DIFFER(ours v retail) -- 88v90 under /QIfist: retail rounds with an inlined inline-asm helper (`fstp [esp+8] ; fld [esp+8] ; fistp dword ptr [esp]` into a `push ecx` frame word); /QIfist emits `fistp qword ptr [esp]` into an 8-byte frame, plain C++ calls __ftol2. Lane gap: asm_bake (inline-asm rounding helper)
// Flags: /QIfist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C3B70, length 89 bytes.
// Callers: CGameScriptInterface::EntityFadeOut @ 0x008A0720 (so the manifest's "FadeIn" label is the twin's, 0x006C3BD0),
// CWorld::UpdateRegionLoad, CTCDExperienceOrb::FrameUpdate, CTCLifeTimer.
// Layout: PDB CTCFadeOutAndIn (40 bytes) over CTCBase (engine/CTCBase.h): FadingOut +0xc, FadingIn +0xd, Alpha +0x10,
// FramesLeftForFade +0x14. The thing's graphic record hangs off +0x64 with its alpha byte at +0x5a.
// Shape: the frame count is `fild g_framesPerSecond (0x01375550) ; fmul seconds`, narrowed to float through the dead
// `seconds` slot (`fstp [esp+8] ; fld [esp+8]`) and converted with a bare `fistp` into the `push ecx` frame word (no
// __ftol2: the round-to-nearest conversion); the graphic alpha is `fild` of the zero-extended byte through the dead
// `immediately` slot times 1/255 (pool constant @ 0x01231724).
#include "engine/CTCBase.h"

extern int g_framesPerSecond;                      // 0x01375550

struct CThingGraphic {
    unsigned char _pad[0x5a];
    unsigned char m_alpha;                         // +0x5a
};

struct CThing {
    unsigned char  _pad[0x64];
    CThingGraphic* m_pGraphic;                     // +0x64
};

struct CTCFadeOutAndIn : CTCBase {
    bool  FadingOut;                               // +0x0c
    bool  FadingIn;                                // +0x0d
    float Alpha;                                   // +0x10
    long  FramesLeftForFade;                       // +0x14
    void FadeOut(float seconds, bool immediately);
};

void CTCFadeOutAndIn::FadeOut(float seconds, bool immediately)
{
    FadingOut = true;
    FadingIn = false;
    float frames = g_framesPerSecond * seconds;
    FramesLeftForFade = (long)frames;
    if (immediately)
        Alpha = 1.0f;
    else if (ParentThing->m_pGraphic)
        Alpha = ParentThing->m_pGraphic->m_alpha * (1.0f / 255.0f);
}
