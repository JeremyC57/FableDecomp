// CIME::Render() -- draw the IME composition string and the front end's IME indicator sprite each frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404C00, length 80 bytes.
//
// Called from the front end's render pass. When the IME was initialised we first publish the alpha of
// the edit box's current look (the edit box object at +0x4C, virtual slot 27 gives its style record,
// byte 3 is the alpha) into the global the IME UI reads, or 0 when no edit box is attached, so the
// composition window fades with the control that owns it. Then the IME UI draws the composition and
// candidate strings, and finally the front-end manager's IME sprite (the on-screen mode indicator) is
// drawn if the front end created one.
//
// Matching: `mov dl,1 ; mov cl,dl` for the two identical bool flags, the global byte store, and the
// last call is a `jmp` tail into CSprite::DrawWithRenderManager @ 0x0041A980.
extern unsigned char g_imeCompositionAlpha_013750b0;   // alpha the IME UI draws the composition with
struct SThemeRecord_00404c00 { unsigned char _pad[3]; unsigned char m_alpha; };   // edit box style record; byte 3 is alpha
struct CThemeProvider_00404c00 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026();
    virtual SThemeRecord_00404c00* GetTheme();     // slot 27: the control's current style record
};
// IME UI: draw the composition / candidate strings (0x00CB38E0).
void __fastcall ImeUI_RenderCompositionString(bool a, bool b);
struct CSprite { void DrawWithRenderManager(); };   // 0x0041A980
struct CFrontEndManager_00404c00 {
    unsigned char _pad[0xb8];
    CSprite*      m_pImeSprite;                    // the IME mode indicator sprite, if created
    CSprite* GetImeSprite() { return m_pImeSprite; }
    static CFrontEndManager_00404c00* __fastcall GetInstance();   // 0x0041E5F2
};
struct CIME {
    unsigned char             _pad[8];
    bool                      m_active;            // IsInitialised
    unsigned char             _pad09[0x43];
    CThemeProvider_00404c00*  m_pTheme;            // PEditBox: the edit box currently taking IME input
    void Render();
};

void CIME::Render()
{
    if (m_active) {
        // Composition alpha follows the owning edit box.
        if (m_pTheme)
            g_imeCompositionAlpha_013750b0 = m_pTheme->GetTheme()->m_alpha;
        else
            g_imeCompositionAlpha_013750b0 = 0;
        bool flags = true;
        ImeUI_RenderCompositionString(flags, flags);
        // Mode indicator, when the front end has one.
        CSprite* sprite = CFrontEndManager_00404c00::GetInstance()->GetImeSprite();
        if (sprite)
            sprite->DrawWithRenderManager();
    }
}
