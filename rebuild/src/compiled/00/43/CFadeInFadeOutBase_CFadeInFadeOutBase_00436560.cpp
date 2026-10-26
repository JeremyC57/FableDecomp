// CFadeInFadeOutBase::CFadeInFadeOutBase() -- inactive, not opening or closing, timers at zero, fade-to colour opaque black
// Leaf: ??0CFadeInFadeOutBase@@QAE@XZ
// Retail: Fable.exe @ 0x00436560, 43 bytes, returns this. Catalog label CTCHeroStats::SoberUp is wrong: the 14 callers
// are constructors that run it on 28-byte members (CSubtitleRenderer 0x0049166E at +0xe8/+0x104, NPlayerGui::CDrawMiniMap
// 0x0064E83F at +0x100/+0x11c/+0x138 -- landed there as CFadeInFadeOutBase::CFadeInFadeOutBase() @ 0x00436560 --,
// CDrawHealth, CDrawGuildSeal, CDrawQuickAccessIcons, NHeroInformationScreens::CManager, CTCTrapBase) and on a 0x1c-byte
// stack temporary (0x00594E00). The stores (+0 byte, +0xc/+0xd bytes, +0x10/+0x14 floats, colour at +0x18) are exactly
// Ego_r.pdb's CFadeInFadeOutBase; FadeInTime/FadeOutTime are left unset.
// Layout: rebuild/include/engine/CFadeInFadeOutBase.h models ToColour as opaque bytes, but retail builds a CRGBColour
// (B,G,R,A) temporary on the stack and copies it as a dword, so the class is spelled here with a real CRGBColour.
// Shape: body assignments (retail order Active, CloseTimer, OpenTimer, Opening, Closing, ToColour), not an init list.
#include <stddef.h>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CFadeInFadeOutBase { public:
    bool       Active;                                   // +0x00
    float      FadeInTime;                               // +0x04
    float      FadeOutTime;                              // +0x08
    bool       Closing;                                  // +0x0c
    bool       Opening;                                  // +0x0d
    float      OpenTimer;                                // +0x10
    float      CloseTimer;                               // +0x14
    CRGBColour ToColour;                                 // +0x18
    CFadeInFadeOutBase();
};
CFadeInFadeOutBase::CFadeInFadeOutBase()
{
    Active = false;
    CloseTimer = 0.0f;
    OpenTimer = 0.0f;
    Opening = false;
    Closing = false;
    ToColour = CRGBColour(0, 0, 0, 255);
}