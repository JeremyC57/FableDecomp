// CIME::SetEditBoxRegion(long x, long y, long width, long height) -- tell the IME UI where the active edit box is on screen
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
struct SRect_00404bb0 { long left, top, right, bottom; };
// IME UI: position and style the composition string (font, colour, rectangle).
void __fastcall ImeUi_SetCompStringAppearance(void* font, long colour, const SRect_00404bb0* rect);   // 0x00CB2880
struct CIME {
    unsigned char _pad[8];
    bool          m_active;                        // IsInitialised
    unsigned char _pad09[0x13];
    void*         m_pFont;                         // Appearance.font: the font the composition string is drawn in
    void SetEditBoxRegion(long x, long y, long width, long height);
};

void CIME::SetEditBoxRegion(long x, long y, long width, long height)
{
    if (m_active) {
        // Edit box rectangle in screen space.
        SRect_00404bb0 rect;
        rect.left = x;
        rect.top = y;
        rect.right = x + width;
        rect.bottom = y + height;
        ImeUi_SetCompStringAppearance(m_pFont, -1, &rect);
    }
}
