// Leaf: ?SetEditBoxRegion@CIME@NUISystem@@QAEXJJJJ@Z
// NUISystem::CIME::SetEditBoxRegion(long x, long y, long width, long height) -- tell the IME UI where the active edit box is on screen
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404BB0, length 80 bytes.
//
// The composition window (the floating box showing the characters a player is composing) has to sit
// over the edit control that owns the text, so whenever an edit box gains focus or moves it reports its
// rectangle here. We convert x/y/width/height to a left/top/right/bottom RECT and hand it to
// ImeUi_SetCompStringAppearance together with the font from our Appearance block and -1 (keep the
// current colour). Nothing happens unless the IME was initialised at start-up.
//
// Matching: a 16-byte RECT local, the width held in esi for the `right` add, `or edx,0FFFFFFFFh` for
// the -1 colour, `ret 10h`.
struct tagRECT { long left, top, right, bottom; };
// IME UI: position and style the composition string (font, colour, rectangle).
class CImeUiFont_Base;
void __fastcall ImeUi_SetCompStringAppearance(CImeUiFont_Base* font, unsigned long colour, const tagRECT* rect);   // 0x00CB2880 (Ego_r spelling, as row 0x00CB2880 defines it)
namespace NUISystem {
class CIME {
public:
    unsigned char    _pad00[0x8];
    bool             IsInitialised;                // +0x08
    unsigned char    _pad09[0x13];
    CImeUiFont_Base* m_pFont;                      // +0x1C Appearance.font: the font the composition string is drawn in
    void SetEditBoxRegion(long x, long y, long width, long height);
};
}

void NUISystem::CIME::SetEditBoxRegion(long x, long y, long width, long height)
{
    if (IsInitialised) {
        // Edit box rectangle in screen space.
        tagRECT rect;
        rect.left = x;
        rect.top = y;
        rect.right = x + width;
        rect.bottom = y + height;
        ImeUi_SetCompStringAppearance(m_pFont, -1, &rect);
    }
}
