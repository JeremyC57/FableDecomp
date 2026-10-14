// CIME::SetEditBoxRegion(long x, long y, long width, long height) -- when the IME is active (+8), hand the edit box rectangle (left/top, right = x + width, bottom = y + height) to ImeUi_SetCompStringAppearance (0x00CB2880, `__fastcall`) with the font at +0x1c and colour -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404BB0, length 80 bytes.
// Shape: a 16-byte RECT frame, the width in esi, `or edx,0FFFFFFFFh`, `ret 10h`.
struct SRect_00404bb0 { long left, top, right, bottom; };
void __fastcall ImeUi_SetCompStringAppearance(void* font, long colour, const SRect_00404bb0* rect);   // 0x00CB2880
struct CIME {
    unsigned char _pad[8];
    bool          m_active;                        // +0x08
    unsigned char _pad09[0x13];
    void*         m_pFont;                         // +0x1c
    void SetEditBoxRegion(long x, long y, long width, long height);
};

void CIME::SetEditBoxRegion(long x, long y, long width, long height)
{
    if (m_active) {
        SRect_00404bb0 rect;
        rect.left = x;
        rect.top = y;
        rect.right = x + width;
        rect.bottom = y + height;
        ImeUi_SetCompStringAppearance(m_pFont, -1, &rect);
    }
}